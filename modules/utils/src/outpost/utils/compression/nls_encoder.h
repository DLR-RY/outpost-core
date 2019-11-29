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

#ifndef OUTPOST_UTILS_COMPRESSION_NLS_ENCODER_H_
#define OUTPOST_UTILS_COMPRESSION_NLS_ENCODER_H_

#include <outpost/base/slice.h>
#include <outpost/utils/storage/bitstream.h>
#include <outpost/utils/storage/serialize.h>

namespace outpost
{
namespace compression
{
/**
 * Implementation of the No List Set Partitioning in Hierarchical Trees (SPIHT) encoding.
 * For further information, check:
 * https://ieeexplore.ieee.org/abstract/document/859236
 *
 * For the complete compression scheme, see:
 * https://elib.dlr.de/112826/
 */
class NLSEncoder
{
private:
    static constexpr uint16_t MAX_LENGTH = 4096U;

public:
    // State table markers
    enum Marker
    {
        NM,   // Not marked
        MIP,  // The coefficient is insignificant or untested for this bitplane
        MNP,  // The coefficient is newly significant so it will not be refined for this bitplane
        MSP,  // The coefficient is significant and will be refined in this bitplane
        MCP,  // Like MIP, but applied during partitioning in the IS pass leading to an immediate
              // test for significance
        MD,   // The coefficient is the first (lowest index) child in a set consisting of all
              // descendants of its parent
        MG,   // The coefficient is the first (lowest index) grandchild in  a set consisting of all
              // granddescendants of its grandparent coefficient, not including the grandparent
              // itself or its children

        // MN*: Used on the leading node of each lower level of an
        // insignificance tree
        MN2,
        MN3,
        MN4,
        MN5,
        MN6,
        MN7,
        MN8,
        MN9,
        MN10,
        MN11,
        MN12,
        MN13,
        MN14
    };

    NLSEncoder() = default;

    ~NLSEncoder() = default;

    /**
     * Forward No List SPIHT transform
     * WARNING: For handling bits correcty, the input buffer values will, after encoding the sign
     * bits, be converted to their absolute values.
     * @param inBuffer
     *     Input buffer to be encoded
     * @param outBuffer
     *     Bitstream to write the encoding to
     */
    void
    encode(outpost::Slice<int16_t> inBuffer, outpost::Bitstream& outBuffer);

    /**
     * Forward No List SPIHT transform
     * WARNING: For handling bits correcty, the input buffer values will, after encoding the sign
     * bits, be converted to their absolute values.
     * @param inBuffer
     *     Input buffer to be encoded
     * @param outBuffer
     *     Bitstream to write the encoding to
     * @param dcComponents
     *     Number of components that are not encoded but rather prepend the stream
     * @param maxBytes
     *     Maximum number of bytes to write to outBuffer
     */
    void
    encode(outpost::Slice<int16_t> inBuffer,
           outpost::Bitstream& outBuffer,
           uint8_t dcComponents,
           size_t maxBytes);

    /**
     * Backward transform from a No List SPIHT encoded bitstream to int16_t
     * @param inBuffer
     *     NLS encoded bitstream
     * @param outBuffer
     *     Result buffer
     * @return
     *     Length of the resulting buffer
     */
    outpost::Slice<int16_t>
    decode(outpost::Bitstream& inBuffer, outpost::Slice<int16_t> outBuffer);

    Marker mark[MAX_LENGTH];
    int16_t dmax[MAX_LENGTH / 2];
    int16_t gmax[MAX_LENGTH / 4];

private:
    /**
     * Push increasing MN* markings in the state marker table down the tree of coefficients
     * @param pI
     *     Starting index
     * @param pBufferLength
     *     Length of the mark buffer
     */
    void
    push(uint16_t pI, size_t pBufferLength);

    /**
     * Number of coefficients to skip during IP and REF passes depending on the current marker
     * @param pM
     *     Current marker
     * @return
     *     Number of coefficients to skip
     */
    inline static uint16_t
    skip(Marker pM)
    {
        switch (pM)
        {
            case MIP:
            case MNP:
            case MCP:
            case MSP:
            case NM: return 1;
            case MD:
            case MN2: return 2;
            case MG:
            case MN3: return 4;
            case MN4: return 8;
            case MN5: return 16;
            case MN6: return 32;
            case MN7: return 64;
            case MN8: return 128;
            case MN9: return 256;
            case MN10: return 512;
            case MN11: return 1024;
            case MN12: return 2048;
            case MN13: return 4096;
            case MN14: return 8192;
            default: return 1;
        }
    }

    /**
     * Number of coefficients to skip during the IS pass depending on the current marker
     * @param pM
     *     Current marker
     * @return
     *     Number of coefficients to skip
     */
    inline static uint16_t
    isSkip(Marker pM)
    {
        switch (pM)
        {
            case MCP:
            case NM: return 1;
            case MD:
            case MN2:
            case MIP:
            case MNP:
            case MSP: return 2;
            case MG:
            case MN3: return 4;
            case MN4: return 8;
            case MN5: return 16;
            case MN6: return 32;
            case MN7: return 64;
            case MN8: return 128;
            case MN9: return 256;
            case MN10: return 512;
            case MN11: return 1024;
            case MN12: return 2048;
            case MN13: return 4096;
            case MN14: return 8192;
            default: return 1;
        }
    }
};

}  // namespace compression
}  // namespace outpost

#endif /* OUTPOST_UTILS_COMPRESSION_NLS_ENCODER_H_ */
