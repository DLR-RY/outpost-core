/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_RTOS_FREERTOS_TRAITS_H
#define OUTPOST_RTOS_FREERTOS_TRAITS_H

#include <stdint.h>

namespace outpost
{
namespace rtos
{
/**
 * Access the signed and unsigned types of the different types used
 * in FreeRTOS.
 *
 * \author  Fabian Greif
 */
template <typename T>
struct Traits
{
};

template <>
struct Traits<int16_t>
{
    typedef int16_t SignedType;
    typedef uint16_t UnsignedType;
};

template <>
struct Traits<uint16_t>
{
    typedef int16_t SignedType;
    typedef uint16_t UnsignedType;
};

template <>
struct Traits<int32_t>
{
    typedef int32_t SignedType;
    typedef uint32_t UnsignedType;
};

template <>
struct Traits<uint32_t>
{
    typedef int32_t SignedType;
    typedef uint32_t UnsignedType;
};
template <>
struct Traits<int64_t>
{
    typedef int64_t SignedType;
    typedef uint64_t UnsignedType;
};

template <>
struct Traits<uint64_t>
{
    typedef int64_t SignedType;
    typedef uint64_t UnsignedType;
};
}  // namespace rtos
}  // namespace outpost

#endif
