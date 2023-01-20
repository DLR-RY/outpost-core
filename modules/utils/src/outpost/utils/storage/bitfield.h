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

#ifndef OUTPOST_UTILS_BITFIELD_H
#define OUTPOST_UTILS_BITFIELD_H

#include <limits>
#include <stdint.h>

#include <type_traits>  // std::conditional

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
    // Determine the most appropriate underlying type for the BitField
    template <unsigned int N>
    struct IntegerFromBitSize
    {
        static_assert(N <= 64, "BitField can read/write at most 64 bits");
        using type = typename std::conditional<
                N <= 8,
                uint8_t,
                typename std::conditional<
                        N <= 16,
                        uint16_t,
                        typename std::conditional<N <= 32, uint32_t, uint64_t>::type>::type>::type;
        static constexpr uint32_t size = 8 * sizeof(type);
    };

    template <unsigned int start, unsigned int end>
    using UInteger = typename IntegerFromBitSize<end - start + 1>::type;

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
    template <unsigned int offset>
    static bool
    read(const uint8_t* byteArray);

    /**
     * Read multiple bits from a byte stream.
     *
     * Reads up to 64 bits. The bits must fit into consecutive bytes in the
     * byte array!
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
    template <unsigned int start, unsigned int end>
    static UInteger<start, end>
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
    template <unsigned int offset>
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
    template <unsigned int start, unsigned int end>
    static void
    write(uint8_t* byteArray, UInteger<start, end> value);

private:
    static constexpr unsigned int numberOfBitsPerByte = 8;

    static constexpr inline unsigned int
    affectedBytes(unsigned int start, unsigned int end)
    {
        return ((end / numberOfBitsPerByte) - (start / numberOfBitsPerByte)) + 1;
    }

    template <typename T>
    static constexpr T
    getMask(uint32_t numBits = sizeof(T) * numberOfBitsPerByte)
    {
        return (std::numeric_limits<T>::max() >> (sizeof(T) * numberOfBitsPerByte - numBits));
    }
};

/**
 * Set and get multiple bits from a byte array.
 *
 * The data in the bytes is assumed to be in little endian order (lower byte
 * first). Uses MSB 0 bit ordering (bit 0 is the MSB).
 *
 * \author  Jan Sommer
 */
class BitfieldLittleEndian
{
public:
    template <unsigned int start, unsigned int end>
    using UInteger = Bitfield::UInteger<start, end>;

    // Disable compiler generated functions
    BitfieldLittleEndian() = delete;

    ~BitfieldLittleEndian() = delete;

    BitfieldLittleEndian(const BitfieldLittleEndian& other) = delete;

    BitfieldLittleEndian&
    operator=(const BitfieldLittleEndian& other) = delete;

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
    template <unsigned int offset>
    static bool
    read(const uint8_t* byteArray)
    {
        return Bitfield::read<offset>(byteArray);
    }

    /**
     * Read multiple bits from a byte stream.
     *
     * Reads up to 64 bit. The bits must fit into consecutive bytes in the
     * byte array!
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
    template <unsigned int start, unsigned int end>
    static UInteger<start, end>
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
    template <unsigned int offset>
    static void
    write(uint8_t* byteArray, bool value)
    {
        Bitfield::write<offset>(byteArray, value);
    }

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
    template <unsigned int start, unsigned int end>
    static void
    write(uint8_t* byteArray, UInteger<start, end> value);

private:
    static constexpr uint8_t numberOfBitsPerByte = 8;
    /**
     * Swap the endianness of a given value
     *
     * see https://stackoverflow.com/a/36937049
     *
     * For constant values this function is optimized away by compiler
     *
     * TODO: When outpost moves to  C++17 a version without recursion is possible
     */
    template <typename T>
    static constexpr T
    byteSwap(T i, T j = 0u, uint32_t n = 0u)
    {
        return n == sizeof(T) ? j
                              : byteSwap<T>(i >> numberOfBitsPerByte,
                                            (j << numberOfBitsPerByte) | (i & static_cast<T>(0xFF)),
                                            n + 1);
    }
};

}  // namespace outpost

#include "bitfield_impl.h"

#endif
