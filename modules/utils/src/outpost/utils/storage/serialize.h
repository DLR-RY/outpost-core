/*
 * Copyright (c) 2014-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2018, Fabian Greif (DLR RY-AVS)
 * - 2015, Janosch Reinking (DLR RY-AVS)
 * - 2015, 2018, Olaf Maibaum (DLR SC-SRV)
 */

#ifndef OUTPOST_UTILS_SERIALIZE_H
#define OUTPOST_UTILS_SERIALIZE_H

#include "serialize_storage_traits.h"
#include "serialize_traits.h"
#include "variable_width_integer.h"

#include <outpost/base/slice.h>
#include <outpost/utils/container/fixed_size_array.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <type_traits>

namespace outpost
{
/**
 * Serialize data as big-endian into raw byte arrays.
 *
 * The supported types can be extended through overloads of the `SerializeBigEndianTraits` class.
 */
class Serialize
{
public:
    explicit inline Serialize(outpost::Slice<uint8_t> array) : mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    template <size_t N>
    explicit inline Serialize(outpost::FixedSizeArrayView<uint8_t, N> array) :
        mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    explicit inline Serialize(uint8_t* outputBuffer) : mBuffer(outputBuffer), mBegin(outputBuffer)
    {
    }

    ~Serialize() = default;

    Serialize(const Serialize& other) = default;

    // disable assignment operator
    Serialize&
    operator=(const Serialize& other) = delete;

    /**
     * Reset the read pointer to the beginning of the
     * originating buffer.
     */
    inline void
    reset()
    {
        mBuffer = mBegin;
    }

    template <typename T>
    static inline constexpr size_t
    getTypeSize()
    {
        // Removes the const and/or volatile qualification from the given type
        // to avoid problems with e.g. SerializeBigEndianTraits<T> and
        // SerializeBigEndianTraits<const T> requiring different traits.
        return SerializeBigEndianTraits<typename std::remove_cv<T>::type>::size();
    }

    template <typename U>
    static inline size_t
    getTypeSize(outpost::Slice<U> array)
    {
        return SerializeBigEndianTraits<typename std::remove_cv<U>::type>::size()
               * array.getNumberOfElements();
    }

    template <typename T>
    inline void
    store(T data)
    {
        SerializeBigEndianTraits<T>::store(mBuffer, data);
    }

    template <typename T>
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
        if (buffer && length)
        {
            memcpy(mBuffer, buffer, length);
            mBuffer += length;
        }
    }

    inline void
    store(outpost::Slice<const uint8_t> array)
    {
        size_t length = array.getNumberOfElements();
        if (length)
        {
            memcpy(mBuffer, array.getDataPointer(), length);
            mBuffer += length;
        }
    }

    template <size_t N>
    inline void
    store(outpost::FixedSizeArrayView<const uint8_t, N> array)
    {
        if (N)
        {
            memcpy(mBuffer, array.getDataPointer(), N);
            mBuffer += N;
        }
    }

    template <typename U>
    inline void
    store(outpost::Slice<const U> array)
    {
        for (size_t i = 0; i < array.getNumberOfElements(); ++i)
        {
            store<U>(array[i]);
        }
    }

    template <typename U>
    inline void
    store(outpost::Slice<U> array)
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

    template <typename T>
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
    template <typename T = ptrdiff_t>
    inline T
    getPosition() const
    {
        return static_cast<T>(mBuffer - mBegin);
    }

    inline uint8_t*
    getPointerToCurrentPosition()
    {
        return mBuffer;
    }

    inline Slice<uint8_t>
    asSlice()
    {
        return Slice<uint8_t>::unsafe(mBegin, getPosition());
    }

    template <typename T>
    inline Serialize&
    operator<<(const T& data)
    {
        store<T>(data);
        return *this;
    }

private:
    uint8_t* mBuffer;
    uint8_t* mBegin;
};

/**
 * Deserialize big-endian data from raw byte arrays.
 *
 * The `read<T>()` functions read the number of bits from the current location and move the
 * data pointer forward correspondingly. The peek<T>() read a value n bytes in front of the
 * current location and *don't* move the data pointer.
 *
 * The supported types can be extended through overloads of the `SerializeBigEndianTraits` class.
 */
class Deserialize
{
public:
    explicit inline Deserialize(const uint8_t* inputBuffer) :
        mBuffer(inputBuffer), mBegin(inputBuffer)
    {
    }

    explicit inline Deserialize(outpost::Slice<const uint8_t> array) :
        mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    template <size_t N>
    explicit inline Deserialize(outpost::FixedSizeArrayView<const uint8_t, N> array) :
        mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    ~Deserialize() = default;

    Deserialize(const Deserialize& other) = default;

    // disable assignment operator
    Deserialize&
    operator=(const Deserialize& other) = delete;

    /**
     * Reset the read pointer to the beginning of the
     * originating buffer.
     */
    inline void
    reset()
    {
        mBuffer = mBegin;
    }

    template <typename T>
    inline T
    peek(const size_t n) const
    {
        return SerializeBigEndianTraits<T>::peek(mBuffer, n);
    }

    template <typename T>
    inline T
    read()
    {
        return SerializeBigEndianTraits<T>::read(mBuffer);
    }

    inline void
    read(outpost::Slice<uint8_t> array)
    {
        size_t length = array.getNumberOfElements();
        memcpy(&array[0], mBuffer, length);
        mBuffer += length;
    }

    template <typename U>
    inline void
    read(outpost::Slice<U> array)
    {
        for (size_t i = 0; i < array.getNumberOfElements(); ++i)
        {
            array[i] = SerializeBigEndianTraits<U>::read(mBuffer);
        }
    }

    inline uint32_t
    readUnsigned24()
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[0]) << 16;
        value |= static_cast<uint32_t>(mBuffer[1]) << 8;
        value |= static_cast<uint32_t>(mBuffer[2]) << 0;
        mBuffer += 3;

        return value;
    }

    inline int32_t
    readSigned24()
    {
        int32_t value = 0;
        value |= static_cast<int32_t>(mBuffer[0]) << 16;
        value |= static_cast<int32_t>(mBuffer[1]) << 8;
        value |= static_cast<int32_t>(mBuffer[2]) << 0;
        mBuffer += 3;

        // is negative if the MSB is 1 -> set bits 31-24 to 1
        if (0x00800000 & value)
        {
            value |= 0xFF000000;
        }
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
        first = static_cast<uint32_t>(mBuffer[0]) << 4;
        first |= static_cast<uint32_t>(mBuffer[1]) >> 4;

        second = static_cast<uint32_t>(mBuffer[1] & 0x0F) << 8;
        second |= static_cast<uint32_t>(mBuffer[2]);
        mBuffer += 3;
    }

    inline void
    peekPacked12(const size_t n, uint16_t& first, uint16_t& second)
    {
        first = static_cast<uint32_t>(mBuffer[n + 0]) << 4;
        first |= static_cast<uint32_t>(mBuffer[n + 1]) >> 4;

        second = static_cast<uint32_t>(mBuffer[n + 1] & 0x0F) << 8;
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

    template <typename T>
    inline void
    skip()
    {
        mBuffer += SerializeBigEndianTraits<T>::size();
    }

    template <typename T>
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

    template <typename T = ptrdiff_t>
    inline T
    getPosition() const
    {
        return static_cast<T>(mBuffer - mBegin);
    }

    inline Slice<const uint8_t>
    asSlice() const
    {
        return Slice<const uint8_t>::unsafe(mBegin, getPosition());
    }

private:
    const uint8_t* mBuffer;
    const uint8_t* const mBegin;
};
}  // namespace outpost

#endif
