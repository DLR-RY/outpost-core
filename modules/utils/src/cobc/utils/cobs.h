/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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

class ConsistentOverheadByteStuffing
{
public:
    ConsistentOverheadByteStuffing();

    void
    start(const uint8_t* data,
          size_t length);

    uint8_t
    getNextByte();

    size_t
    getEncodedArray(uint8_t* data, size_t maximumLength);

    bool
    isFinished();

private:
    uint8_t
    findNextBlock();

    const uint8_t* mData;
    size_t mLength;
    size_t mCurrentPosition;
    uint8_t mNextBlockSize;
    bool mBlockWithoutZero;
};

}
}

#endif // COBC_UTILS_COBS_H
