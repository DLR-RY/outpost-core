/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
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

#ifndef COBC_UTILS_SERIALIZE_LITTLE_ENDIAN_IMPL_H
#define COBC_UTILS_SERIALIZE_LITTLE_ENDIAN_IMPL_H

#include "serialize_little_endian.h"

namespace cobc
{

// ----------------------------------------------------------------------------
// Store
template <>
inline void
SerializeLittleEndian::store<uint8_t>(uint8_t data)
{
    mBuffer[0] = data;
    mBuffer += 1;
}

template <>
inline void
SerializeLittleEndian::store<uint16_t>(uint16_t data)
{
    mBuffer[1] = static_cast<uint8_t>(data >> 8);
    mBuffer[0] = static_cast<uint8_t>(data >> 0);
    mBuffer += 2;
}

template <>
inline void
SerializeLittleEndian::store<uint32_t>(uint32_t data)
{
    mBuffer[3] = static_cast<uint8_t>(data >> 24);
    mBuffer[2] = static_cast<uint8_t>(data >> 16);
    mBuffer[1] = static_cast<uint8_t>(data >> 8);
    mBuffer[0] = static_cast<uint8_t>(data >> 0);
    mBuffer += 4;
}

template <>
inline void
SerializeLittleEndian::store<uint64_t>(uint64_t data)
{
    mBuffer[7] = static_cast<uint8_t>(data >> 56);
    mBuffer[6] = static_cast<uint8_t>(data >> 48);
    mBuffer[5] = static_cast<uint8_t>(data >> 40);
    mBuffer[4] = static_cast<uint8_t>(data >> 32);
    mBuffer[3] = static_cast<uint8_t>(data >> 24);
    mBuffer[2] = static_cast<uint8_t>(data >> 16);
    mBuffer[1] = static_cast<uint8_t>(data >> 8);
    mBuffer[0] = static_cast<uint8_t>(data >> 0);
    mBuffer += 8;
}

template <>
inline void
SerializeLittleEndian::store<int8_t>(int8_t data)
{
    store<uint8_t>(static_cast<uint8_t>(data));
}

template <>
inline void
SerializeLittleEndian::store<int16_t>(int16_t data)
{
    store<uint16_t>(static_cast<uint16_t>(data));
}

template <>
inline void
SerializeLittleEndian::store<int32_t>(int32_t data)
{
    store<uint32_t>(static_cast<uint32_t>(data));
}

template <>
inline void
SerializeLittleEndian::store<int64_t>(int64_t data)
{
    store<uint64_t>(static_cast<uint64_t>(data));
}

template <>
inline void
SerializeLittleEndian::store<float>(float data)
{
    const uint32_t* ptr = reinterpret_cast<const uint32_t *>(&data);
    store<uint32_t>(*ptr);
}

template <>
inline void
SerializeLittleEndian::store<double>(double data)
{
    const uint64_t* ptr = reinterpret_cast<const uint64_t *>(&data);
    store<uint64_t>(*ptr);
}

template <>
inline void
SerializeLittleEndian::store<cobc::BoundedArray<const uint8_t> >(cobc::BoundedArray<const uint8_t> array)
{
    size_t length = array.getNumberOfElements();
    memcpy(mBuffer, &array[0], length);
    mBuffer += length;
}

// ----------------------------------------------------------------------------
// Peek
template <>
inline uint8_t
DeserializeLittleEndian::peek<uint8_t>(size_t n) const
{
    return mBuffer[n];
}

template <>
inline uint16_t
DeserializeLittleEndian::peek<uint16_t>(size_t n) const
{
    uint16_t value = 0;
    value |= static_cast<uint16_t>(mBuffer[n + 1]) << 8;
    value |= static_cast<uint16_t>(mBuffer[n + 0]) << 0;

    return value;
}

template <>
inline uint32_t
DeserializeLittleEndian::peek<uint32_t>(size_t n) const
{
    uint32_t value = 0;
    value |= static_cast<uint32_t>(mBuffer[n + 3]) << 24;
    value |= static_cast<uint32_t>(mBuffer[n + 2]) << 16;
    value |= static_cast<uint32_t>(mBuffer[n + 1]) << 8;
    value |= static_cast<uint32_t>(mBuffer[n + 0]) << 0;

    return value;
}

template <>
inline uint64_t
DeserializeLittleEndian::peek<uint64_t>(size_t n) const
{
    uint64_t value = 0;
    value |= static_cast<uint64_t>(mBuffer[n + 7]) << 56;
    value |= static_cast<uint64_t>(mBuffer[n + 6]) << 48;
    value |= static_cast<uint64_t>(mBuffer[n + 5]) << 40;
    value |= static_cast<uint64_t>(mBuffer[n + 4]) << 32;
    value |= static_cast<uint64_t>(mBuffer[n + 3]) << 24;
    value |= static_cast<uint64_t>(mBuffer[n + 2]) << 16;
    value |= static_cast<uint64_t>(mBuffer[n + 1]) << 8;
    value |= static_cast<uint64_t>(mBuffer[n + 0]) << 0;

    return value;
}

template <>
inline int8_t
DeserializeLittleEndian::peek<int8_t>(size_t n) const
{
    int8_t value = static_cast<int8_t>(peek<uint8_t>(n));
    return value;
}

template <>
inline int16_t
DeserializeLittleEndian::peek<int16_t>(size_t n) const
{
    int16_t value = static_cast<int16_t>(peek<uint16_t>(n));
    return value;
}

template <>
inline int32_t
DeserializeLittleEndian::peek<int32_t>(size_t n) const
{
    int32_t value = static_cast<int32_t>(peek<uint32_t>(n));
    return value;
}

template <>
inline int64_t
DeserializeLittleEndian::peek<int64_t>(size_t n) const
{
    int64_t value = static_cast<int64_t>(peek<uint64_t>(n));
    return value;
}

template <>
inline float
DeserializeLittleEndian::peek<float>(size_t n) const
{
    float f;
    const uint32_t value = peek<uint32_t>(n);

    memcpy(&f, &value, sizeof(f));
    return f;
}

template <>
inline double
DeserializeLittleEndian::peek<double>(size_t n) const
{
    double d;
    const uint64_t value = peek<uint64_t>(n);

    memcpy(&d, &value, sizeof(d));
    return d;
}

// ----------------------------------------------------------------------------
// Read
template <>
inline uint8_t
DeserializeLittleEndian::read<uint8_t>()
{
    uint8_t value;
    value = mBuffer[0];
    mBuffer += 1;

    return value;
}

template <>
inline uint16_t
DeserializeLittleEndian::read<uint16_t>()
{
    uint16_t value = 0;
    value |= static_cast<uint16_t>(mBuffer[1]) << 8;
    value |= static_cast<uint16_t>(mBuffer[0]) << 0;
    mBuffer += 2;

    return value;
}

template <>
inline uint32_t
DeserializeLittleEndian::read<uint32_t>()
{
    uint32_t value = 0;
    value |= static_cast<uint32_t>(mBuffer[3]) << 24;
    value |= static_cast<uint32_t>(mBuffer[2]) << 16;
    value |= static_cast<uint32_t>(mBuffer[1]) << 8;
    value |= static_cast<uint32_t>(mBuffer[0]) << 0;
    mBuffer += 4;

    return value;
}

template <>
inline uint64_t
DeserializeLittleEndian::read<uint64_t>()
{
    uint64_t value = 0;
    value |= static_cast<uint64_t>(mBuffer[7]) << 56;
    value |= static_cast<uint64_t>(mBuffer[6]) << 48;
    value |= static_cast<uint64_t>(mBuffer[5]) << 40;
    value |= static_cast<uint64_t>(mBuffer[4]) << 32;
    value |= static_cast<uint64_t>(mBuffer[3]) << 24;
    value |= static_cast<uint64_t>(mBuffer[2]) << 16;
    value |= static_cast<uint64_t>(mBuffer[1]) << 8;
    value |= static_cast<uint64_t>(mBuffer[0]) << 0;
    mBuffer += 8;

    return value;
}


template <>
inline int8_t
DeserializeLittleEndian::read<int8_t>()
{
    int8_t value = static_cast<int8_t>(read<uint8_t>());
    return value;
}

template <>
inline int16_t
DeserializeLittleEndian::read<int16_t>()
{
    int16_t value = static_cast<int16_t>(read<uint16_t>());
    return value;
}

template <>
inline int32_t
DeserializeLittleEndian::read<int32_t>()
{
    int32_t value = static_cast<int32_t>(read<uint32_t>());
    return value;
}

template <>
inline int64_t
DeserializeLittleEndian::read<int64_t>()
{
    int64_t value = static_cast<int64_t>(read<uint64_t>());
    return value;
}

template <>
inline float
DeserializeLittleEndian::read<float>()
{
    float f;
    const uint32_t value = read<uint32_t>();

    memcpy(&f, &value, sizeof(f));
    return f;
}


template <>
inline double
DeserializeLittleEndian::read<double>()
{
    double d;
    const uint64_t value = read<uint64_t>();

    memcpy(&d, &value, sizeof(d));
    return d;
}

}

#endif
