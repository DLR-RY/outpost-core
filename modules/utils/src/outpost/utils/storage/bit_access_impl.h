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
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_BIT_ACCESS_IMPL_H
#define OUTPOST_UTILS_BIT_ACCESS_IMPL_H

#include "bit_access.h"

namespace outpost
{
// Valid register types
template <typename T>
class ValueType
{
};

template <>
class ValueType<uint8_t>
{
public:
    static const int width = 8;
};

template <>
class ValueType<uint16_t>
{
public:
    static const int width = 16;
};

template <>
class ValueType<uint32_t>
{
public:
    static const int width = 32;
};
}

template <typename T, int offset>
bool
outpost::BitAccess::get(const T& data)
{
    static_assert(offset <= (ValueType<T>::width - 1), "Access out of the range of the register width!");

    bool value = data & (1 << offset);
    return value;
}

template <typename T, int end, int start>
T
outpost::BitAccess::get(const T& data)
{
    static_assert(start < ValueType<T>::width, "Access out of the range of the register width!");
    static_assert(end < ValueType<T>::width, "Access out of the range of the register width!");
    static_assert(start <= end, "Invalid bitfield definition! 'start' must be smaller than 'end'");

    const int width = (end - start) + 1;
    T mask = getMask<T>(width);
    T value = static_cast<T>((data >> start) & mask);

    return value;
}

template <typename T, int offset>
void
outpost::BitAccess::set(T& data, bool value)
{
    static_assert(offset <= (ValueType<T>::width - 1), "Access out of the range of the register width!");

    T reg = data;
    T mask = 1 << offset;

    reg &= ~mask;
    reg |= value ? mask : 0;

    data = reg;
}

template <typename T, int end, int start>
void
outpost::BitAccess::set(T& data, T value)
{
    static_assert(start < ValueType<T>::width, "Access out of the range of the register width!");
    static_assert(end < ValueType<T>::width, "Access out of the range of the register width!");
    static_assert(start <= end, "Invalid bitfield definition! 'start' must be smaller than 'end'");

    T reg = data;
    const int width = (end - start) + 1;
    T mask = getMask<T>(width) << start;

    reg &= ~mask;
    reg |= (value << start) & mask;

    data = reg;
}

template <typename T>
T
outpost::BitAccess::getMask(size_t length)
{
    T mask = (static_cast<T>(1) << length) - 1;
    return mask;
}

namespace outpost
{
// ----------------------------------------------------------------------------
// Template specialization for common cases

template <>
inline uint32_t
BitAccess::get<uint32_t, 31, 0>(const uint32_t& data)
{
    return data;
}

template <>
inline uint32_t
BitAccess::get<uint32_t, 31, 24>(const uint32_t& data)
{
    const uint32_t mask = 0xFF000000;
    uint32_t value = (data & mask) >> 24;
    return value;
}

template <>
inline void
BitAccess::set<uint32_t, 31, 0>(uint32_t& data, uint32_t value)
{
    data = value;
}

template <>
inline void
BitAccess::set<uint32_t, 31, 24>(uint32_t& data, uint32_t value)
{
    static const uint32_t maskValue = 0xFF000000;
    static const uint32_t maskClear = 0x00FFFFFF;
    uint32_t reg = data;

    data = (reg & maskClear) | ((value << 24) & maskValue);
}
}

#endif
