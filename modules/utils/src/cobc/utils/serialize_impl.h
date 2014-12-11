/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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

#ifndef COBC_UTILS_SERIALIZE_IMPL_H
#define COBC_UTILS_SERIALIZE_IMPL_H

#include "serialize.h"

namespace cobc
{

// ----------------------------------------------------------------------------
// Store
template <>
inline void
Serialize::store<uint8_t>(uint8_t data)
{
    store8(data);
}

template <>
inline void
Serialize::store<uint16_t>(uint16_t data)
{
    store16(data);
}

template <>
inline void
Serialize::store<uint32_t>(uint32_t data)
{
    store32(data);
}

template <>
inline void
Serialize::store<uint64_t>(uint64_t data)
{
    store64(data);
}

template <>
inline void
Serialize::store<int8_t>(int8_t data)
{
    store8(static_cast<uint8_t>(data));
}

template <>
inline void
Serialize::store<int16_t>(int16_t data)
{
    store16(static_cast<uint16_t>(data));
}

template <>
inline void
Serialize::store<int32_t>(int32_t data)
{
    store32(static_cast<uint32_t>(data));
}

template <>
inline void
Serialize::store<int64_t>(int64_t data)
{
    store64(static_cast<uint64_t>(data));
}

template <>
inline void
Serialize::store<float>(float data)
{
    storeFloat(data);
}

template <>
inline void
Serialize::store<double>(double data)
{
    storeDouble(data);
}

// ----------------------------------------------------------------------------
// Peek
template <>
inline uint8_t
Deserialize::peek<uint8_t>(size_t n) const
{
    uint8_t value = peek8(n);
    return value;
}

template <>
inline uint16_t
Deserialize::peek<uint16_t>(size_t n) const
{
    uint16_t value = peek16(n);
    return value;
}

template <>
inline uint32_t
Deserialize::peek<uint32_t>(size_t n) const
{
    uint32_t value = peek32(n);
    return value;
}

template <>
inline uint64_t
Deserialize::peek<uint64_t>(size_t n) const
{
    uint64_t value = peek64(n);
    return value;
}

template <>
inline int8_t
Deserialize::peek<int8_t>(size_t n) const
{
    int8_t value = static_cast<int8_t>(peek8(n));
    return value;
}

template <>
inline int16_t
Deserialize::peek<int16_t>(size_t n) const
{
    int16_t value = static_cast<int16_t>(peek16(n));
    return value;
}

template <>
inline int32_t
Deserialize::peek<int32_t>(size_t n) const
{
    int32_t value = static_cast<int32_t>(peek32(n));
    return value;
}

template <>
inline int64_t
Deserialize::peek<int64_t>(size_t n) const
{
    int64_t value = static_cast<int64_t>(peek64(n));
    return value;
}

template <>
inline float
Deserialize::peek<float>(size_t n) const
{
    float value = peekFloat(n);
    return value;
}

template <>
inline double
Deserialize::peek<double>(size_t n) const
{
    double value = peekDouble(n);
    return value;
}



// ----------------------------------------------------------------------------
// Read
template <>
inline uint8_t
Deserialize::read<uint8_t>()
{
    uint8_t value = read8();
    return value;
}

template <>
inline uint16_t
Deserialize::read<uint16_t>()
{
    uint16_t value = read16();
    return value;
}

template <>
inline uint32_t
Deserialize::read<uint32_t>()
{
    uint32_t value = read32();
    return value;
}

template <>
inline uint64_t
Deserialize::read<uint64_t>()
{
    uint64_t value = read64();
    return value;
}

template <>
inline int8_t
Deserialize::read<int8_t>()
{
    int8_t value = static_cast<int8_t>(read8());
    return value;
}

template <>
inline int16_t
Deserialize::read<int16_t>()
{
    int16_t value = static_cast<int16_t>(read16());
    return value;
}

template <>
inline int32_t
Deserialize::read<int32_t>()
{
    int32_t value = static_cast<int32_t>(read32());
    return value;
}

template <>
inline int64_t
Deserialize::read<int64_t>()
{
    int64_t value = static_cast<int64_t>(read64());
    return value;
}

template <>
inline float
Deserialize::read<float>()
{
    float value = readFloat();
    return value;
}


template <>
inline double
Deserialize::read<double>()
{
    double value = readDouble();
    return value;
}

}

#endif // COBC_UTILS_SERIALIZE_IMPL_H
