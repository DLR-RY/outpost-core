/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#ifndef COBC_UTILS_SERIALIZE_H
#define COBC_UTILS_SERIALIZE_H

#include <stddef.h>
#include <string.h>
#include <stdint.h>

namespace cobc
{

struct uint24_t
{
    static const int byteLength = 3;

    uint8_t value[byteLength];
};

/**
 * \author Fabian Greif
 */
// LCOV_EXCL_START
// Functions are tested in \c test/unit/test_byteorder.cpp
// But as they are inline functions lcov can't generate useful
// coverage reports for them
class Serialize
{
public:
    explicit inline
    Serialize(uint8_t* outputBuffer) :
            mBuffer(outputBuffer), mBegin(outputBuffer)
    {
    }

    inline
    ~Serialize()
    {
    }

    inline
    Serialize(const Serialize& other) :
            mBuffer(other.mBuffer), mBegin(other.mBegin)
    {
    }

    /**
     * Reset the read pointer to the beginning of the
     * originating buffer.
     */
    inline void
    reset()
    {
        mBuffer = mBegin;
    }

    inline void
    store8(uint8_t data)
    {
        mBuffer[0] = data;
        mBuffer += 1;
    }

    inline void
    store16(uint16_t data)
    {
        mBuffer[0] = static_cast<uint8_t>(data >> 8);
        mBuffer[1] = static_cast<uint8_t>(data >> 0);
        mBuffer += 2;
    }

    inline void
    store24(uint32_t data)
    {
        mBuffer[0] = static_cast<uint8_t>(data >> 16);
        mBuffer[1] = static_cast<uint8_t>(data >> 8);
        mBuffer[2] = static_cast<uint8_t>(data >> 0);
        mBuffer += 3;
    }

    inline void
    store32(uint32_t data)
    {
        mBuffer[0] = static_cast<uint8_t>(data >> 24);
        mBuffer[1] = static_cast<uint8_t>(data >> 16);
        mBuffer[2] = static_cast<uint8_t>(data >> 8);
        mBuffer[3] = static_cast<uint8_t>(data >> 0);
        mBuffer += 4;
    }

    inline void
    store64(uint64_t data)
    {
        mBuffer[0] = static_cast<uint8_t>(data >> 56);
        mBuffer[1] = static_cast<uint8_t>(data >> 48);
        mBuffer[2] = static_cast<uint8_t>(data >> 40);
        mBuffer[3] = static_cast<uint8_t>(data >> 32);
        mBuffer[4] = static_cast<uint8_t>(data >> 24);
        mBuffer[5] = static_cast<uint8_t>(data >> 16);
        mBuffer[6] = static_cast<uint8_t>(data >> 8);
        mBuffer[7] = static_cast<uint8_t>(data >> 0);
        mBuffer += 8;
    }

    /**
     * Store two 12 bit values in a three byte array.
     *
     * The 12 bit value need to be stored in the lower 12 bit of the
     * two 16 bit parameters.
     */
    inline void
    storePacked12(uint16_t first, uint16_t second)
    {
        mBuffer[0] = static_cast<uint8_t>(first >> 4);
        mBuffer[1] = static_cast<uint8_t>(((first << 4) & 0xF0) | ((second >> 8) & 0x0F));
        mBuffer[2] = static_cast<uint8_t>(second >> 0);
        mBuffer += 3;
    }

    inline void
    storeFloat(float data)
    {
        const uint32_t* ptr = reinterpret_cast<const uint32_t *>(&data);
        store32(*ptr);
    }

    inline void
    storeDouble(double data)
    {
        const uint64_t* ptr = reinterpret_cast<const uint64_t *>(&data);
        store64(*ptr);
    }

    inline void
    storeBuffer(const uint8_t* buffer, size_t length)
    {
        memcpy(mBuffer, buffer, length);
        mBuffer += length;
    }

    // explicit template instantiations are provided in serialize_impl.h
    template<typename T>
    inline void
    store(T data);

    template<typename T>
    inline void
    storeObject(const T& data);

    /**
     * Skip forward the given number of bytes.
     *
     * \param bytes
     *      Number of byte to skip forward.
     */
    inline void
    skip(size_t bytes)
    {
        mBuffer += bytes;
    }

    template<typename T>
    inline void
    skip()
    {
        mBuffer += sizeof(T);
    }

    inline uint8_t*
    getPointer()
    {
        return mBegin;
    }

    // get position of the buffer
    inline ptrdiff_t
    getPosition() const
    {
        return (mBuffer - mBegin);
    }

    inline uint8_t*
    getPointerToCurrentPosition()
    {
        return mBuffer;
    }

    template<typename T>
    inline Serialize&
    operator<<(const T& data)
    {
        store<T>(data);
        return *this;
    }

private:
    // disable assignment operator
    Serialize&
    operator=(const Serialize& other);

    uint8_t* mBuffer;
    uint8_t* mBegin;
};
// LCOV_EXCL_STOP
// LCOV_EXCL_END

// ----------------------------------------------------------------------------
/**
 * Deserialize the application data of a SPP/PUS packet.
 *
 * The read8(), read16() and read32() functions read the number of bits
 * from the current location and move the data pointer forward
 * correspondingly. The peek8(), peek16() and peek32() read a value
 * n bytes in front of the current location and *don't* move the
 * data pointer.
 *
 * \author Fabian Greif
 */
class Deserialize
{
public:
    explicit inline
    Deserialize(const uint8_t* inputBuffer) :
            mBuffer(inputBuffer), mBegin(inputBuffer)
    {
    }

    inline
    ~Deserialize()
    {
    }

    inline
    Deserialize(const Deserialize& other) :
            mBuffer(other.mBuffer), mBegin(other.mBegin)
    {
    }

    /**
     * Reset the read pointer to the beginning of the
     * originating buffer.
     */
    inline void
    reset()
    {
        mBuffer = mBegin;
    }

    inline uint8_t
    read8()
    {
        uint8_t value;
        value = mBuffer[0];
        mBuffer += 1;

        return value;
    }

    inline uint8_t
    peek8(size_t n) const
    {
        return mBuffer[n];
    }

    inline uint16_t
    read16()
    {
        uint16_t value = 0;
        value |= static_cast<uint16_t>(mBuffer[0]) << 8;
        value |= static_cast<uint16_t>(mBuffer[1]) << 0;
        mBuffer += 2;

        return value;
    }

    inline uint16_t
    peek16(size_t n) const
    {
        uint16_t value = 0;
        value |= static_cast<uint16_t>(mBuffer[n + 0]) << 8;
        value |= static_cast<uint16_t>(mBuffer[n + 1]) << 0;

        return value;
    }

    inline uint32_t
    read24()
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[0]) << 16;
        value |= static_cast<uint32_t>(mBuffer[1]) << 8;
        value |= static_cast<uint32_t>(mBuffer[2]) << 0;
        mBuffer += 3;

        return value;
    }

    inline uint32_t
    peek24(size_t n) const
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[n + 0]) << 16;
        value |= static_cast<uint32_t>(mBuffer[n + 1]) << 8;
        value |= static_cast<uint32_t>(mBuffer[n + 2]) << 0;

        return value;
    }

    inline uint32_t
    read32()
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[0]) << 24;
        value |= static_cast<uint32_t>(mBuffer[1]) << 16;
        value |= static_cast<uint32_t>(mBuffer[2]) << 8;
        value |= static_cast<uint32_t>(mBuffer[3]) << 0;
        mBuffer += 4;

        return value;
    }

    inline uint32_t
    peek32(size_t n) const
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[n + 0]) << 24;
        value |= static_cast<uint32_t>(mBuffer[n + 1]) << 16;
        value |= static_cast<uint32_t>(mBuffer[n + 2]) << 8;
        value |= static_cast<uint32_t>(mBuffer[n + 3]) << 0;

        return value;
    }

    inline uint64_t
    read64()
    {
        uint64_t value = 0;
        value |= static_cast<uint64_t>(mBuffer[0]) << 56;
        value |= static_cast<uint64_t>(mBuffer[1]) << 48;
        value |= static_cast<uint64_t>(mBuffer[2]) << 40;
        value |= static_cast<uint64_t>(mBuffer[3]) << 32;
        value |= static_cast<uint64_t>(mBuffer[4]) << 24;
        value |= static_cast<uint64_t>(mBuffer[5]) << 16;
        value |= static_cast<uint64_t>(mBuffer[6]) << 8;
        value |= static_cast<uint64_t>(mBuffer[7]) << 0;
        mBuffer += 8;

        return value;
    }

    inline uint64_t
    peek64(size_t n) const
    {
        uint64_t value = 0;
        value |= static_cast<uint64_t>(mBuffer[n + 0]) << 56;
        value |= static_cast<uint64_t>(mBuffer[n + 1]) << 48;
        value |= static_cast<uint64_t>(mBuffer[n + 2]) << 40;
        value |= static_cast<uint64_t>(mBuffer[n + 3]) << 32;
        value |= static_cast<uint64_t>(mBuffer[n + 4]) << 24;
        value |= static_cast<uint64_t>(mBuffer[n + 5]) << 16;
        value |= static_cast<uint64_t>(mBuffer[n + 6]) << 8;
        value |= static_cast<uint64_t>(mBuffer[n + 7]) << 0;

        return value;
    }

    /**
     * Read two 12 bit values from a three byte array.
     *
     * The 12 bit value are stored in the lower 12 bit of the two 16 bit
     * parameters.
     */
    inline void
    readPacked12(uint16_t& first, uint16_t& second)
    {
        first  = static_cast<uint32_t>(mBuffer[0]) << 4;
        first |= static_cast<uint32_t>(mBuffer[1]) >> 4;

        second  = static_cast<uint32_t>(mBuffer[1] & 0x0F) << 8;
        second |= static_cast<uint32_t>(mBuffer[2]);
        mBuffer += 3;
    }

    inline void
    peekPacked12(size_t n, uint16_t& first, uint16_t& second)
    {
        first  = static_cast<uint32_t>(mBuffer[n + 0]) << 4;
        first |= static_cast<uint32_t>(mBuffer[n + 1]) >> 4;

        second  = static_cast<uint32_t>(mBuffer[n + 1] & 0x0F) << 8;
        second |= static_cast<uint32_t>(mBuffer[n + 2]);
    }

    inline float
    readFloat(void)
    {
        float f;
        const uint32_t value = read32();

        memcpy(&f, &value, sizeof(f));
        return f;
    }

    inline float
    peekFloat(size_t n) const
    {
        float f;
        const uint32_t value = peek32(n);

        memcpy(&f, &value, sizeof(f));
        return f;
    }

    inline double
    readDouble(void)
    {
        double d;
        const uint64_t value = read64();

        memcpy(&d, &value, sizeof(d));
        return d;
    }

    inline double
    peekDouble(size_t n) const
    {
        double d;
        const uint64_t value = peek64(n);

        memcpy(&d, &value, sizeof(d));
        return d;
    }

    inline void
    readBuffer(uint8_t* buffer, size_t length)
    {
        memcpy(buffer, mBuffer, length);
        mBuffer += length;
    }

    inline void
    peekBuffer(uint8_t* buffer, size_t length)
    {
        memcpy(buffer, mBuffer, length);
    }

    template<typename T>
    inline T
    peek(size_t n) const;

    template<typename T>
    inline T
    read();

    /**
     * Skip forward the given number of bytes.
     *
     * \param bytes
     *      Number of byte to skip forward.
     */
    inline void
    skip(size_t bytes)
    {
        mBuffer += bytes;
    }

    template<typename T>
    inline void
    skip()
    {
        mBuffer += sizeof(T);
    }

    template<typename T>
    inline Deserialize&
    operator>>(T& data)
    {
        data = read<T>();
        return *this;
    }

    inline const uint8_t*
    getPointer() const
    {
        return mBegin;
    }

    /**
     * Get Pointer to the current location in the buffer.
     */
    inline const uint8_t*
    getPointerToCurrentPosition()
    {
        return mBuffer;
    }

    inline ptrdiff_t
    getPosition() const
    {
        return (mBuffer - mBegin);
    }

    template<typename T>
    inline T
    getPosition() const
    {
        return static_cast<T>(mBuffer - mBegin);
    }

private:
    // disable assignment operator
    Deserialize&
    operator=(const Deserialize& other);

    const uint8_t* mBuffer;
    const uint8_t* const mBegin;
};
}

#include "serialize_impl.h"

#endif // COBC_UTILS_SERIALIZE_H
