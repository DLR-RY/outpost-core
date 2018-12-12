/*
 * Copyright (c) 2014-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 * - 2018, Jan Malburg (DLR RY-AVS)
 */

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
    static_assert(start >= 0 && end >= 0,
                  "Invalid bitfield definition! 'start' and 'end' must be at least 0");

    constexpr uint32_t numberOfBits = (end - start) + 1;

    static_assert(numberOfBits <= 16, "Bitfield::read can read at most 16 bits");

    // Based on the number of affected bytes another specialized version of read will be called.
    return Access<start, end, affectedBytes(start, end)>::read(byteArray);
}

template <int start, int end>
uint16_t
outpost::Bitfield::Access<start, end, 1>::read(const uint8_t* byteArray)
{
    // Load the bytes in big endian order
    Deserialize stream(&byteArray[BitorderMsb0ToLsb0<uint8_t, start, end>::byteIndex]);
    uint8_t byte = stream.read<uint8_t>();

    uint16_t value = BitAccess::get<uint8_t,
                                    BitorderMsb0ToLsb0<uint8_t, start, end>::start,
                                    BitorderMsb0ToLsb0<uint8_t, start, end>::end>(byte);

    return value;
}

template <int start, int end>
uint16_t
outpost::Bitfield::Access<start, end, 2>::read(const uint8_t* byteArray)
{
    // Load the bytes in big endian order
    Deserialize stream(&byteArray[BitorderMsb0ToLsb0<uint16_t, start, end>::byteIndex]);
    uint16_t word = stream.read<uint16_t>();

    uint16_t value = BitAccess::get<uint16_t,
                                    BitorderMsb0ToLsb0<uint16_t, start, end>::start,
                                    BitorderMsb0ToLsb0<uint16_t, start, end>::end>(word);

    return value;
}

template <int start, int end>
uint16_t
outpost::Bitfield::Access<start, end, 3>::read(const uint8_t* byteArray)
{
    // put the data into a temporary variable to prevent access behind the array
    uint8_t buffer[4] = {0, 0, 0, 0};
    memcpy(&buffer, byteArray, 3);

    Deserialize stream(buffer);
    uint32_t tmp = stream.read<uint32_t>();

    uint16_t value = BitAccess::get<uint32_t,
                                    BitorderMsb0ToLsb0<uint32_t, start, end>::start,
                                    BitorderMsb0ToLsb0<uint32_t, start, end>::end>(tmp);

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
    static_assert(start >= 0 && end >= 0,
                  "Invalid bitfield definition! 'start' and 'end' must be at least 0");

    constexpr uint32_t numberOfBits = (end - start) + 1;

    static_assert(numberOfBits <= 16, "Bitfield::read can read at most 16 bits");

    // Based on the number of affected bytes another specialized version of write will be called.
    Access<start, end, affectedBytes(start, end)>::write(byteArray, value);
}

template <int start, int end>
void
outpost::Bitfield::Access<start, end, 1>::write(uint8_t* byteArray, uint16_t value)
{
    constexpr uint32_t numberOfBits = (end - start) + 1;
    constexpr uint16_t pos = start & 0x7;
    constexpr uint16_t index = start / numberOfBitsPerByte;

    // pos of bits to set (in byte)
    constexpr uint16_t bitpos = 8 - (pos + numberOfBits);

    // bitmask for number of bits to set
    const uint8_t mask = ((1 << numberOfBits) - 1) << bitpos;

    // shift value to desired position
    uint8_t toSet = static_cast<uint8_t>(value << bitpos);

    // get the original value
    uint8_t byte = byteArray[index];

    // clear bit field
    byte = (byte & ~mask);

    // set the corresponding bits
    byte = (byte | (mask & toSet));

    byteArray[index] = byte;
}

template <int start, int end>
void
outpost::Bitfield::Access<start, end, 2>::write(uint8_t* byteArray, uint16_t value)
{
    constexpr uint32_t numberOfBits = (end - start) + 1;
    constexpr uint16_t pos = start & 0x7;
    constexpr uint16_t index = start / numberOfBitsPerByte;

    // pos of bits to set (in 16-bit word)
    constexpr uint16_t bitpos = 16 - (pos + numberOfBits);

    // bitmask for number of bits to set
    const uint16_t mask = ((1 << numberOfBits) - 1) << bitpos;

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

template <int start, int end>
void
outpost::Bitfield::Access<start, end, 3>::write(uint8_t* byteArray, uint16_t value)
{
    constexpr uint16_t pos = start & 0x7;
    constexpr uint16_t index = start / numberOfBitsPerByte;

    constexpr uint16_t bitpos_low = 8 - (pos);
    constexpr uint16_t bitpos_high = 1 + (end & 0x7);

    constexpr uint8_t mask_low = static_cast<uint8_t>(0xff << bitpos_low);
    constexpr uint8_t mask_high = static_cast<uint8_t>(0xff >> bitpos_high);

    // low in sense of index means low byte most significant
    // we must push the start position and we must additionally push the most significant byte
    // into the less significant byte.
    uint8_t byte_low = static_cast<uint8_t>((value >> (pos + 8)) & 0xff);

    // we only must adjust to an unaligned start
    uint8_t byte_middle = static_cast<uint8_t>((value >> pos) & 0xff);

    // high byte must be the lower on as at least 8 bits are in the middle byte
    // then just push the bit on the correct position
    uint8_t byte_high = static_cast<uint8_t>((value & 0xff) << (8 - bitpos_high));

    // first zero out the part of the value we don't want
    uint8_t toSet_low = byte_low & ~mask_low;
    uint8_t toSet_high = byte_high & ~mask_high;

    // then combine with existing contents of the array
    toSet_low |= (byteArray[index] & mask_low);
    toSet_high |= (byteArray[index + 2] & mask_high);

    byteArray[index] = toSet_low;
    byteArray[index + 1] =
            byte_middle;  // there is no mask needed for middle byte as it will be written whole
    byteArray[index + 2] = toSet_high;
}

#endif
