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

#ifndef COBC_TRAITS_H
#define COBC_TRAITS_H

#include <stdint.h>

namespace cobc
{
    /**
     * Arithmetic Traits.
     *
     * Traits give types more information e.g. the number of bytes needed
     * to represent the given type.
     *
     * \author    Fabian Greif
     */
    template<typename T>
    struct Traits
    {
    };

    template<>
    struct Traits<uint8_t>
    {
        typedef int8_t SignedType;
        typedef uint8_t UnsignedType;

        static const int width = 1;
    };

    template<>
    struct Traits<int8_t>
    {
        typedef int8_t SignedType;
        typedef uint8_t UnsignedType;

        static const int width = 1;
    };

    template<>
    struct Traits<uint16_t>
    {
        typedef int16_t SignedType;
        typedef uint16_t UnsignedType;

        static const int width = 2;
    };

    template<>
    struct Traits<int16_t>
    {
        typedef int16_t SignedType;
        typedef uint16_t UnsignedType;

        static const int width = 2;
    };

    template<>
    struct Traits<uint32_t>
    {
        typedef int32_t SignedType;
        typedef uint32_t UnsignedType;

        static const int width = 4;
    };

    template<>
    struct Traits<int32_t>
    {
        typedef int32_t SignedType;
        typedef uint32_t UnsignedType;

        static const int width = 4;
    };

    template<>
    struct Traits<uint64_t>
    {
        typedef int64_t SignedType;
        typedef uint64_t UnsignedType;

        static const int width = 8;
    };

    template<>
    struct Traits<int64_t>
    {
        typedef int64_t SignedType;
        typedef uint64_t UnsignedType;

        static const int width = 8;
    };
}

#endif // COBC_TRAITS_H
