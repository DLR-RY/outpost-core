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
 * - 2018       Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_BITFIELD_H
#define OUTPOST_UTILS_BITFIELD_H

#include <stdint.h>

namespace outpost
{
/**
 * Set and get multiple bits from a byte array.
 *
 * The data in the bytes is assumed to be in big endian order (higher byte
 * first). Uses MSB 0 bit ordering (bit 0 is the MSB).
 *
 * \author  Fabian Greif
 */
class Bitfield
{
public:
    // Disable compiler generated functions
    Bitfield() = delete;

    ~Bitfield() = delete;

    Bitfield(const Bitfield& other) = delete;

    Bitfield&
    operator=(const Bitfield& other) = delete;

    /**
     * Read a single bit from a byte stream.
     *
     * \tparam  offset
     *      Offset of the bit to read.
     *
     * \param byteArray
     *      Pointer to the byte array to access.
     *
     * \return  Value of the bit.
     */
    template <int offset>
    static bool
    read(const uint8_t* byteArray);

    /**
     * Read multiple bits from a byte stream.
     *
     * Reads up to 16 bit. The bits must fit into consecutive bytes in the
     * byte array! E.g. it is possible to do read<0, 15>() but not
     * read<1, 16>(). The first requires to read the first two bytes, the later
     * to read the first three bytes.
     *
     * \tparam  start
     *      First bit of the bitfield
     * \tparam  end
     *      Last bit of the bifield. E.g. start=0, end=15 reads the full
     *      first two bytes.
     *
     * \param byteArray
     *      Pointer to the byte array to access.
     *
     * \return  Value contained in the bits given.
     */
    template <int start, int end>
    static uint16_t
    read(const uint8_t* byteArray);

    /**
     * Write a single bit.
     *
     * \tparam  offset
     *      Offset of the bit to read.
     *
     * \param byteArray
     *      Pointer to the byte array to write.
     * \param value
     *      Value to write to the array.
     */
    template <int offset>
    static void
    write(uint8_t* byteArray, bool value);

    /**
     * Write multiple bits.
     *
     * \tparam  start
     *      First bit of the bitfield
     * \tparam  end
     *      Last bit of the bifield. E.g. start=0, end=15 reads the full
     *      first two bytes.
     *
     * \param byteArray
     *      Pointer to the byte array to access.
     * \param value
     *      Value to write to the array. The bits defined through \c start
     *      and \c end are first cleared and then overwritten with the value
     *      defined through this parameter.
     */
    template <int start, int end>
    static void
    write(uint8_t* byteArray, uint16_t value);

private:
    static constexpr int numberOfBitsPerByte = 8;

    static constexpr inline int
    affectedBytes(int start, int end)
    {
        return ((end / numberOfBitsPerByte) - (start / numberOfBitsPerByte)) + 1;
    }

    /**
     * Base Version no instantiation, only specializations are used
     */
    template <int start, int end, int affectedBytes>
    struct Reader
    {
        inline static uint16_t
        read(const uint8_t* byteArray);
    };

    /**
     * The specialization when a single byte is affected
     */
    template <int start, int end>
    struct Reader<start, end, 1>
    {
        inline static uint16_t
        read(const uint8_t* byteArray);
    };

    /**
     * The specialization when two bytes are affected
     */
    template <int start, int end>
    struct Reader<start, end, 2>
    {
        inline static uint16_t
        read(const uint8_t* byteArray);
    };

    /**
     * The specialization when three bytes are affected
     */
    template <int start, int end>
    struct Reader<start, end, 3>
    {
        inline static uint16_t
        read(const uint8_t* byteArray);
    };
};

}  // namespace outpost

#include "bitfield_impl.h"

#endif
