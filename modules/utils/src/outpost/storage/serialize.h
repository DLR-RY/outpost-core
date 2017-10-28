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
 * - 2015, Janosch Reinking (DLR RY-AVS)
 * - 2015, Olaf Maibaum (DLR SC-SRV)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_SERIALIZE_H
#define OUTPOST_UTILS_SERIALIZE_H

#include <stddef.h>
#include <string.h>
#include <stdint.h>

#include <outpost/container/bounded_array.h>
#include <outpost/container/fixed_size_array.h>

#include "serialize_traits.h"
#include "serialize_storage_traits.h"

namespace outpost
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
    Serialize(outpost::BoundedArray<uint8_t> array) :
            mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    template <size_t N>
    explicit inline
    Serialize(outpost::FixedSizeArrayView<uint8_t, N> array) :
            mBuffer(&array[0]), mBegin(&array[0])
    {
    }

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

    template<typename T>
    static inline size_t
    getTypeSize()
    {
        return SerializeBigEndianTraits<T>::size();
    }

    template<typename T>
    inline void
    store(T data)
    {
        SerializeBigEndianTraits<T>::store(mBuffer, data);
    }

    template<typename T>
    inline void
    storeObject(const T& data)
    {
        SerializeBigEndianTraits<T>::store(mBuffer, data);
    }

    inline void
    store24(const uint32_t data)
    {
        mBuffer[0] = static_cast<uint8_t>(data >> 16);
        mBuffer[1] = static_cast<uint8_t>(data >> 8);
        mBuffer[2] = static_cast<uint8_t>(data >> 0);
        mBuffer += 3;
    }

    /**
     * Store two 12 bit values in a three byte array.
     *
     * The 12 bit value need to be stored in the lower 12 bit of the
     * two 16 bit parameters.
     */
    inline void
    storePacked12(const uint16_t first, const uint16_t second)
    {
        mBuffer[0] = static_cast<uint8_t>(first >> 4);
        mBuffer[1] = static_cast<uint8_t>(((first << 4) & 0xF0) | ((second >> 8) & 0x0F));
        mBuffer[2] = static_cast<uint8_t>(second >> 0);
        mBuffer += 3;
    }

    inline void
    storeBuffer(const uint8_t* buffer, const size_t length)
    {
        memcpy(mBuffer, buffer, length);
        mBuffer += length;
    }

    inline void
    store(outpost::BoundedArray<const uint8_t> array)
    {
        size_t length = array.getNumberOfElements();
        memcpy(mBuffer, &array[0], length);
        mBuffer += length;
    }

    template <size_t N>
    inline void
    store(outpost::FixedSizeArrayView<const uint8_t, N> array)
    {
        memcpy(mBuffer, &array[0], N);
        mBuffer += N;
    }

    template <typename U>
    inline void
    store(outpost::BoundedArray<const U> array)
    {
        for (size_t i = 0; i < array.getNumberOfElements(); ++i)
        {
            store<U>(array[i]);
        }
    }

    template <typename U>
    inline void
    store(outpost::BoundedArray<U> array)
    {
        for (size_t i = 0; i < array.getNumberOfElements(); ++i)
        {
            store<U>(array[i]);
        }
    }

    /**
     * Skip forward the given number of bytes.
     *
     * \param bytes
     *      Number of byte to skip forward.
     */
    inline void
    skip(const size_t bytes)
    {
        mBuffer += bytes;
    }

    template<typename T>
    inline void
    skip()
    {
        mBuffer += SerializeBigEndianTraits<T>::size();
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
 * The read<uint8_t>(), read<uint16_t>() and read<uint32_t>() functions read the number of bits
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

    explicit inline
    Deserialize(outpost::BoundedArray<const uint8_t> array) :
            mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    template <size_t N>
    explicit inline
    Deserialize(outpost::FixedSizeArrayView<const uint8_t, N> array) :
            mBuffer(&array[0]), mBegin(&array[0])
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

    template<typename T>
    inline T
    peek(const size_t n) const
    {
        return SerializeBigEndianTraits<T>::peek(mBuffer, n);
    }

    template<typename T>
    inline T
    read()
    {
        return SerializeBigEndianTraits<T>::read(mBuffer);
    }

    inline void
    read(outpost::BoundedArray<uint8_t> array)
    {
        size_t length = array.getNumberOfElements();
        memcpy(&array[0], mBuffer, length);
        mBuffer += length;
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
    peek24(const size_t n) const
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[n + 0]) << 16;
        value |= static_cast<uint32_t>(mBuffer[n + 1]) << 8;
        value |= static_cast<uint32_t>(mBuffer[n + 2]) << 0;

        return value;
    }

    inline void
    readBuffer(uint8_t* buffer, const size_t length)
    {
        memcpy(buffer, mBuffer, length);
        mBuffer += length;
    }

    inline void
    peekBuffer(uint8_t* buffer, const size_t length)
    {
        memcpy(buffer, mBuffer, length);
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
    peekPacked12(const size_t n, uint16_t& first, uint16_t& second)
    {
        first  = static_cast<uint32_t>(mBuffer[n + 0]) << 4;
        first |= static_cast<uint32_t>(mBuffer[n + 1]) >> 4;

        second  = static_cast<uint32_t>(mBuffer[n + 1] & 0x0F) << 8;
        second |= static_cast<uint32_t>(mBuffer[n + 2]);
    }

    /**
     * Skip forward the given number of bytes.
     *
     * \param bytes
     *      Number of byte to skip forward.
     */
    inline void
    skip(const size_t bytes)
    {
        mBuffer += bytes;
    }

    template<typename T>
    inline void
    skip()
    {
        mBuffer += SerializeBigEndianTraits<T>::size();
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
    getPointerToCurrentPosition() const
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

#endif
