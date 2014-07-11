/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
    return peek8(n);
}

template <>
inline uint16_t
Deserialize::peek<uint16_t>(size_t n) const
{
    return peek16(n);
}

template <>
inline uint32_t
Deserialize::peek<uint32_t>(size_t n) const
{
    return peek32(n);
}

template <>
inline uint64_t
Deserialize::peek<uint64_t>(size_t n) const
{
    return peek64(n);
}

template <>
inline float
Deserialize::peek<float>(size_t n) const
{
    return peekFloat(n);
}

template <>
inline double
Deserialize::peek<double>(size_t n) const
{
    return peekDouble(n);
}

// ----------------------------------------------------------------------------
// Read
template <>
inline uint8_t
Deserialize::read<uint8_t>()
{
    return read8();
}

template <>
inline uint16_t
Deserialize::read<uint16_t>()
{
    return read16();
}

template <>
inline uint32_t
Deserialize::read<uint32_t>()
{
    return read32();
}

template <>
inline uint64_t
Deserialize::read<uint64_t>()
{
    return read64();
}

template <>
inline float
Deserialize::read<float>()
{
    return readFloat();
}


template <>
inline double
Deserialize::read<double>()
{
    return readDouble();
}

}

#endif // COBC_UTILS_SERIALIZE_IMPL_H
