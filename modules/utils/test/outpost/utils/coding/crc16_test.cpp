/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
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

#include <outpost/utils/coding/crc16.h>

using outpost::Crc16Ccitt;

TEST(Crc16Test, initialValue)
{
    Crc16Ccitt crc;

    EXPECT_EQ(0xFFFF, crc.getValue());
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(Crc16Test, testEcssPus1)
{
    uint8_t data[] = {
        0x00, 0x00
    };

    EXPECT_EQ(0x1D0F, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(Crc16Test, testEcssPus2)
{
    uint8_t data[] = {
        0x00, 0x00, 0x00
    };

    EXPECT_EQ(0xCC9C, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(Crc16Test, testEcssPus3)
{
    uint8_t data[] = {
        0xAB, 0xCD, 0xEF, 0x01
    };

    EXPECT_EQ(0x04A2, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(Crc16Test, testEcssPus4)
{
    uint8_t data[] = {
        0x14, 0x56, 0xF8, 0x9A, 0x00, 0x01
    };

    EXPECT_EQ(0x7FD5, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

/**
 * Test the result of a CRC over the message + CRC, should be 0
 */
TEST(Crc16Test, testEcssPus4Total)
{
    uint8_t data[] = {
        0x14, 0x56, 0xF8, 0x9A, 0x00, 0x01, 0x7F, 0xD5
    };

    EXPECT_EQ(0, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

TEST(Crc16Test, testMltpExample1)
{
    uint8_t data[] = {
        0x30, 0xC1, 0xA1, 0x7D, 0x43, 0xD2, 0x10, 0x82,
        0xAB, 0x32, 0x7D, 0xEF, 0x7E, 0x10, 0x81, 0x08,
        0x34, 0x7E, 0xE5, 0xC3, 0x90,
    };

    EXPECT_EQ(0x4959, Crc16Ccitt::calculate(outpost::asSlice(data)));
}


TEST(Crc16Test, testMltpExample2)
{
    uint8_t data[] = {
        0x10, 0x82, 0x23, 0x7D, 0x43, 0xD2,
    };

    EXPECT_EQ(0xF7AC, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

TEST(Crc16Test, testMltpExample3)
{
    uint8_t data0[] = {
        0x10, 0x82, 0x8F, 0x7D, 0x00,
        0x00, 0x11, 0x43, 0xD2, 0xAB, 0x32, 0x7D, 0xEF,
    };

    uint8_t data1[] = {
        0x10, 0x82, 0x8F, 0x7D, 0x01,
        0x7E, 0x10, 0x81, 0x08, 0x34, 0x7E, 0xE5, 0xC3,
    };

    uint8_t data2[] = {
        0x10, 0x82, 0x8F, 0x7D, 0x02,
        0x90,
    };

    EXPECT_EQ(0xB233, Crc16Ccitt::calculate(outpost::asSlice(data0)));
    EXPECT_EQ(0x8541, Crc16Ccitt::calculate(outpost::asSlice(data1)));
    EXPECT_EQ(0x0493, Crc16Ccitt::calculate(outpost::asSlice(data2)));
}

TEST(Crc16Test, testMltpExample4)
{
    uint8_t data[] = {
        0x30, 0xC1, 0x01, 0x7D, 0x43, 0xD2, 0x10, 0x82,
    };

    EXPECT_EQ(0x8CC7, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

/**
 * Test the manual CRC generation instead of calling only calculate().
 *
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(Crc16Test, testUpdate)
{
    uint8_t data[] = {
        0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01
    };

    Crc16Ccitt crc;

    for (uint_fast8_t i = 0; i < sizeof(data); ++i) {
        crc.update(data[i]);
    }

    EXPECT_EQ(0x7fd5, crc.getValue());
}

// code from http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
static uint16_t
crc_xmodem_update(uint16_t crc, uint8_t data)
{
    crc = crc ^ (static_cast<uint16_t>(data) << 8);
    for (uint_fast8_t i = 0; i < 8; ++i)
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
TEST(Crc16Test, testXModemAvr)
{
    uint8_t data[] = {
        0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01
    };

    uint16_t crc = 0xffff;
    for (uint_fast8_t i = 0; i < sizeof(data); ++i)
    {
        crc = crc_xmodem_update(crc, data[i]);
    }

    EXPECT_EQ(crc, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

TEST(Crc16Test, testRandom)
{
    uint8_t data[512];

    for (uint_fast16_t i = 0; i < 512; ++i) {
        data[i] = 0xff;
    }

    EXPECT_EQ(0x6995, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

TEST(Crc16Test, testRandom2)
{
    uint8_t data[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    EXPECT_EQ(0x29B1, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

TEST(Crc16Test, testRandom3)
{
    uint8_t data[] = {
        0xff, 0xff
    };

    EXPECT_EQ(0, Crc16Ccitt::calculate(outpost::asSlice(data)));
}

