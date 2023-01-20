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
 * - 2021, Jan Sommer (DLR SC-SRV)
 */

#ifndef OUTPOST_UTILS_BITFIELD_IMPL_H
#define OUTPOST_UTILS_BITFIELD_IMPL_H

#include "bit_access.h"
#include "bitfield.h"
#include "bitorder.h"
#include "serialize.h"

template <typename T>
static constexpr const T
difference(const T& a, const T& b)
{
    return (a > b ? a - b : b - a);
}

template <unsigned int offset>
bool
outpost::Bitfield::read(const uint8_t* byteArray)
{
    const size_t index = offset / 8;
    const int offsetByte = offset & 7;

    bool value = BitAccess::get<uint8_t, 7 - offsetByte>(byteArray[index]);
    return value;
}

template <unsigned int start, unsigned int end>
outpost::Bitfield::UInteger<start, end>
outpost::Bitfield::read(const uint8_t* byteArray)
{
    static_assert(start < end, "Invalid bitfield definition! 'start' must be smaller than 'end'");
    static_assert(start >= 0 && end >= 0,
                  "Invalid bitfield definition! 'start' and 'end' must be at least 0");

    constexpr uint32_t numberOfBits = (end - start) + 1;

    static_assert(numberOfBits <= 64, "Bitfield::read can read at most 64 bits");

    using ReturnType = UInteger<start, end>;

    constexpr uint32_t index = start / numberOfBitsPerByte;  // Index of the first affected bytes
    constexpr std::size_t numAffectedBytes = affectedBytes(start, end);

    // Difference between number of affected bytes and the return type in bits.
    // Needed for shift calculation as both can be equal or one can be larger
    // depending on the byte boundaries within the read bits
    constexpr std::size_t bitOffset =
            (difference(sizeof(ReturnType), numAffectedBytes)) * numberOfBitsPerByte;

    // First bit to read in the first affected Byte (i.e. start % 8)
    constexpr uint16_t pos = start & 0x7;
    // Bit position of the first read bit in MSB order
    constexpr uint32_t bitpos =
            numAffectedBytes * numberOfBitsPerByte + bitOffset - (pos + numberOfBits);
    // Bit mask which marks the valid bits in the return value
    constexpr ReturnType mask = getMask<ReturnType>(numberOfBits);
    constexpr uint32_t maxByte =
            numAffectedBytes - 1;  // The index offset of the last byte to write to
    constexpr uint32_t lastShift = bitpos - bitOffset;

    ReturnType value = 0;
    for (unsigned i = 0; i < maxByte; ++i)
    {
        const uint32_t shift = bitOffset + (maxByte - i) * numberOfBitsPerByte;
        // Use ReturnType to store byte, so that shift operation with large offsets work
        const ReturnType byte = byteArray[i + index] & (mask >> (shift - bitpos));
        value |= (byte << (shift - bitpos));
    }
    const uint8_t byte = byteArray[maxByte + index] & (mask << lastShift);
    value |= (byte >> lastShift);

    return value;
}

template <unsigned int offset>
void
outpost::Bitfield::write(uint8_t* byteArray, bool value)
{
    const unsigned int index = offset / numberOfBitsPerByte;
    // Bit to write in the affected Byte (i.e. offset % 8)
    const unsigned int bitpos = offset & 7;

    // clear and set bitfield
    uint8_t mask = static_cast<uint8_t>(1 << (7 - bitpos));
    byteArray[index] &= static_cast<uint8_t>(~mask);
    byteArray[index] |= static_cast<uint8_t>(value << (7 - bitpos));
}

template <unsigned int start, unsigned int end>
void
outpost::Bitfield::write(uint8_t* byteArray, UInteger<start, end> value)
{
    static_assert(start < end, "Invalid bitfield definition! 'start' must be smaller than 'end'");
    static_assert(start >= 0 && end >= 0,
                  "Invalid bitfield definition! 'start' and 'end' must be at least 0");

    constexpr uint32_t numberOfBits = (end - start) + 1;
    constexpr uint32_t numAffectedBytes = affectedBytes(start, end);

    static_assert(numberOfBits <= 64, "Bitfield::read can read at most 64 bits");

    using ValueType = UInteger<start, end>;

    // First bit to write in the first affected Byte (i.e. start % 8)
    constexpr uint16_t pos = start & 0x7;
    constexpr uint16_t index = start / numberOfBitsPerByte;

    constexpr uint32_t bitpos = numAffectedBytes * numberOfBitsPerByte - (pos + numberOfBits);
    constexpr uint32_t maxByte =
            numAffectedBytes - 1;  // The index offset of the last byte to write to
    // The number of bits to shfit for the last byte
    constexpr ValueType mask = getMask<ValueType>(numberOfBits);

    // Compiler should be smart enough to optimize the loop
    for (uint32_t i = 0; i < maxByte; ++i)
    {
        const uint32_t shift = (maxByte - i) * numberOfBitsPerByte - bitpos;
        byteArray[index + i] &= ~(mask >> shift);
        byteArray[index + i] |= (value >> shift) & 0xff;
    }
    byteArray[index + maxByte] &= ~(mask << bitpos);
    byteArray[index + maxByte] |= (value << bitpos) & 0xff;
}

template <unsigned int start, unsigned int end>
outpost::BitfieldLittleEndian::UInteger<start, end>
outpost::BitfieldLittleEndian::read(const uint8_t* byteArray)
{
    using ReturnType = UInteger<start, end>;

    ReturnType value = outpost::Bitfield::read<start, end>(byteArray);

    return byteSwap<ReturnType>(value);
}

template <unsigned int start, unsigned int end>
void
outpost::BitfieldLittleEndian::write(uint8_t* byteArray, UInteger<start, end> value)
{
    using ValueType = UInteger<start, end>;

    value = byteSwap<ValueType>(value);

    outpost::Bitfield::write<start, end>(byteArray, value);
}

#endif
