/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_COBS_IMPL_H
#define OUTPOST_UTILS_COBS_IMPL_H

#include "cobs.h"

#include <string.h>     // for memcpy

namespace outpost
{
namespace utils
{

// ----------------------------------------------------------------------------
template <uint8_t blockLength>
CobsEncodingGeneratorBase<blockLength>::CobsEncodingGeneratorBase(outpost::Slice<const uint8_t> input) :
    mData(&input[0]),
    mLength(input.getNumberOfElements()),
    mCurrentPosition(0),
    mNextBlock(0),
    mZeroElementSkip(false)
{
}

template <uint8_t blockLength>
CobsEncodingGeneratorBase<blockLength>::~CobsEncodingGeneratorBase()
{
}

template <uint8_t blockLength>
CobsEncodingGeneratorBase<blockLength>::CobsEncodingGeneratorBase(const CobsEncodingGeneratorBase& other) :
    mData(other.mData),
    mLength(other.mLength),
    mCurrentPosition(other.mCurrentPosition),
    mNextBlock(other.mNextBlock),
    mZeroElementSkip(other.mZeroElementSkip)
{

}

template <uint8_t blockLength>
CobsEncodingGeneratorBase<blockLength>&
CobsEncodingGeneratorBase<blockLength>::operator=(const CobsEncodingGeneratorBase& other)
{
    // this handles self assignment gracefully
    mData = other.mData;
    mLength = other.mLength;
    mCurrentPosition = other.mCurrentPosition;
    mNextBlock = other.mNextBlock;
    mZeroElementSkip = other.mZeroElementSkip;

    return *this;
}

template <uint8_t blockLength>
uint8_t
CobsEncodingGeneratorBase<blockLength>::getNextByte()
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

        if (mNextBlock == blockLength)
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

template <uint8_t blockLength>
uint8_t
CobsEncodingGeneratorBase<blockLength>::findNextBlock()
{
    uint8_t blockSize = 0;
    size_t position = mCurrentPosition;

    // Loop until either:
    // - A zero is found which determines the block length
    // - No zero is found for 254 consecutive bytes
    // - The end of the input array is reached.
    while ((mData[position] != 0) &&
           (blockSize < blockLength) &&
           (position < mLength))
    {
        position++;
        blockSize++;
    }

    return blockSize;
}

// ----------------------------------------------------------------------------
template <uint8_t blockLength>
size_t
CobsBase<blockLength>::encode(outpost::Slice<const uint8_t> input,
						      outpost::Slice<uint8_t> output)
{
	const uint8_t* inputPtr = &input[0];
    const uint8_t* inputEnd = inputPtr + input.getNumberOfElements();
    uint8_t* outputPtr = &output[0];

    // Pointer to the position where later the block length is inserted
    uint8_t* blockLengthPtr = outputPtr++;
    size_t length = 1;
    uint8_t currentBlockLength = 0;

    while ((inputPtr < inputEnd) && (length < output.getNumberOfElements()))
    {
        if (*inputPtr == 0)
        {
            *blockLengthPtr = currentBlockLength + 1;
            blockLengthPtr = outputPtr++;
            length++;
            currentBlockLength = 0;
        }
        else
        {
            *outputPtr++ = *inputPtr;
            length++;
            currentBlockLength++;
            if ((currentBlockLength == blockLength) && (length < output.getNumberOfElements()))
            {
                *blockLengthPtr = currentBlockLength + 1;
                blockLengthPtr = outputPtr++;
                length++;
                currentBlockLength = 0;
            }
        }
        inputPtr++;
    }
    *blockLengthPtr = currentBlockLength + 1;

    return length;
}

template <uint8_t blockLength>
size_t
CobsBase<blockLength>::getMaximumSizeOfEncodedData(size_t inputLength)
{
    size_t length = inputLength;
    if (inputLength == 0)
    {
        length = 1;
    }
    else
    {
        length += (inputLength - 1) / blockLength + 1;
    }

    return length;
}

template <uint8_t blockLength>
size_t
CobsBase<blockLength>::decode(outpost::Slice<const uint8_t> input,
						      uint8_t* output)
{
    size_t outputPosition = 0;
    const uint8_t* inputPtr = &input[0];
	const uint8_t* inputEnd = inputPtr + input.getNumberOfElements();

    while (inputPtr < inputEnd)
    {
        uint8_t data = *inputPtr++;
        if (data == 0)
        {
            outputPosition = 0;
            inputPtr = inputEnd;
        }
        else
        {
            uint8_t currentBlockLength = data - 1;

            // memmove instead of memcpy is needed here because the input and output
            // array may overlap.
            memmove(&output[outputPosition], inputPtr, currentBlockLength);
            outputPosition += currentBlockLength;
            inputPtr += currentBlockLength;

            if (currentBlockLength < blockLength)
            {
                // The last (implicit) zero is suppressed and not output.
                if (inputPtr < inputEnd)
                {
                    output[outputPosition] = 0;
                    outputPosition++;
                }
            }
        }
    }

    return outputPosition;
}

}
}

#endif
