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

#ifndef COBC_UTILS_BITFIELD_H
#define COBC_UTILS_BITFIELD_H

#include <stdint.h>

namespace cobc
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
    // Disable compiler generated functions
    Bitfield();

    ~Bitfield();

    Bitfield(const Bitfield& other);

    Bitfield&
    operator=(const Bitfield& other);

    static const int numberOfBitsPerByte = 8;
};

}

#include "bitfield_impl.h"

#endif // COBC_UTILS_BITFIELD_H
