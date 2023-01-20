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

#ifndef OUTPOST_CRC16_H
#define OUTPOST_CRC16_H

#include <outpost/base/slice.h>

#include <stddef.h>
#include <stdint.h>

namespace outpost
{
/**
 * CRC-16-CCITT calculation.
 *
 * Polynomial    : x^16 + x^12 + x^5 + 1 (0x1021, MSB first)
 * Initial value : 0xFFFF
 *
 * Used for space packet transfer frames.
 *
 * \ingroup crc
 * \author  Fabian Greif
 */
class Crc16Ccitt
{
public:
    inline Crc16Ccitt() : mCrc(initialValue)
    {
    }

    inline ~Crc16Ccitt()
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

    static constexpr uint16_t initialValue = 0xFFFF;
    static constexpr int32_t numberOfBitsPerByte = 8;
    static constexpr int32_t numberOfValuesPerByte = 256;

    /// Pre-calculated CRC table for one byte
    static const uint16_t crcTable[numberOfValuesPerByte];

    uint16_t mCrc;
};
}  // namespace outpost

#endif
