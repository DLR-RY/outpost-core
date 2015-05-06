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

#include "cobs.h"

#include <string.h>     // for memcpy

using cobc::utils::CobsEncodingGenerator;
using cobc::utils::Cobs;

// ----------------------------------------------------------------------------
CobsEncodingGenerator::CobsEncodingGenerator(const uint8_t* data,
                                             size_t length) :
    mData(data),
    mLength(length),
    mCurrentPosition(0),
    mNextBlock(0),
    mZeroElementSkip(false)
{
}

CobsEncodingGenerator::~CobsEncodingGenerator()
{
}

CobsEncodingGenerator::CobsEncodingGenerator(const CobsEncodingGenerator& other) :
    mData(other.mData),
    mLength(other.mLength),
    mCurrentPosition(other.mCurrentPosition),
    mNextBlock(other.mNextBlock),
    mZeroElementSkip(other.mZeroElementSkip)
{

}

CobsEncodingGenerator&
CobsEncodingGenerator::operator=(const CobsEncodingGenerator& other)
{
    // this handles self assignment gracefully
    mData = other.mData;
    mLength = other.mLength;
    mCurrentPosition = other.mCurrentPosition;
    mNextBlock = other.mNextBlock;
    mZeroElementSkip = other.mZeroElementSkip;

    return *this;
}

uint8_t
CobsEncodingGenerator::getNextByte()
{
    uint8_t value;
    if (mNextBlock == 0)
    {
        if (mZeroElementSkip)
        {
            // Zero values are replaced with length of the following block
            // and are therefore skipped here. This has to be done before
            // calling findNextBlock() because it starts on the current
            // character.
            mCurrentPosition++;
        }
        else
        {
            mZeroElementSkip = true;
        }
        mNextBlock = findNextBlock();

        if (mNextBlock == maximumBlockLength)
        {
            mZeroElementSkip = false;
        }

        value = mNextBlock + 1;
    }
    else
    {
        value = mData[mCurrentPosition];
        mCurrentPosition++;
        mNextBlock--;
    }

    return value;
}

uint8_t
CobsEncodingGenerator::findNextBlock()
{
    uint8_t blockSize = 0;
    size_t position = mCurrentPosition;

    // Loop until either:
    // - A zero is found which determines the block length
    // - No zero is found for 254 consecutive bytes
    // - The end of the input array is reached.
    while ((mData[position] != 0) &&
           (blockSize < maximumBlockLength) &&
           (position < mLength))
    {
        position++;
        blockSize++;
    }

    return blockSize;
}

// ----------------------------------------------------------------------------
size_t
Cobs::encode(const uint8_t* input,
             size_t inputLength,
             uint8_t* output,
             size_t maximumOutputLength)
{
    const uint8_t* inputEnd = input + inputLength;

    // Pointer to the position where later the block length is inserted
    uint8_t* blockLengthPtr = output++;
    size_t length = 1;
    uint8_t blockLength = 0;

    while ((input < inputEnd) && (length < maximumOutputLength))
    {
        if (*input == 0)
        {
            *blockLengthPtr = blockLength + 1;
            blockLengthPtr = output++;
            length++;
            blockLength = 0;
        }
        else
        {
            *output++ = *input;
            length++;
            blockLength++;
            if ((blockLength == maximumBlockLength) && (length < maximumOutputLength))
            {
                *blockLengthPtr = blockLength + 1;
                blockLengthPtr = output++;
                length++;
                blockLength = 0;
            }
        }
        input++;
    }
    *blockLengthPtr = blockLength + 1;

    return length;
}

size_t
Cobs::getMaximumSizeOfEncodedData(size_t inputLength)
{
    size_t length = inputLength;
    if (inputLength == 0)
    {
        length = 1;
    }
    else
    {
        length += (inputLength - 1) / Cobs::maximumBlockLength + 1;
    }

    return length;
}

size_t
Cobs::decode(const uint8_t* input,
             size_t inputLength,
             uint8_t* output)
{
    size_t outputPosition = 0;
    const uint8_t* end = input + inputLength;

    while (input < end)
    {
        uint8_t blockLength = *input++ - 1;

        // memmove instead of memcpy is needed here because input and output
        // array may overlap.
        memmove(&output[outputPosition], input, blockLength);
        outputPosition += blockLength;
        input += blockLength;

        if (blockLength < maximumBlockLength)
        {
            // The last (implicit) zero is suppressed and not output.
            if (input < end)
            {
                output[outputPosition] = 0;
                outputPosition++;
            }
        }
    }

    return outputPosition;
}
