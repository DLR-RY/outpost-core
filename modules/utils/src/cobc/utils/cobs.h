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

class CobsEncodingGenerator
{
public:
    CobsEncodingGenerator(const uint8_t* data, size_t length);

    ~CobsEncodingGenerator();

    CobsEncodingGenerator(const CobsEncodingGenerator& other);

    CobsEncodingGenerator&
    operator =(const CobsEncodingGenerator& other);

    uint8_t
    getNextByte();

    bool
    isFinished();

private:
    uint8_t
    findNextBlock();

    const uint8_t* mData;
    size_t mLength;
    size_t mCurrentPosition;
    uint8_t mNextBlock;
    bool mZeroElementSkip;
};

class CobsEncoder
{
public:
    static size_t
    encode(const uint8_t* input,
           size_t inputLength,
           uint8_t* output,
           size_t maximumOutputLength);

    static size_t
    decode(const uint8_t* input,
           size_t inputLength,
           uint8_t* output);
};

}
}

#endif // COBC_UTILS_COBS_H
