/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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

#ifndef COBC_UTILS_BITFIELD_IMPL_H
#define COBC_UTILS_BITFIELD_IMPL_H

#include "bitfield.h"

#include "bit_access.h"
#include "serialize.h"
#include "bitorder.h"

#include "helper.h"

template <int offset>
bool
cobc::Bitfield::read(const uint8_t* byteArray)
{
    const size_t index = offset / 8;
    const int offsetByte = offset & 7;

    bool value = BitAccess::get<uint8_t, 7 - offsetByte>(byteArray[index]);
    return value;
}

template <int start, int end>
uint16_t
cobc::Bitfield::read(const uint8_t* byteArray)
{
    static_assert(start < end, "Invalid bitfield definition! 'start' must be smaller than 'end'");

    // Load the bytes in big endian order
    Deserialize stream(&byteArray[BitorderMsb0ToLsb0<uint16_t, start, end>::byteIndex]);
    uint16_t word = stream.read<uint16_t>();

    uint16_t value = BitAccess::get<uint16_t,
                                    BitorderMsb0ToLsb0<uint16_t, start, end>::start,
                                    BitorderMsb0ToLsb0<uint16_t, start, end>::end>(word);

    return value;
}

template <int offset>
void
cobc::Bitfield::write(uint8_t* byteArray, bool value)
{
    const int index = offset / 8;
    const int bitpos = offset & 7;

    // clear and set bitfield
    byteArray[index] &= static_cast<uint8_t>(~(1 << (7 - bitpos)));
    byteArray[index] |= static_cast<uint8_t>(value << (7 - bitpos));
}

template <int start, int end>
void
cobc::Bitfield::write(uint8_t* byteArray, uint16_t value)
{
    static_assert(start < end, "Invalid bitfield definition! 'start' must be smaller than 'end'");

    // get the byte index, if odd, round it down, then get position in 16-bit word
    unsigned int index = start / 8;
    uint16_t pos = start & 0x7;

    const uint16_t numberOfBits = end - start + 1;

    // pos of bits to set (in 16-bit word)
    int bitpos    = 16 - (pos + numberOfBits);

    // bitmask for number of bits to set
    uint16_t mask = static_cast<uint16_t>(((1 << numberOfBits) - 1) << bitpos);

    // shift value to desired position
    value         = static_cast<uint16_t>(value << bitpos);

    // make sure to load the bytes in big endian order
    uint16_t word = (static_cast<uint16_t>(byteArray[index]) << 8) |
                     static_cast<uint16_t>(byteArray[index + 1]);

    // clear bit field
    word = static_cast<uint16_t>(word & ~mask);

    // set the corresponding bits
    word = static_cast<uint16_t>(word | (mask & value));

    // make sure to store the bytes in big-endian order
    // most significant first
    byteArray[index]     = static_cast<uint8_t>(word >> 8);
    byteArray[index + 1] = static_cast<uint8_t>(word);
}

#endif // COBC_UTILS_BITFIELD_IMPL_H
