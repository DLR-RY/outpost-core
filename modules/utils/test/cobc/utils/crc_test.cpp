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

using cobc::Crc16Ccitt;

TEST(CrcTest, initialValue)
{
    Crc16Ccitt crc;

    EXPECT_EQ(0xFFFF, crc.getValue());
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testEcssPus1)
{
    uint8_t data[] = {
        0x00, 0x00
    };

    EXPECT_EQ(0x1D0F, Crc16Ccitt::calculate(data, 2));
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

    EXPECT_EQ(0xCC9C, Crc16Ccitt::calculate(data, 3));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testEcssPus3)
{
    uint8_t data[] = {
        0xAB, 0xCD, 0xEF, 0x01
    };

    EXPECT_EQ(0x04A2, Crc16Ccitt::calculate(data, 4));
}

/**
 * The test is based on the example given in
 * ECSS-E-70-41A Annex A (Version from January 2003).
 */
TEST(CrcTest, testEcssPus4)
{
    uint8_t data[] = {
        0x14, 0x56, 0xF8, 0x9A, 0x00, 0x01
    };

    EXPECT_EQ(0x7FD5, Crc16Ccitt::calculate(data, 6));
}

/**
 * Test the result of a CRC over the message + CRC, should be 0
 */
TEST(CrcTest, testEcssPus4Total)
{
    uint8_t data[] = {
        0x14, 0x56, 0xF8, 0x9A, 0x00, 0x01, 0x7F, 0xD5
    };

    EXPECT_EQ(0, Crc16Ccitt::calculate(data, sizeof(data)));
}

TEST(CrcTest, testMltpExample1)
{
	uint8_t data[] = {
		0x30, 0xC1, 0xA1, 0x7D, 0x43, 0xD2, 0x10, 0x82,
		0xAB, 0x32, 0x7D, 0xEF, 0x7E, 0x10, 0x81, 0x08,
		0x34, 0x7E, 0xE5, 0xC3, 0x90,
	};

    EXPECT_EQ(0x4959, Crc16Ccitt::calculate(data, sizeof(data)));
}


TEST(CrcTest, testMltpExample2)
{
	uint8_t data[] = {
		0x10, 0x82, 0x23, 0x7D, 0x43, 0xD2,
	};

    EXPECT_EQ(0xF7AC, Crc16Ccitt::calculate(data, sizeof(data)));
}

TEST(CrcTest, testMltpExample3)
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

    EXPECT_EQ(0xB233, Crc16Ccitt::calculate(data0, sizeof(data0)));
    EXPECT_EQ(0x8541, Crc16Ccitt::calculate(data1, sizeof(data1)));
    EXPECT_EQ(0x0493, Crc16Ccitt::calculate(data2, sizeof(data2)));
}

TEST(CrcTest, testMltpExample4)
{
	uint8_t data[] = {
		0x30, 0xC1, 0x01, 0x7D, 0x43, 0xD2, 0x10, 0x82,
	};

    EXPECT_EQ(0x8CC7, Crc16Ccitt::calculate(data, sizeof(data)));
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

    EXPECT_EQ(0x7fd5, crc.getValue());
}

// code from http://www.nongnu.org/avr-libc/user-manual/group__util__crc.html
uint16_t
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
TEST(CrcTest, testXModemAvr)
{
    uint8_t data[] = {
        0x14, 0x56, 0xf8, 0x9a, 0x00, 0x01
    };

    uint16_t crc = 0xffff;
    for (uint_fast8_t i = 0; i < sizeof(data); ++i) {
        crc = crc_xmodem_update(crc, data[i]);
    }

    EXPECT_EQ(0x7fd5, crc);
}

TEST(CrcTest, testRandom)
{
    uint8_t data[512];

    for (uint_fast16_t i = 0; i < 512; ++i) {
        data[i] = 0xff;
    }

    EXPECT_EQ(0x6995, Crc16Ccitt::calculate(data, 512));
}

TEST(CrcTest, testRandom2)
{
    uint8_t data[] = {
        '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };

    EXPECT_EQ(0x29B1, Crc16Ccitt::calculate(data, sizeof(data)));
}

TEST(CrcTest, testRandom3)
{
    uint8_t data[] = {
        0xff, 0xff
    };

    EXPECT_EQ(0, Crc16Ccitt::calculate(data, sizeof(data)));
}

