/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_CRC_H
#define COBC_CRC_H

#include <stdint.h>
#include <cstddef>

namespace cobc
{
/**
 * CRC-16-CCITT calculation.
 *
 * Polynomial    : x^16 + x^12 + x^5 + 1 (0x8408, LSB first)
 * Initial value : 0xffff
 *
 * Used for CCSDS transfer frames.
 */
class Crc16Ccitt
{
public:
    inline Crc16Ccitt() : crc(initialValue)
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
    calculate(const uint8_t * data, size_t length);

    /**
     * Reset CRC calculation
     */
    inline void
    reset()
    {
        crc = initialValue;
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
        return crc;
    }

private:
    static const uint16_t initialValue = 0xFFFF;
    uint16_t crc;
};
}

#endif // COBC_CRC_H
