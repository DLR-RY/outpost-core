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

#ifndef OUTPOST_UTILS_COMPRESSION_NLS_TRANSFORM_H_
#define OUTPOST_UTILS_COMPRESSION_NLS_TRANSFORM_H_

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
class NLSTransformer
{
private:
    static constexpr uint16_t MAX_LENGTH = 4096U;

public:
    enum Marker
    {
        NM,
        MIP,
        MNP,
        MSP,
        MCP,
        MD,
        MG,
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

    NLSTransformer() = default;

    ~NLSTransformer() = default;

    void
    forward(int16_t* inBuffer, size_t inBufferLength, outpost::Bitstream& outBuffer);

    void
    forward(int16_t* inBuffer,
            size_t inBufferLength,
            outpost::Bitstream& outBuffer,
            uint8_t dcComponents,
            size_t maxBytes);

    void
    backward(outpost::Bitstream& inBuffer, int16_t* outBuffer, size_t& outBufferLength);

    Marker mark[MAX_LENGTH];
    int16_t dmax[MAX_LENGTH / 2];
    int16_t gmax[MAX_LENGTH / 4];

private:
    void
    push(uint16_t p_I, size_t p_BufferLength);

    inline static uint16_t
    skip(Marker p_M)
    {
        switch (p_M)
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

    inline static uint16_t
    isSkip(Marker p_M)
    {
        switch (p_M)
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

#endif /* OUTPOST_UTILS_COMPRESSION_NLS_TRANSFORM_H_ */
