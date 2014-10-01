/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_BIT_ACCESS_IMPL_H
#define COBC_UTILS_BIT_ACCESS_IMPL_H

#include "bit_access.h"

namespace cobc
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
cobc::BitAccess::get(const T& data)
{
//    static_assert(offset <= (ValueType<T>::width - 1), "Access out of the range of the register width!");

    bool value = data & (1 << offset);
    return value;
}

template <typename T, int start, int end>
T
cobc::BitAccess::get(const T& data)
{
//    static_assert(start < ValueType<T>::width, "Access out of the range of the register width!");
//    static_assert(end < ValueType<T>::width, "Access out of the range of the register width!");
//    static_assert(start < end, "Invalid bitfield definition! 'start' must be smaller than 'end'");

    const int width = end - start + 1;
    T mask = createMask<T>(width);
    T value = (data >> start) & mask;

    return value;
}

template <typename T, int offset>
void
cobc::BitAccess::set(T& data, bool value)
{
//    static_assert(offset <= (ValueType<T>::width - 1), "Access out of the range of the register width!");

    T reg = data;
    T mask = 1 << offset;

    reg &= ~mask;
    reg |= value ? mask : 0;

    data = reg;
}

template <typename T, int start, int end>
void
cobc::BitAccess::set(T& data, T value)
{
//    static_assert(start < ValueType<T>::width, "Access out of the range of the register width!");
//    static_assert(end < ValueType<T>::width, "Access out of the range of the register width!");
//    static_assert(start < end, "Invalid bitfield definition! 'start' must be smaller than 'end'");

    T reg = data;
    const int width = end - start + 1;
    T mask = createMask<T>(width) << start;

    reg &= ~mask;
    reg |= (value << start) & mask;

    data = reg;
}

template <typename T>
T
cobc::BitAccess::createMask(int length)
{
    T mask = 0;

    int position = 0;
    while (position < length)
    {
        mask <<= 1;
        mask |= 1;
        position++;
    }

    return mask;
}

#endif // COBC_UTILS_BIT_ACCESS_IMPL_H
