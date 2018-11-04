/*
 * Copyright (c) 2014-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2018, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_COBS_H
#define OUTPOST_UTILS_COBS_H

#include <outpost/base/slice.h>

#include <stddef.h>
#include <stdint.h>

namespace outpost
{
namespace utils
{
/**
 * COBS (Consistent Overhead Byte Stuffing) encoding.
 *
 * This class implements a COBS encoding generator. The generator does not use
 * any additional buffer memory and outputs one byte at a time. It needs access
 * to the complete input array to calculate the COBS block sizes.
 *
 * Use this function only if you need the encoded data on a byte by byte base,
 * e.g. when loading a FIFO. The Cobs::encode() function is faster when
 * generating the data into an array in the memory.
 *
 * \author  Fabian Greif
 */
template <uint8_t blockLength>
class CobsEncodingGeneratorBase
{
public:
    /// Maximum length of a COBS block
    static const uint8_t maximumBlockLength = 254;

    /**
     * Construct a COBS generator object.
     *
     * \param data
     *     Input data field.
     */
    CobsEncodingGeneratorBase(outpost::Slice<const uint8_t> input);

    ~CobsEncodingGeneratorBase();

    CobsEncodingGeneratorBase(const CobsEncodingGeneratorBase& other);

    CobsEncodingGeneratorBase&
    operator=(const CobsEncodingGeneratorBase& other);

    inline bool
    isFinished()
    {
        return (mCurrentPosition == mLength);
    }

    uint8_t
    getNextByte();

private:
    uint8_t
    findNextBlock();

    const uint8_t* mData;
    size_t mLength;
    size_t mCurrentPosition;
    uint8_t mNextBlock;
    bool mZeroElementSkip;
};

/**
 * COBS (Consistent Overhead Byte Stuffing) encoding and decoding.
 *
 * This class implements a modified version of the original COBS algorithm. The
 * modifications allow to specify the available output buffer space and to
 * return the number of bytes actually needed.
 *
 * \author  Fabian Greif
 *
 * \see     http://conferences.sigcomm.org/sigcomm/1997/papers/p062.pdf
 * \see     http://en.wikipedia.org/wiki/Consistent_Overhead_Byte_Stuffing
 */
template <uint8_t blockLength>
class CobsBase
{
public:
    /// Maximum length of a COBS block
    static const uint8_t maximumBlockLength = 254;

    /**
     * Encode a byte array.
     *
     * \param input
     *     Input buffer of data to be encoded.
     * \param output
     *     Output buffer. The output buffer needs to be one byte per block
     *     length longer than the input buffer. The encoding is aborted if
     *     the length of the encoded output reaches this number.
     *
     * \return
     *     Number of bytes of the output data field actually used. This value
     *     will never be bigger than \p maximumOutputLength.
     */
    static size_t
    encode(outpost::Slice<const uint8_t> input, outpost::Slice<uint8_t> output);

    static size_t
    getMaximumSizeOfEncodedData(size_t inputLength);

    /**
     * Decode a COBS encoded array.
     *
     * \param input
     *     Input buffer of the COBS encoded data to be decoded.
     * \param output
     *     Output buffer. The buffer should have at least the same the size
     *     as the input buffer. It is possible to do the encoding in place
     *     by suppling the same pointer as input and output.
     *
     * \return
     *     Number of bytes of the output data field actually used. Will never
     *     be bigger than \p inputLength.
     */
    static size_t
    decode(outpost::Slice<const uint8_t> input, uint8_t* output);
};

typedef CobsEncodingGeneratorBase<254> CobsEncodingGenerator;
typedef CobsBase<254> Cobs;

}  // namespace utils
}  // namespace outpost

#include "cobs_impl.h"

#endif
