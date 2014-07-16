/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "crc.h"
#include "crc_table.h"

using namespace cobc;

void
Crc16Ccitt::update(uint8_t data)
{
    crc = (crc << 8) ^ crcTable[(crc >> 8) ^ data];
}

uint16_t
Crc16Ccitt::calculate(const uint8_t* data, size_t length)
{
    Crc16Ccitt crc;
    for (size_t i = 0; i < length; ++i)
    {
        crc.update(data[i]);
    }

    return crc.getValue();
}
