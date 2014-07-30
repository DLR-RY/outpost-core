/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "cobs.h"

using cobc::utils::ConsistentOverheadByteStuffing;

ConsistentOverheadByteStuffing::ConsistentOverheadByteStuffing() :
        mData(0),
        mLength(0),
        mCurrentPosition(0),
        mNextBlockSize(0),
        mBlockWithoutZero(false)
{
}

void
ConsistentOverheadByteStuffing::start(const uint8_t* data,
                                              size_t length)
{
    mData = data;
    mLength = length;
    mCurrentPosition = -1;
    mNextBlockSize = 0;
    mBlockWithoutZero = false;
}

size_t
ConsistentOverheadByteStuffing::getEncodedArray(uint8_t* data,
                                                        size_t maximumLength)
{
    size_t length = 0;
    while (!isFinished() && (length < maximumLength))
    {
        data[length] = getNextByte();
        ++length;
    }

    return length;

    /*const uint8_t* start = data;
    const uint8_t* end = mData + mLength;
    uint8_t* codePtr = data++;
    uint8_t code = 0x01;

    while (mData < end)
    {
        if (*mData == 0)
        {
            *codePtr = code;
            codePtr = data++;
            code = 0x01;
        }
        else
        {
            *data++ = *mData;
            code++;
            if (code == 0xFF)
            {
                *codePtr = code;
                codePtr = data++;
                code = 0x01;
            }
        }
        mData++;
    }

    *codePtr = code;

    return (data - start);*/
}

uint8_t
ConsistentOverheadByteStuffing::getNextByte()
{
    uint8_t value;
    if (mNextBlockSize == 0)
    {
        mNextBlockSize = findNextBlock();
        value = mNextBlockSize + 1;

        if (!mBlockWithoutZero)
        {
            ++mCurrentPosition;
            mBlockWithoutZero = false;
        }

        if (mNextBlockSize == 254)
        {
            mBlockWithoutZero = true;
        }
    }
    else
    {
        value = mData[mCurrentPosition];
        --mNextBlockSize;
        ++mCurrentPosition;
    }

    return value;
}

bool
ConsistentOverheadByteStuffing::isFinished()
{
    return (mCurrentPosition == mLength);
}

uint8_t
ConsistentOverheadByteStuffing::findNextBlock()
{
    uint8_t blockSize = 0;
    size_t position = mCurrentPosition + 1;
    while ((position < mLength) && (blockSize < 254) && (mData[position] != 0))
    {
        position++;
        blockSize++;
    }

    return blockSize;
}
