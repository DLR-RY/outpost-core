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

#ifndef OUTPOST_UTILS_BITFIELD_IMPL_H
#define OUTPOST_UTILS_BITFIELD_IMPL_H

#include "bit_access.h"
#include "bitfield.h"
#include "bitorder.h"
#include "serialize.h"

template <int offset>
bool
outpost::Bitfield::read(const uint8_t* byteArray)
{
    const size_t index = offset / 8;
    const int offsetByte = offset & 7;

    bool value = BitAccess::get<uint8_t, 7 - offsetByte>(byteArray[index]);
    return value;
}

template <int start, int end>
uint16_t
outpost::Bitfield::read(const uint8_t* byteArray)
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
outpost::Bitfield::write(uint8_t* byteArray, bool value)
{
    const int index = offset / numberOfBitsPerByte;
    const int bitpos = offset & 7;

    // clear and set bitfield
    uint8_t mask = static_cast<uint8_t>(1 << (7 - bitpos));
    byteArray[index] &= static_cast<uint8_t>(~mask);
    byteArray[index] |= static_cast<uint8_t>(value << (7 - bitpos));
}

template <int start, int end>
void
outpost::Bitfield::write(uint8_t* byteArray, uint16_t value)
{
    static_assert(start < end, "Invalid bitfield definition! 'start' must be smaller than 'end'");

    // get the byte index, if odd, round it down, then get position in 16-bit word
    unsigned int index = start / numberOfBitsPerByte;
    uint16_t pos = start & 0x7;

    const uint16_t numberOfBits = (end - start) + 1;

    // pos of bits to set (in 16-bit word)
    int bitpos = 16 - (pos + numberOfBits);

    // bitmask for number of bits to set
    uint16_t mask = static_cast<uint16_t>(((1 << numberOfBits) - 1) << bitpos);

    // shift value to desired position
    value = static_cast<uint16_t>(value << bitpos);

    // make sure to load the bytes in big endian order
    uint16_t word = (static_cast<uint16_t>(byteArray[index]) << numberOfBitsPerByte)
                    | static_cast<uint16_t>(byteArray[index + 1]);

    // clear bit field
    word = static_cast<uint16_t>(word & ~mask);

    // set the corresponding bits
    word = static_cast<uint16_t>(word | (mask & value));

    // make sure to store the bytes in big-endian order
    // most significant first
    byteArray[index] = static_cast<uint8_t>(word >> numberOfBitsPerByte);
    byteArray[index + 1] = static_cast<uint8_t>(word);
}

#endif
