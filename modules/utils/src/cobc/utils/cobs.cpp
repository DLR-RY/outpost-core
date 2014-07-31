/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "cobs.h"

using cobc::utils::CobsEncodingGenerator;
using cobc::utils::CobsEncoder;

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
CobsEncodingGenerator::operator =(const CobsEncodingGenerator& other)
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

        if (mNextBlock == 254)
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

bool
CobsEncodingGenerator::isFinished()
{
    return (mCurrentPosition == mLength);
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
           (blockSize < 254) &&
           (position < mLength))
    {
        position++;
        blockSize++;
    }

    return blockSize;
}

// ----------------------------------------------------------------------------
size_t
CobsEncoder::encode(const uint8_t* input,
                    size_t inputLength,
                    uint8_t* output,
                    size_t maximumOutputLength)
{
    const uint8_t* inputEnd = input + inputLength;

    // Pointer to the position where later the block length is inserted
    uint8_t* blockLengthPtr = output++;
    size_t length = 1;
    uint8_t code = 1;

    while ((input < inputEnd) && (length < (maximumOutputLength - 1)))
    {
        if (*input == 0)
        {
            *blockLengthPtr = code;
            blockLengthPtr = output++;
            length++;
            code = 1;
        }
        else
        {
            *output++ = *input;
            length++;
            code++;
            if (code == 255)
            {
                *blockLengthPtr = code;
                blockLengthPtr = output++;
                length++;
                code = 1;
            }
        }
        input++;
    }
    *blockLengthPtr = code;

    return length;
}

size_t
CobsEncoder::decode(const uint8_t* input,
                    size_t inputLength,
                    uint8_t* output)
{
    size_t length = 0;
    const uint8_t* end = input + inputLength;

    while (input < end)
    {
        int i;
        int code = *input++;
        for (i = 1; i < code; i++)
        {
            *output++ = *input++;
            length++;
        }
        // The last (implicit) zero is suppressed and not output.
        if ((code < 0xFF) && (input < end)) {
            *output++ = 0;
            length++;
        }
    }

    return length;
}
