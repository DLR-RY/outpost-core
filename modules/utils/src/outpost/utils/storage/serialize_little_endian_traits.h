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
 * - 2014, Annika Ofenloch (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_SERIALIZE_LITTLE_ENDIAN_TRAITS_H
#define OUTPOST_UTILS_SERIALIZE_LITTLE_ENDIAN_TRAITS_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace outpost
{
template <typename T>
struct SerializeLittleEndianTraits
{
    static void
    store(uint8_t*& buffer, T value);

    static T
    read(const uint8_t*& buffer);

    static T
    peek(const uint8_t* const& buffer, size_t offset);

    static constexpr size_t
    size();
};

template <>
struct SerializeLittleEndianTraits<uint8_t>
{
    static inline void
    store(uint8_t*& buffer, uint8_t data)
    {
        buffer[0] = data;
        buffer += 1;
    }

    static inline uint8_t
    read(const uint8_t*& buffer)
    {
        uint8_t value;
        value = buffer[0];
        buffer += 1;

        return value;
    }

    static inline uint8_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        return buffer[n];
    }

    static inline constexpr size_t
    size()
    {
        return 1;
    }
};

template <>
struct SerializeLittleEndianTraits<uint16_t>
{
    static inline void
    store(uint8_t*& buffer, uint16_t data)
    {
        buffer[1] = static_cast<uint8_t>(data >> 8);
        buffer[0] = static_cast<uint8_t>(data >> 0);
        buffer += 2;
    }

    static inline uint16_t
    read(const uint8_t*& buffer)
    {
        uint16_t value = 0;
        value |= static_cast<uint16_t>(buffer[1]) << 8;
        value |= static_cast<uint16_t>(buffer[0]) << 0;
        buffer += 2;

        return value;
    }

    static inline uint16_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        uint16_t value = 0;
        value |= static_cast<uint16_t>(buffer[n + 1]) << 8;
        value |= static_cast<uint16_t>(buffer[n + 0]) << 0;

        return value;
    }

    static inline constexpr size_t
    size()
    {
        return 2;
    }
};

template <>
struct SerializeLittleEndianTraits<uint32_t>
{
    static inline void
    store(uint8_t*& buffer, uint32_t data)
    {
        buffer[3] = static_cast<uint8_t>(data >> 24);
        buffer[2] = static_cast<uint8_t>(data >> 16);
        buffer[1] = static_cast<uint8_t>(data >> 8);
        buffer[0] = static_cast<uint8_t>(data >> 0);
        buffer += 4;
    }

    static inline uint32_t
    read(const uint8_t*& buffer)
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(buffer[3]) << 24;
        value |= static_cast<uint32_t>(buffer[2]) << 16;
        value |= static_cast<uint32_t>(buffer[1]) << 8;
        value |= static_cast<uint32_t>(buffer[0]) << 0;
        buffer += 4;

        return value;
    }

    static inline uint32_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        uint32_t value = 0;
        value |= static_cast<uint32_t>(buffer[n + 3]) << 24;
        value |= static_cast<uint32_t>(buffer[n + 2]) << 16;
        value |= static_cast<uint32_t>(buffer[n + 1]) << 8;
        value |= static_cast<uint32_t>(buffer[n + 0]) << 0;

        return value;
    }

    static inline constexpr size_t
    size()
    {
        return 4;
    }
};

template <>
struct SerializeLittleEndianTraits<uint64_t>
{
    static inline void
    store(uint8_t*& buffer, uint64_t data)
    {
        buffer[7] = static_cast<uint8_t>(data >> 56);
        buffer[6] = static_cast<uint8_t>(data >> 48);
        buffer[5] = static_cast<uint8_t>(data >> 40);
        buffer[4] = static_cast<uint8_t>(data >> 32);
        buffer[3] = static_cast<uint8_t>(data >> 24);
        buffer[2] = static_cast<uint8_t>(data >> 16);
        buffer[1] = static_cast<uint8_t>(data >> 8);
        buffer[0] = static_cast<uint8_t>(data >> 0);
        buffer += 8;
    }

    static inline uint64_t
    read(const uint8_t*& buffer)
    {
        uint64_t value = 0;
        value |= static_cast<uint64_t>(buffer[7]) << 56;
        value |= static_cast<uint64_t>(buffer[6]) << 48;
        value |= static_cast<uint64_t>(buffer[5]) << 40;
        value |= static_cast<uint64_t>(buffer[4]) << 32;
        value |= static_cast<uint64_t>(buffer[3]) << 24;
        value |= static_cast<uint64_t>(buffer[2]) << 16;
        value |= static_cast<uint64_t>(buffer[1]) << 8;
        value |= static_cast<uint64_t>(buffer[0]) << 0;
        buffer += 8;

        return value;
    }

    static inline uint64_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        uint64_t value = 0;
        value |= static_cast<uint64_t>(buffer[n + 7]) << 56;
        value |= static_cast<uint64_t>(buffer[n + 6]) << 48;
        value |= static_cast<uint64_t>(buffer[n + 5]) << 40;
        value |= static_cast<uint64_t>(buffer[n + 4]) << 32;
        value |= static_cast<uint64_t>(buffer[n + 3]) << 24;
        value |= static_cast<uint64_t>(buffer[n + 2]) << 16;
        value |= static_cast<uint64_t>(buffer[n + 1]) << 8;
        value |= static_cast<uint64_t>(buffer[n + 0]) << 0;

        return value;
    }

    static inline constexpr size_t
    size()
    {
        return 8;
    }
};

template <>
struct SerializeLittleEndianTraits<char>
{
    static inline void
    store(uint8_t*& buffer, char data)
    {
        SerializeLittleEndianTraits<uint8_t>::store(buffer, data);
    }
};

template <>
struct SerializeLittleEndianTraits<int8_t>
{
    static inline void
    store(uint8_t*& buffer, int8_t data)
    {
        SerializeLittleEndianTraits<uint8_t>::store(buffer, static_cast<uint8_t>(data));
    }

    static inline int8_t
    read(const uint8_t*& buffer)
    {
        int8_t value = static_cast<int8_t>(SerializeLittleEndianTraits<uint8_t>::read(buffer));
        return value;
    }

    static inline int8_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        int8_t value = static_cast<int8_t>(SerializeLittleEndianTraits<uint8_t>::peek(buffer, n));
        return value;
    }

    static inline constexpr size_t
    size()
    {
        return 1;
    }
};

template <>
struct SerializeLittleEndianTraits<int16_t>
{
    static inline void
    store(uint8_t*& buffer, int16_t data)
    {
        SerializeLittleEndianTraits<uint16_t>::store(buffer, static_cast<uint16_t>(data));
    }

    static inline int16_t
    read(const uint8_t*& buffer)
    {
        int16_t value = static_cast<int16_t>(SerializeLittleEndianTraits<uint16_t>::read(buffer));
        return value;
    }

    static inline int16_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        int16_t value =
                static_cast<int16_t>(SerializeLittleEndianTraits<uint16_t>::peek(buffer, n));
        return value;
    }

    static inline constexpr size_t
    size()
    {
        return 2;
    }
};

template <>
struct SerializeLittleEndianTraits<int32_t>
{
    static inline void
    store(uint8_t*& buffer, int32_t data)
    {
        SerializeLittleEndianTraits<uint32_t>::store(buffer, static_cast<uint32_t>(data));
    }

    static inline int32_t
    read(const uint8_t*& buffer)
    {
        int32_t value = static_cast<int32_t>(SerializeLittleEndianTraits<uint32_t>::read(buffer));
        return value;
    }

    static inline int32_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        int32_t value =
                static_cast<int32_t>(SerializeLittleEndianTraits<uint32_t>::peek(buffer, n));
        return value;
    }

    static inline constexpr size_t
    size()
    {
        return 4;
    }
};

template <>
struct SerializeLittleEndianTraits<int64_t>
{
    static inline void
    store(uint8_t*& buffer, int64_t data)
    {
        SerializeLittleEndianTraits<uint64_t>::store(buffer, static_cast<uint64_t>(data));
    }

    static inline int64_t
    read(const uint8_t*& buffer)
    {
        int64_t value = static_cast<int64_t>(SerializeLittleEndianTraits<uint64_t>::read(buffer));
        return value;
    }

    static inline int64_t
    peek(const uint8_t* const& buffer, size_t n)
    {
        int64_t value =
                static_cast<int64_t>(SerializeLittleEndianTraits<uint64_t>::peek(buffer, n));
        return value;
    }

    static inline constexpr size_t
    size()
    {
        return 8;
    }
};

template <>
struct SerializeLittleEndianTraits<float>
{
    static inline void
    store(uint8_t*& buffer, float data)
    {
        const uint32_t* ptr = reinterpret_cast<const uint32_t*>(&data);
        SerializeLittleEndianTraits<uint32_t>::store(buffer, *ptr);
    }

    static inline float
    read(const uint8_t*& buffer)
    {
        float f;
        const uint32_t value = SerializeLittleEndianTraits<uint32_t>::read(buffer);

        memcpy(&f, &value, sizeof(f));
        return f;
    }

    static inline float
    peek(const uint8_t* const& buffer, size_t n)
    {
        float f;
        const uint32_t value = SerializeLittleEndianTraits<uint32_t>::peek(buffer, n);

        memcpy(&f, &value, sizeof(f));
        return f;
    }

    static inline constexpr size_t
    size()
    {
        return 4;
    }
};

template <>
struct SerializeLittleEndianTraits<double>
{
    static inline void
    store(uint8_t*& buffer, double data)
    {
        const uint64_t* ptr = reinterpret_cast<const uint64_t*>(&data);
        SerializeLittleEndianTraits<uint64_t>::store(buffer, *ptr);
    }

    static inline double
    read(const uint8_t*& buffer)
    {
        double d;
        const uint64_t value = SerializeLittleEndianTraits<uint64_t>::read(buffer);

        memcpy(&d, &value, sizeof(d));
        return d;
    }

    static inline double
    peek(const uint8_t* const& buffer, size_t n)
    {
        double d;
        const uint64_t value = SerializeLittleEndianTraits<uint64_t>::peek(buffer, n);

        memcpy(&d, &value, sizeof(d));
        return d;
    }

    static inline constexpr size_t
    size()
    {
        return 8;
    }
};

}  // namespace outpost

#endif
