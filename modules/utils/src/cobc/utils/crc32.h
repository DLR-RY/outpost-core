/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#ifndef COBC_CRC32_H
#define COBC_CRC32_H

#include <stdint.h>
#include <stddef.h>

#include "bounded_array.h"

namespace cobc
{
/**
 * CRC-32-CCITT calculation.
 *
 * Polynomial    : X^32 + X^26 + X^23 + X^22 + X^16 + X^12 + X^11 +
 *                 X^10 + X^8 + X^7 + X^5 + X^4 + X^2 + X + 1
 *                 (0xEDB88320, LSB first)
 * Initial value : 0xFFFFFFFF
 * Final XOR     : 0xFFFFFFFF
 *
 * Used in IEEE 802.3 and PNG [2], see also ISO 3309 or ITU-T V.42, [1] and [3]
 *
 * [1] https://users.ece.cmu.edu/~koopman/crc/crc32.html
 * [2] http://www.w3.org/TR/PNG/#D-CRCAppendix
 * [3] http://www.greenend.org.uk/rjk/tech/crc.html
 *
 * \author  Fabian Greif
 */
class Crc32Ccitt
{
public:
    inline
    Crc32Ccitt() :
        mCrc(initialValue)
    {
    }

    inline
    ~Crc32Ccitt()
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
    static uint32_t
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
    inline uint32_t
    getValue() const
    {
        return mCrc ^ finalXor;
    }

private:
    // disable copy constructor
    Crc32Ccitt(const Crc32Ccitt&);

    // disable copy-assignment operator
    Crc32Ccitt&
    operator=(const Crc32Ccitt&);

    static const uint32_t initialValue = 0xFFFFFFFF;
    static const uint32_t finalXor     = 0xFFFFFFFF;


    static const int numberOfBitsPerByte = 8;
    static const int numberOfValuesPerByte = 256;

    /// Pre-calculated CRC table for one byte
    static const uint32_t crcTable[numberOfValuesPerByte];

    uint32_t mCrc;
};
}

#endif
