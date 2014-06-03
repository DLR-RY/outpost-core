/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
/**
 * \file
 * \brief    Test CRC functions
 *
 * See also the "On-line CRC calculation and free library"
 * (http://www.lammertbies.nl/comm/info/crc-calculation.html). The code
 * presented here implements the "CRC-CCITT (0xFFFF)" routine.
 *
 *
 * \author    Fabian Greif
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cobc/utils/crc.h>

using namespace cobc;

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testEcssPus1)
{
    uint8_t data[] = {
        0x00, 0x00
    };

    ASSERT_EQ(0x1d0f, Crc16Ccitt::calculate(data, 2));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testEcssPus2)
{
    uint8_t data[] = {
        0x00, 0x00, 0x00
    };

    ASSERT_EQ(0xcc9c, Crc16Ccitt::calculate(data, 3));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testEcssPus3)
{
    uint8_t data[] = {
        0xab, 0xcd, 0xef, 0x01
    };

    ASSERT_EQ(0x04a2, Crc16Ccitt::calculate(data, 4));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testEcssPus4)
{
    uint8_t data[] = {
        0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01
    };

    ASSERT_EQ(0x7fd5, Crc16Ccitt::calculate(data, 6));
}

/**
 * Test the result of a CRC over the message + CRC, should be 0
 */
TEST(CrcTest, testEcssPus4Total)
{
    uint8_t data[] = {
        0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01, 0x7f, 0xd5
    };

    ASSERT_EQ(0, Crc16Ccitt::calculate(data, sizeof(data)));
}

/**
 * Test the manual CRC generation instead of calling only calculate().
 *
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testUpdate)
{
    uint8_t data[] = {
        0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01
    };

    Crc16Ccitt crc;

    for (uint_fast8_t i = 0; i < sizeof(data); ++i) {
        crc.update(data[i]);
    }

    ASSERT_EQ(0x7fd5, crc.getValue());
}

// code from http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
uint16_t
crc_xmodem_update(uint16_t crc, uint8_t data)
{
    crc = crc ^ (static_cast<uint16_t>(data) << 8);
    for (uint_fast16_t i = 0; i < 8; ++i)
    {
        if (crc & 0x8000)
        {
            crc = (crc << 1) ^ 0x1021;
        }
        else
        {
            crc <<= 1;
        }
    }

    return crc;
}

/**
 * Test that the CRC used here matches the Xmodem CRC from avr-libc
 */
TEST(CrcTest, testXModemAvr)
{
    uint8_t data[] = {
        0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01
    };

    uint16_t crc = 0xffff;
    for (uint_fast8_t i = 0; i < sizeof(data); ++i) {
        crc = crc_xmodem_update(crc, data[i]);
    }

    ASSERT_EQ(0x7fd5, crc);
}

TEST(CrcTest, testRandom)
{
    uint8_t data[512];

    for (uint_fast16_t i = 0; i < 512; ++i) {
        data[i] = 0xff;
    }

    ASSERT_EQ(0x6995, Crc16Ccitt::calculate(data, 512));
}

TEST(CrcTest, testRandom2)
{
    uint8_t data[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    ASSERT_EQ(0x29B1, Crc16Ccitt::calculate(data, sizeof(data)));
}

TEST(CrcTest, testRandom3)
{
    uint8_t data[] = {
        0xff, 0xff
    };

    ASSERT_EQ(0, Crc16Ccitt::calculate(data, sizeof(data)));
}
