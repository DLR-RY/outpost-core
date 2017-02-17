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
 * - 2014, Annika Ofenloch (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_SERIALIZE_IMPL_H
#define COBC_UTILS_SERIALIZE_IMPL_H

#include "serialize.h"

namespace outpost
{

// ----------------------------------------------------------------------------
// Store
template <>
inline void
Serialize::store<uint8_t>(uint8_t data)
{
    mBuffer[0] = data;
    mBuffer += 1;
}

template <>
inline void
Serialize::store<uint16_t>(uint16_t data)
{
    mBuffer[0] = static_cast<uint8_t>(data >> 8);
    mBuffer[1] = static_cast<uint8_t>(data >> 0);
    mBuffer += 2;
}

template <>
inline void
Serialize::store<uint32_t>(uint32_t data)
{
    mBuffer[0] = static_cast<uint8_t>(data >> 24);
    mBuffer[1] = static_cast<uint8_t>(data >> 16);
    mBuffer[2] = static_cast<uint8_t>(data >> 8);
    mBuffer[3] = static_cast<uint8_t>(data >> 0);
    mBuffer += 4;
}

template <>
inline void
Serialize::store<uint64_t>(uint64_t data)
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

template <>
inline void
Serialize::store<char>(char data)
{
    store<uint8_t>(data);
}

template <>
inline void
Serialize::store<int8_t>(int8_t data)
{
    store<uint8_t>(static_cast<uint8_t>(data));
}

template <>
inline void
Serialize::store<int16_t>(int16_t data)
{
    store<uint16_t>(static_cast<uint16_t>(data));
}

template <>
inline void
Serialize::store<int32_t>(int32_t data)
{
    store<uint32_t>(static_cast<uint32_t>(data));
}

template <>
inline void
Serialize::store<int64_t>(int64_t data)
{
    store<uint64_t>(static_cast<uint64_t>(data));
}

template <>
inline void
Serialize::store<float>(float data)
{
    const uint32_t* ptr = reinterpret_cast<const uint32_t *>(&data);
    store<uint32_t>(*ptr);
}

template <>
inline void
Serialize::store<double>(double data)
{
    const uint64_t* ptr = reinterpret_cast<const uint64_t *>(&data);
    store<uint64_t>(*ptr);
}

template <>
inline void
Serialize::store<outpost::BoundedArray<const uint8_t> >(outpost::BoundedArray<const uint8_t> array)
{
    size_t length = array.getNumberOfElements();
    memcpy(mBuffer, &array[0], length);
    mBuffer += length;
}

// ----------------------------------------------------------------------------
// Peek
template <>
inline uint8_t
Deserialize::peek<uint8_t>(size_t n) const
{
    return mBuffer[n];
}

template <>
inline uint16_t
Deserialize::peek<uint16_t>(size_t n) const
{
    uint16_t value = 0;
    value |= static_cast<uint16_t>(mBuffer[n + 0]) << 8;
    value |= static_cast<uint16_t>(mBuffer[n + 1]) << 0;

    return value;
}

template <>
inline uint32_t
Deserialize::peek<uint32_t>(size_t n) const
{
    uint32_t value = 0;
    value |= static_cast<uint32_t>(mBuffer[n + 0]) << 24;
    value |= static_cast<uint32_t>(mBuffer[n + 1]) << 16;
    value |= static_cast<uint32_t>(mBuffer[n + 2]) << 8;
    value |= static_cast<uint32_t>(mBuffer[n + 3]) << 0;

    return value;
}

template <>
inline uint64_t
Deserialize::peek<uint64_t>(size_t n) const
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

template <>
inline int8_t
Deserialize::peek<int8_t>(size_t n) const
{
    int8_t value = static_cast<int8_t>(peek<uint8_t>(n));
    return value;
}

template <>
inline int16_t
Deserialize::peek<int16_t>(size_t n) const
{
    int16_t value = static_cast<int16_t>(peek<uint16_t>(n));
    return value;
}

template <>
inline int32_t
Deserialize::peek<int32_t>(size_t n) const
{
    int32_t value = static_cast<int32_t>(peek<uint32_t>(n));
    return value;
}

template <>
inline int64_t
Deserialize::peek<int64_t>(size_t n) const
{
    int64_t value = static_cast<int64_t>(peek<uint64_t>(n));
    return value;
}

template <>
inline float
Deserialize::peek<float>(size_t n) const
{
    float f;
    const uint32_t value = peek<uint32_t>(n);

    memcpy(&f, &value, sizeof(f));
    return f;
}

template <>
inline double
Deserialize::peek<double>(size_t n) const
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
Deserialize::read<uint8_t>()
{
    uint8_t value;
    value = mBuffer[0];
    mBuffer += 1;

    return value;
}

template <>
inline uint16_t
Deserialize::read<uint16_t>()
{
    uint16_t value = 0;
    value |= static_cast<uint16_t>(mBuffer[0]) << 8;
    value |= static_cast<uint16_t>(mBuffer[1]) << 0;
    mBuffer += 2;

    return value;
}

template <>
inline uint32_t
Deserialize::read<uint32_t>()
{
    uint32_t value = 0;
    value |= static_cast<uint32_t>(mBuffer[0]) << 24;
    value |= static_cast<uint32_t>(mBuffer[1]) << 16;
    value |= static_cast<uint32_t>(mBuffer[2]) << 8;
    value |= static_cast<uint32_t>(mBuffer[3]) << 0;
    mBuffer += 4;

    return value;
}

template <>
inline uint64_t
Deserialize::read<uint64_t>()
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

template <>
inline int8_t
Deserialize::read<int8_t>()
{
    int8_t value = static_cast<int8_t>(read<uint8_t>());
    return value;
}

template <>
inline int16_t
Deserialize::read<int16_t>()
{
    int16_t value = static_cast<int16_t>(read<uint16_t>());
    return value;
}

template <>
inline int32_t
Deserialize::read<int32_t>()
{
    int32_t value = static_cast<int32_t>(read<uint32_t>());
    return value;
}

template <>
inline int64_t
Deserialize::read<int64_t>()
{
    int64_t value = static_cast<int64_t>(read<uint64_t>());
    return value;
}

template <>
inline float
Deserialize::read<float>()
{
    float f;
    const uint32_t value = read<uint32_t>();

    memcpy(&f, &value, sizeof(f));
    return f;
}


template <>
inline double
Deserialize::read<double>()
{
    double d;
    const uint64_t value = read<uint64_t>();

    memcpy(&d, &value, sizeof(d));
    return d;
}

inline void
Deserialize::read(outpost::BoundedArray<uint8_t> array)
{
    size_t length = array.getNumberOfElements();
    memcpy(&array[0], mBuffer, length);
    mBuffer += length;
}

}

#endif // COBC_UTILS_SERIALIZE_IMPL_H
