/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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

#ifndef COBC_UTILS_LIMITS_H
#define COBC_UTILS_LIMITS_H

namespace cobc
{

template <typename T>
struct Limits
{
};

template <>
struct Limits<int8_t>
{
    static const int8_t min = -128;
    static const int8_t max = 127;
};

template <>
struct Limits<uint8_t>
{
    static const uint8_t min = 0;
    static const uint8_t max = 255;
};

template <>
struct Limits<int16_t>
{
    static const int16_t min = -32768;
    static const int16_t max = 32767;
};

template <>
struct Limits<uint16_t>
{
    static const uint16_t min = 0;
    static const uint16_t max = 65535;
};

template <>
struct Limits<int32_t>
{
    static const int32_t min = -2147483647L - 1;
    static const int32_t max = 2147483647;
};

template <>
struct Limits<uint32_t>
{
    static const uint32_t min = 0;
    static const uint32_t max = 4294967295UL;
};

template <>
struct Limits<int64_t>
{
    static const int64_t min = -9223372036854775807LL - 1LL;
    static const int64_t max = 9223372036854775807LL;
};

template <>
struct Limits<uint64_t>
{
    static const uint64_t min = 0;
    static const uint64_t max = 18446744073709551615ULL;
};

}

#endif // COBC_UTILS_LIMITS_H
