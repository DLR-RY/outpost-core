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
 * - 2015, Olaf Maibaum (DLR SC-SRV)
 */

#ifndef OUTPOST_UTILS_SERIALIZE_LITTLE_ENDIAN_H
#define OUTPOST_UTILS_SERIALIZE_LITTLE_ENDIAN_H

#include "serialize_little_endian_traits.h"

#include <outpost/base/slice.h>

#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace outpost
{
/**
 * Serialize data in little endian byte order.
 *
 * The supported types can be extended through overloads of the `SerializeLittleEndianTraits` class.
 */
class SerializeLittleEndian
{
public:
    explicit inline SerializeLittleEndian(outpost::Slice<uint8_t> array) :
        mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    explicit inline SerializeLittleEndian(uint8_t* outputBuffer) :
        mBuffer(outputBuffer), mBegin(outputBuffer)
    {
    }

    ~SerializeLittleEndian() = default;

    SerializeLittleEndian(const SerializeLittleEndian& other) = default;

    // disable assignment operator
    SerializeLittleEndian&
    operator=(const SerializeLittleEndian& other) = delete;

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
    static inline size_t
    getTypeSize()
    {
        return SerializeLittleEndianTraits<T>::size();
    }

    inline void
    store(outpost::Slice<const uint8_t> array)
    {
        size_t length = array.getNumberOfElements();
        memcpy(mBuffer, &array[0], length);
        mBuffer += length;
    }

    // explicit template instantiations are provided in serialize_impl.h
    template <typename T>
    inline void
    store(T data)
    {
        SerializeLittleEndianTraits<T>::store(mBuffer, data);
    }

    template <typename T>
    inline void
    storeObject(const T& data)
    {
        SerializeLittleEndianTraits<T>::store(mBuffer, data);
    }

    inline void
    store24(const uint32_t data)
    {
        mBuffer[2] = static_cast<uint8_t>(data >> 16);
        mBuffer[1] = static_cast<uint8_t>(data >> 8);
        mBuffer[0] = static_cast<uint8_t>(data >> 0);
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
        mBuffer[0] = static_cast<uint8_t>(first >> 0);
        mBuffer[1] = static_cast<uint8_t>(((first >> 4) & 0xF0) | (second & 0x0F));
        mBuffer[2] = static_cast<uint8_t>(second >> 4);
        mBuffer += 3;
    }

    inline void
    storeBuffer(const uint8_t* buffer, const size_t length)
    {
        memcpy(mBuffer, buffer, length);
        mBuffer += length;
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
        mBuffer += SerializeLittleEndianTraits<T>::size();
    }

    inline uint8_t*
    getPointer()
    {
        return mBegin;
    }

    inline uint8_t*
    getPointerToCurrentPosition()
    {
        return mBuffer;
    }

    // get position of the buffer
    template <typename T = ptrdiff_t>
    inline T
    getPosition() const
    {
        return static_cast<T>(mBuffer - mBegin);
    }

    inline Slice<uint8_t>
    asSlice()
    {
        return Slice<uint8_t>::unsafe(mBegin, getPosition());
    }

    template <typename T>
    inline SerializeLittleEndian&
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
class DeserializeLittleEndian
{
public:
    explicit inline DeserializeLittleEndian(const uint8_t* inputBuffer) :
        mBuffer(inputBuffer), mBegin(inputBuffer)
    {
    }

    explicit inline DeserializeLittleEndian(outpost::Slice<const uint8_t> array) :
        mBuffer(&array[0]), mBegin(&array[0])
    {
    }

    ~DeserializeLittleEndian() = default;

    DeserializeLittleEndian(const DeserializeLittleEndian& other) = default;

    // disable assignment operator
    DeserializeLittleEndian&
    operator=(const DeserializeLittleEndian& other) = delete;

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
        return SerializeLittleEndianTraits<T>::peek(mBuffer, n);
    }

    template <typename T>
    inline T
    read()
    {
        return SerializeLittleEndianTraits<T>::read(mBuffer);
    }

    template <typename U>
    inline void
    read(outpost::Slice<U> array)
    {
        for (size_t i = 0; i < array.getNumberOfElements(); ++i)
        {
            array[i] = SerializeLittleEndianTraits<U>::read(mBuffer);
        }
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
        first = static_cast<uint16_t>(mBuffer[0]);
        first |= static_cast<uint16_t>(mBuffer[1] & 0xF0) << 4;

        second = static_cast<uint16_t>(mBuffer[1] & 0x0F);
        second |= static_cast<uint16_t>(mBuffer[2]) << 4;
        mBuffer += 3;
    }

    inline void
    peekPacked12(const size_t n, uint16_t& first, uint16_t& second)
    {
        first = static_cast<uint16_t>(mBuffer[n + 0]);
        first |= static_cast<uint16_t>(mBuffer[n + 1] & 0xF0) << 4;

        second = static_cast<uint16_t>(mBuffer[n + 1] & 0x0F);
        second |= static_cast<uint16_t>(mBuffer[n + 2]) << 4;
    }

    inline uint32_t
    read24()
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[2]) << 16;
        value |= static_cast<uint32_t>(mBuffer[1]) << 8;
        value |= static_cast<uint32_t>(mBuffer[0]) << 0;
        mBuffer += 3;

        return value;
    }

    inline uint32_t
    peek24(const size_t n) const
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(mBuffer[n + 2]) << 16;
        value |= static_cast<uint32_t>(mBuffer[n + 1]) << 8;
        value |= static_cast<uint32_t>(mBuffer[n + 0]) << 0;

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
        mBuffer += SerializeLittleEndianTraits<T>::size();
    }

    template <typename T>
    inline DeserializeLittleEndian&
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
