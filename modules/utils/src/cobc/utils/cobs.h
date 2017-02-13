/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_COBS_H
#define COBC_UTILS_COBS_H

#include <stdint.h>
#include <stddef.h>

#include "bounded_array.h"

namespace cobc
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
    CobsEncodingGeneratorBase(cobc::BoundedArray<const uint8_t> input);

    ~CobsEncodingGeneratorBase();

    CobsEncodingGeneratorBase(const CobsEncodingGeneratorBase& other);

    CobsEncodingGeneratorBase&
    operator =(const CobsEncodingGeneratorBase& other);

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
    encode(cobc::BoundedArray<const uint8_t> input,
		   cobc::BoundedArray<uint8_t> output);

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
    decode(cobc::BoundedArray<const uint8_t> input,
           uint8_t* output);
};

typedef CobsEncodingGeneratorBase<254> CobsEncodingGenerator;
typedef CobsBase<254> Cobs;

}
}

#include "cobs_impl.h"

#endif // COBC_UTILS_COBS_H
