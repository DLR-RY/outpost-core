/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2018, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_CRC32_H
#define OUTPOST_CRC32_H

#include <outpost/base/slice.h>

#include <stddef.h>
#include <stdint.h>

namespace outpost
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
 * \ingroup crc
 * \author  Fabian Greif
 */
class Crc32Reversed
{
public:
    inline Crc32Reversed() : mCrc(initialValue)
    {
    }

    inline ~Crc32Reversed()
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
    calculate(outpost::Slice<const uint8_t> data);

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
    Crc32Reversed(const Crc32Reversed&);

    // disable copy-assignment operator
    Crc32Reversed&
    operator=(const Crc32Reversed&);

    static const uint32_t initialValue = 0xFFFFFFFF;
    static const uint32_t finalXor = 0xFFFFFFFF;

    static const int numberOfBitsPerByte = 8;
    static const int numberOfValuesPerByte = 256;

    /// Pre-calculated CRC table for one byte
    static const uint32_t crcTable[numberOfValuesPerByte];

    uint32_t mCrc;
};
}  // namespace outpost

#endif
