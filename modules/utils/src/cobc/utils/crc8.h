/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#ifndef COBC_CRC8_H
#define COBC_CRC8_H

#include <stdint.h>
#include <stddef.h>

#include "bounded_array.h"

namespace cobc
{
/**
 * CRC-8 CCITT.
 *
 * Polynomial    : x^8 + x^2 + x + 1 (0x07, MSB first)
 * Initial value : 0x00
 *
 * \ingroup crc
 * \author  Fabian Greif
 */
class Crc8Ccitt
{
public:
    inline
    Crc8Ccitt() :
        mCrc(initialValue)
    {
    }

    inline
    ~Crc8Ccitt()
    {
    }

    /**
     * Calculate CRC from a block of data.
     *
     * \param data
     *     address
     * \param length
     *     length
     *
     * \retval crc
     *     calculated checksum
     */
    static uint8_t
    calculate(cobc::BoundedArray<const uint8_t> data);

    /**
     * Reset CRC calculation
     */
    inline void
    reset()
    {
        mCrc = initialValue;
    }

    /**
     * CRC update.
     *
     * \param data
     *     byte
     */
    void
    update(uint8_t data);

    /**
     * Get result of CRC calculation.
     */
    inline uint8_t
    getValue() const
    {
        return mCrc;
    }

private:
    // disable copy constructor
    Crc8Ccitt(const Crc8Ccitt&);

    // disable copy-assignment operator
    Crc8Ccitt&
    operator=(const Crc8Ccitt&);

    static const uint8_t initialValue = 0x00;
    static const int numberOfValuesPerByte = 256;

    /// Pre-calculated CRC table for one byte
    static const uint8_t crcTable[numberOfValuesPerByte];

    uint8_t mCrc;
};

/**
 * CRC-8 calculation for RMAP.
 *
 * Polynomial    : x^8 + x^2 + x + 1 (0xE0, LSB first)
 * Initial value : 0x00
 *
 * \see     ECSS-E-50-11 SpaceWire RMAP Protocol
 *
 * \ingroup crc
 * \author  Fabian Greif
 */
class Crc8CcittReversed
{
public:
    inline
    Crc8CcittReversed() :
        mCrc(initialValue)
    {
    }

    inline
    ~Crc8CcittReversed()
    {
    }

    /**
     * Calculate CRC from a block of data.
     *
     * \param data
     *     address
     * \param length
     *     length
     *
     * \retval crc
     *     calculated checksum
     */
    static uint8_t
    calculate(cobc::BoundedArray<const uint8_t> data);

    /**
     * Reset CRC calculation
     */
    inline void
    reset()
    {
        mCrc = initialValue;
    }

    /**
     * CRC update.
     *
     * \param data
     *     byte
     */
    void
    update(uint8_t data);

    /**
     * Get result of CRC calculation.
     */
    inline uint8_t
    getValue() const
    {
        return mCrc;
    }

private:
    // disable copy constructor
    Crc8CcittReversed(const Crc8CcittReversed&);

    // disable copy-assignment operator
    Crc8CcittReversed&
    operator=(const Crc8CcittReversed&);

    static const uint8_t initialValue = 0x00;
    static const int numberOfValuesPerByte = 256;

    /// Pre-calculated CRC table for one byte
    static const uint8_t crcTable[numberOfValuesPerByte];

    uint8_t mCrc;
};
}

#endif
