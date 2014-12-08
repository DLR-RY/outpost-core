/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#ifndef COBC_CRC_H
#define COBC_CRC_H

#include <stdint.h>
#include <stddef.h>

namespace cobc
{
/**
 * CRC-16-CCITT calculation.
 *
 * Polynomial    : x^16 + x^12 + x^5 + 1 (0x8408, LSB first)
 * Initial value : 0xffff
 *
 * Used for SPP transfer frames.
 */
class Crc16Ccitt
{
public:
    inline
    Crc16Ccitt() :
        mCrc(initialValue)
    {
    }

    inline
    ~Crc16Ccitt()
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
    static uint16_t
    calculate(const uint8_t* data,
              size_t length);

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
    inline uint16_t
    getValue() const
    {
        return mCrc;
    }

private:
    // disable copy constructor
    Crc16Ccitt(const Crc16Ccitt&);

    // disable copy-assignment operator
    Crc16Ccitt&
    operator=(const Crc16Ccitt&);

    static const uint16_t initialValue = 0xFFFF;
    static const int numberOfBitsPerByte = 8;
    static const int numberOfValuesPerByte = 256;

    /// Pre-calculated CRC table for one byte
    static const uint16_t crcTable[numberOfValuesPerByte];

    uint16_t mCrc;
};
}

#endif // COBC_CRC_H
