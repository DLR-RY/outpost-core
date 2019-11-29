/*
 * Copyright (c) 2015-2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#include "nls_encoder.h"

#include <outpost/utils/log2.h>

#include <algorithm>
#include <iostream>

namespace outpost
{
namespace compression
{
void
NLSEncoder::encode(outpost::Slice<int16_t> inBuffer, Bitstream& outBuffer)
{
    encode(inBuffer, outBuffer, 2, 0);
}

void
NLSEncoder::encode(outpost::Slice<int16_t> inBuffer,
                   Bitstream& outBuffer,
                   uint8_t dcComponents,
                   size_t maxBytes)
{
    if (maxBytes == 0)
    {
        maxBytes = inBuffer.getNumberOfElements() << 1;
    }

    // Setup the maximum descendant and granddescendant arrays
    dmax[0] = 0;
    gmax[0] = 0;

    int16_t max = inBuffer[0];
    for (uint16_t i = 1; i < dcComponents; i++)
    {
        if (max < std::abs(inBuffer[i]))
        {
            max = std::abs(inBuffer[i]);
        }
    }

    for (uint16_t i = inBuffer.getNumberOfElements() - 1; i >= 2; i -= 2)
    {
        if (std::abs(inBuffer[i]) > max)
        {
            max = std::abs(inBuffer[i]);
        }

        if (std::abs(inBuffer[i - 1]) > max)
        {
            max = std::abs(inBuffer[i - 1]);
        }
        if (i<inBuffer.getNumberOfElements()>> 1)
        {
            dmax[i >> 1] = std::max<int16_t>(std::max<int16_t>(int16_t(std::abs(inBuffer[i - 1])),
                                                               int16_t(std::abs(inBuffer[i]))),
                                             std::max<int16_t>(dmax[i], dmax[i - 1]));
        }
        else
        {
            dmax[i >> 1] = std::max<int16_t>(int16_t(std::abs(inBuffer[i - 1])),
                                             int16_t(std::abs(inBuffer[i])));
        }
    }

    for (uint16_t i = 1; i<inBuffer.getNumberOfElements()>> 2; i++)
    {
        gmax[i] = std::max<int16_t>(dmax[i << 1], dmax[(i << 1) + 1]);
    }

    // Calculate the number of bitplanes
    int8_t n = Log2(max);
    uint16_t s = 1 << n;

    // Put the number of bitplanes in the output stream
    for (int8_t i = 3; i >= 0; i--)
    {
        outBuffer.pushBit(static_cast<uint16_t>(1 << i) & n);
    }

    // Put the number of DC components in the output stream
    for (int8_t i = 3; i >= 0; i--)
    {
        outBuffer.pushBit(static_cast<uint8_t>(1 << i) & dcComponents);
    }

    // Put the number of coefficients in the output stream
    size_t exponent = Log2(inBuffer.getNumberOfElements());
    for (int8_t i = 3; i >= 0; i--)
    {
        size_t tmp = static_cast<size_t>(1 << i) & exponent;
        outBuffer.pushBit(tmp > 0);
    }

    // Initialize the state marker table
    uint16_t i = 0;
    for (; i < dcComponents; i++)
    {
        mark[i] = MIP;
    }

    for (; i < dcComponents << 1; i++)
    {
        mark[i] = MD;
        push(i, inBuffer.getNumberOfElements());
    }

    for (; i < inBuffer.getNumberOfElements(); i++)
    {
        mark[i] = NM;
    }

    // Iterate over all bitplanes
    while (n >= 0)
    {
        // Insignificant Pixel Pass
        uint16_t j = 0;
        while (j < inBuffer.getNumberOfElements())
        {
            // If the coefficient is not yet tested, test it for significance
            if (mark[j] == MIP)
            {
                // If the coefficient is significant, mark it MNP and push this information and its
                // sign bit to the output stream
                bool sig = std::abs(inBuffer[j]) >= s;
                outBuffer.pushBit(sig);
                if (sig)
                {
                    outBuffer.pushBit(inBuffer[j] < 0);
                    mark[j] = MNP;
                    inBuffer[j] = std::abs(inBuffer[j]);
                }
                j++;
            }
            else
            {
                j += skip(mark[j]);
            }
        }

        // Break if the maximum number of output bytes is reached.
        if (outBuffer.getSize() > maxBytes)
        {
            break;
        }

        // Insignificant Set Pass
        j = 0;
        while (j < inBuffer.getNumberOfElements())
        {
            // Iterate over sets and inspect coefficients marked  MD, MG and MCP
            if (mark[j] == MD)
            {
                bool sig = dmax[j >> 1] >= s;
                outBuffer.pushBit(sig);
                if (sig)
                {
                    mark[j] = mark[j + 1] = MCP;
                    if (static_cast<size_t>(j << 1) < inBuffer.getNumberOfElements())
                    {
                        mark[j << 1] = MG;
                    }
                }
                else
                {
                    j += 2;
                }
            }
            else if (mark[j] == MG)
            {
                bool sig = gmax[j >> 2] >= s;
                outBuffer.pushBit(sig);
                if (sig)
                {
                    mark[j] = mark[j + 2] = MD;
                    push(j, inBuffer.getNumberOfElements());
                    push(j + 2, inBuffer.getNumberOfElements());
                }
                else
                {
                    j += 4;
                }
            }
            else if (mark[j] == MCP)
            {
                bool sig = std::abs(inBuffer[j]) >= s;
                outBuffer.pushBit(sig);
                if (sig)
                {
                    outBuffer.pushBit(inBuffer[j] < 0);
                    mark[j] = MNP;
                    inBuffer[j] = std::abs(inBuffer[j]);
                }
                else
                {
                    mark[j] = MIP;
                }
                j++;
            }
            else
            {
                j = j + isSkip(mark[j]);
            }
        }

        // Break if the maximum number of output bytes is reached.
        if (outBuffer.getSize() > maxBytes)
        {
            break;
        }

        // Refinement Pass
        j = 0;
        while (j < inBuffer.getNumberOfElements())
        {
            // Push significant coefficients to the output stream
            if (mark[j] == MSP)
            {
                outBuffer.pushBit((inBuffer[j] & s) > 0);
                j++;
            }
            // Newly identified significant coefficients shall be refined in the next pass
            else if (mark[j] == MNP)
            {
                mark[j] = MSP;
                j++;
            }
            else
            {
                j += skip(mark[j]);
            }
        }

        // Break if the maximum number of output bytes is reached.
        if (outBuffer.getSize() > maxBytes)
        {
            break;
        }

        // Calculate the next bitplane
        n--;
        s = s >> 1;
    }
}

void
NLSEncoder::push(uint16_t pI, size_t pBufferLength)
{
    pI = pI << 1;
    uint8_t offset = 0;
    while (pI < pBufferLength)
    {
        mark[pI] = Marker(MN2 + offset++);
        pI = pI << 1;
    }
}

outpost::Slice<int16_t> NLSEncoder::decode(Bitstream& inBuffer, outpost::Slice<int16_t> outBuffer)
{
    int8_t n;
    uint8_t dcComponents;
    n = 0x0F & (inBuffer.getByte(0) >> 4);
    dcComponents = inBuffer.getByte(0) & 0x0F;
    uint16_t s = 1 << n;

    size_t outBufferLength = 1 << ((inBuffer.getByte(1) >> 4) & 0x0F);
    if (outBufferLength < 8 || outBuffer.getNumberOfElements() < outBufferLength)
    {
        return outpost::Slice<int16_t>::empty();
    }
    bool signs[MAX_LENGTH];

    for (size_t i = 0; i < outBufferLength; i++)
    {
        mark[i] = NM;
        outBuffer[i] = 0;
        signs[i] = 0;
    }

    for (uint8_t i = 0; i < dcComponents; i++)
    {
        mark[i] = MIP;
    }

    for (uint8_t i = dcComponents; i < dcComponents << 1; i++)
    {
        mark[i] = MD;
        push(i, outBufferLength);
    }

    uint16_t buffer_bit_pointer = 12;

    while (n >= 0)
    {
        // IP Pass
        uint16_t i = 0;
        while (i < outBufferLength)
        {
            if (mark[i] == MIP)
            {
                bool sig = inBuffer.getBit(buffer_bit_pointer++);
                if (sig)
                {
                    signs[i] = inBuffer.getBit(buffer_bit_pointer++);
                    mark[i] = MNP;
                    outBuffer[i] += (1 - 2 * signs[i]) * (s + (s >> 1));
                }
                i++;
            }
            else
            {
                i += skip(mark[i]);
            }
        }

        if (buffer_bit_pointer >> 3 > inBuffer.getSize())
        {
            break;
        }

        // IS Pass
        i = 0;
        while (i < outBufferLength)
        {
            if (mark[i] == MD)
            {
                bool sig = inBuffer.getBit(buffer_bit_pointer++);
                if (sig)
                {
                    mark[i] = mark[i + 1] = MCP;
                    if (static_cast<size_t>(i << 1) < outBufferLength)
                    {
                        mark[i << 1] = MG;
                    }
                }
                else
                {
                    i = i + 2;
                }
            }
            else if (mark[i] == MG)
            {
                bool sig = inBuffer.getBit(buffer_bit_pointer++);
                if (sig)
                {
                    mark[i] = mark[i + 2] = MD;
                    push(i, outBufferLength);
                    push(i + 2, outBufferLength);
                }
                else
                {
                    i += 4;  //?
                }
            }
            else if (mark[i] == MCP)
            {
                bool sig = inBuffer.getBit(buffer_bit_pointer++);
                if (sig)
                {
                    signs[i] = inBuffer.getBit(buffer_bit_pointer++);
                    mark[i] = MNP;
                    outBuffer[i] += (1 - 2 * signs[i]) * (s + (s >> 1));
                }
                else
                {
                    mark[i] = MIP;
                }
                i++;
            }
            else
            {
                i = i + isSkip(mark[i]);
            }
        }

        if (buffer_bit_pointer >> 3 > inBuffer.getSize())
        {
            break;
        }

        // Ref Pass
        i = 0;
        while (i < outBufferLength)
        {
            if (mark[i] == MSP)
            {
                bool sig = inBuffer.getBit(buffer_bit_pointer++);

                if (sig)
                {
                    outBuffer[i] += (1 - 2 * signs[i]) * (s >> 1);
                }
                else
                {
                    outBuffer[i] += (1 - 2 * signs[i]) * ((s >> 1) - s);
                }
                i++;
            }
            else if (mark[i] == MNP)
            {
                mark[i] = MSP;
                i++;
            }
            else
            {
                i += skip(mark[i]);
            }
        }

        if (buffer_bit_pointer >> 3 > inBuffer.getSize())
        {
            break;
        }

        n--;
        s = s >> 1;
    }

    return outBuffer.first(outBufferLength);
}

}  // namespace compression
}  // namespace outpost
