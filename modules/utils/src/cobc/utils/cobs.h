/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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
class CobsEncodingGenerator
{
public:
    /// Maximum length of a COBS block
    static const uint8_t maximumBlockLength = 254;

    /**
     * Construct a COBS generator object.
     *
     * \param data
     *     Input data field.
     * \param length
     *     Length of the input data field in bytes.
     */
    CobsEncodingGenerator(const uint8_t* data, size_t length);

    ~CobsEncodingGenerator();

    CobsEncodingGenerator(const CobsEncodingGenerator& other);

    CobsEncodingGenerator&
    operator =(const CobsEncodingGenerator& other);

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
class Cobs
{
public:
    /// Maximum length of a COBS block
    static const uint8_t maximumBlockLength = 254;

    /**
     * Encode a byte array.
     *
     * \param input
     *     Input buffer of data to be encoded.
     * \param inputLength
     *     Length in bytes of the input buffer.
     * \param output
     *     Output buffer. The output buffer needs to be one byte per 254 byte
     *     longer than the input buffer.
     * \param maximumOutputLength
     *     Available size in the output buffer. The encoding is aborted if
     *     the length of the encoded output reaches this number.
     *
     * \return
     *     Number of bytes of the output data field actually used. This value
     *     will never be bigger than \p maximumOutputLength.
     */
    static size_t
    encode(const uint8_t* input,
           size_t inputLength,
           uint8_t* output,
           size_t maximumOutputLength);

    static size_t
    getMaximumSizeOfEncodedData(size_t inputLength);

    /**
     * Decode a COBS encoded array.
     *
     * \param input
     *     Input buffer of the COBS encoded data to be decoded.
     * \param inputLength
     *     Length in bytes of the input buffer.
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
    decode(const uint8_t* input,
           size_t inputLength,
           uint8_t* output);
};

}
}

#endif // COBC_UTILS_COBS_H
