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

#include "legall_wavelet.h"

#include <string.h>

namespace outpost
{
namespace compression
{
void LeGall53Wavelet::forwardTransform(outpost::Slice<FP<16>> inBuffer,
                                       outpost::Slice<FP<16>> outBuffer)
{
    size_t halfBufferLength = inBuffer.getNumberOfElements();

    size_t step;
    // Perform log2 passes, bisecting the buffer after each pass
    for (step = 0; halfBufferLength > 2; step++)
    {
        // Calculate high- and lowpass coefficients for the general case
        halfBufferLength = halfBufferLength >> 1;
        for (size_t i = 0; i < halfBufferLength - 2; i++)
        {
            // Lowpass
            outBuffer[i] = h0 * inBuffer[2 * i] + h1 * inBuffer[2 * i + 1]
                           + h2 * inBuffer[2 * i + 2] + h3 * inBuffer[2 * i + 3]
                           + h4 * inBuffer[2 * i + 4];
            // Highpass
            outBuffer[i + halfBufferLength] =
                    g0 * inBuffer[2 * i] + g1 * inBuffer[2 * i + 1] + g2 * inBuffer[2 * i + 2];
        }

        // Handle lapping cases
        outBuffer[halfBufferLength - 2] =
                h0 * inBuffer[2 * halfBufferLength - 4] + h1 * inBuffer[2 * halfBufferLength - 3]
                + h2 * inBuffer[2 * halfBufferLength - 2] + h3 * inBuffer[2 * halfBufferLength - 1]
                + h4 * inBuffer[0];
        outBuffer[2 * halfBufferLength - 2] = g0 * inBuffer[2 * halfBufferLength - 4]
                                              + g1 * inBuffer[2 * halfBufferLength - 3]
                                              + g2 * inBuffer[2 * halfBufferLength - 2];

        outBuffer[halfBufferLength - 1] = h0 * inBuffer[2 * halfBufferLength - 2]
                                          + h1 * inBuffer[2 * halfBufferLength - 1]
                                          + h2 * inBuffer[0] + h3 * inBuffer[1] + h4 * inBuffer[2];
        outBuffer[2 * halfBufferLength - 1] = g0 * inBuffer[2 * halfBufferLength - 2]
                                              + g1 * inBuffer[2 * halfBufferLength - 1]
                                              + g2 * inBuffer[0];

        // Swap buffers for the subsequent pass
        outpost::Slice<FP<16>> tmp = inBuffer;
        inBuffer = outBuffer;
        outBuffer = tmp;
    }

    // With coefficients of different levels spread through both buffers, these need to be copied to
    // outBuffer
    size_t steps = step;
    if (step % 2)
    {
        outpost::Slice<FP<16>> tmp = inBuffer;
        inBuffer = outBuffer;
        outBuffer = tmp;
        step = 2;
    }
    else
    {
        step = 1;
        memcpy(&outBuffer[0], &inBuffer[0], 16);
    }

    for (; step < steps; step += 2)
    {
        memcpy(&outBuffer[1 << step], &inBuffer[1 << step], (1 << (step + 2)));
    }
}

void LeGall53Wavelet::forwardTransformInPlace(outpost::Slice<FP<16>> inBuffer)
{
    int16_t length = inBuffer.getNumberOfElements();
    uint16_t inBufferLength = inBuffer.getNumberOfElements();

    // Perform log2 passes, bisecting the buffer after each pass
    for (uint16_t step = 0; length >= 3; step++)
    {
        // Temporarily save these for handling of lapping cases
        FP<16> tmpBuffer[3] = {inBuffer[0], inBuffer[1 << step], inBuffer[2 << step]};

        // Calculate highpass and lowpass coefficients using the lifting scheme
        for (size_t i = 0; ((i + 4) << step) < inBufferLength; i += 2)
        {
            inBuffer[(i << step)] = h0 * inBuffer[i << step] + h1 * inBuffer[(i + 1) << step]
                                    + h2 * inBuffer[(i + 2) << step]
                                    + h3 * inBuffer[(i + 3) << step]
                                    + h4 * inBuffer[(i + 4) << step];
            inBuffer[(i + 1) << step] =
                    ip_g0 * inBuffer[i << step] + ip_g2 * inBuffer[(i + 2) << step]
                    + ip_g3 * inBuffer[(i + 3) << step] + ip_g4 * inBuffer[(i + 4) << step];
        }
        // Handle corner cases with lapping coefficients.
        inBuffer[inBufferLength - (4 << step)] = h0 * inBuffer[inBufferLength - (4 << step)]
                                                 + h1 * inBuffer[inBufferLength - (3 << step)]
                                                 + h2 * inBuffer[inBufferLength - (2 << step)]
                                                 + h3 * inBuffer[inBufferLength - (1 << step)]
                                                 + h4 * tmpBuffer[0];
        inBuffer[inBufferLength - (3 << step)] = ip_g0 * inBuffer[inBufferLength - (4 << step)]
                                                 + ip_g2 * inBuffer[inBufferLength - (2 << step)]
                                                 + ip_g3 * inBuffer[inBufferLength - (1 << step)]
                                                 + ip_g4 * tmpBuffer[0];
        inBuffer[inBufferLength - (2 << step)] = h0 * inBuffer[inBufferLength - (2 << step)]
                                                 + h1 * inBuffer[inBufferLength - (1 << step)]
                                                 + h2 * tmpBuffer[0] + h3 * tmpBuffer[1]
                                                 + h4 * tmpBuffer[2];
        inBuffer[inBufferLength - (1 << step)] = ip_g0 * inBuffer[inBufferLength - (2 << step)]
                                                 + ip_g2 * tmpBuffer[0] + ip_g3 * tmpBuffer[1]
                                                 + ip_g4 * tmpBuffer[2];
        length >>= 1;
    }
}

outpost::Slice<int16_t> LeGall53Wavelet::reorder(outpost::Slice<FP<16>> inBuffer)
{
    // First, round the coefficients. Otherwise, the decimal places cannot be used as temporary
    // memory
    for (size_t i = 0; i < inBuffer.getNumberOfElements(); i++)
    {
        inBuffer[i] = FP<16>(static_cast<int16_t>(inBuffer[i]));
    }

    int16_t* outputBuffer = reinterpret_cast<int16_t*>(inBuffer.begin());
    size_t index = 2;
    outputBuffer[0] = static_cast<int16_t>(inBuffer[0]);
    // From left to right, cast the coefficients to int16_t for transmission and bring in the right
    // order for encoding
    for (size_t step = inBuffer.getNumberOfElements() >> 1; step >= 1; step >>= 1)
    {
        for (size_t i = step; i < inBuffer.getNumberOfElements(); i += 2 * step)
        {
            outputBuffer[index] = (inBuffer[i].getValue() >> 16) & 0xFFFF;
            index += 2;
        }
    }

    for (size_t i = 1; i < inBuffer.getNumberOfElements(); i++)
    {
        outputBuffer[i] = outputBuffer[2 * i];
    }

    return outpost::Slice<int16_t>::unsafe(outputBuffer, inBuffer.getNumberOfElements());
}

void
LeGall53Wavelet::backwardTransform(outpost::Slice<double> inBuffer,
                                   outpost::Slice<double> outBuffer)
{
    size_t halfBufferLength;

    for (size_t j = Log2(inBuffer.getNumberOfElements()) - 1; j >= 1; j--)
    {
        halfBufferLength = inBuffer.getNumberOfElements() >> j;
        outBuffer[0] = ih1 * inBuffer[halfBufferLength - 1] + ih0 * inBuffer[halfBufferLength]
                       + ih2 * inBuffer[2 * halfBufferLength - 1];
        outBuffer[1] = ig3 * inBuffer[halfBufferLength - 1] + ig1 * inBuffer[0]
                       + ig0 * inBuffer[2 * halfBufferLength - 1]
                       + ig4 * inBuffer[halfBufferLength + 1] + ig2 * inBuffer[halfBufferLength];

        for (size_t i = 1; i < halfBufferLength; i++)
        {
            outBuffer[2 * i] = ih1 * inBuffer[i - 1] + ih0 * inBuffer[halfBufferLength + i - 1]
                               + ih2 * inBuffer[halfBufferLength + i];
            outBuffer[2 * i + 1] = ig1 * inBuffer[i - 1] + ig3 * inBuffer[i]
                                   + ig0 * inBuffer[i - 1 + halfBufferLength]
                                   + ig2 * inBuffer[i + halfBufferLength]
                                   + ig4 * inBuffer[(i + 1) % halfBufferLength + halfBufferLength];
        }

        for (size_t i = 0; i < halfBufferLength * 2; i++)
        {
            inBuffer[i] = outBuffer[i];
        }
    }
}

const FP<16> LeGall53Wavelet::h0 = -0.125;
const FP<16> LeGall53Wavelet::h1 = 0.25;
const FP<16> LeGall53Wavelet::h2 = 0.75;
const FP<16> LeGall53Wavelet::h3 = 0.25;
const FP<16> LeGall53Wavelet::h4 = -0.125;

const FP<16> LeGall53Wavelet::g0 = -0.5;
const FP<16> LeGall53Wavelet::g1 = 1.0;
const FP<16> LeGall53Wavelet::g2 = -0.5;

const FP<16> LeGall53Wavelet::ip_g0 = 4.0;
const FP<16> LeGall53Wavelet::ip_g1 = 0.0;
const FP<16> LeGall53Wavelet::ip_g2 = -3.5;
const FP<16> LeGall53Wavelet::ip_g3 = -1.0;
const FP<16> LeGall53Wavelet::ip_g4 = 0.5;

const double LeGall53Wavelet::ih0 = -0.25;
const double LeGall53Wavelet::ih1 = 1.0;
const double LeGall53Wavelet::ih2 = -0.25;

const double LeGall53Wavelet::ig0 = -0.125;
const double LeGall53Wavelet::ig1 = 0.5;
const double LeGall53Wavelet::ig2 = 0.75;
const double LeGall53Wavelet::ig3 = 0.5;
const double LeGall53Wavelet::ig4 = -0.125;

}  // namespace compression
}  // namespace outpost
