/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

/**
 * \file
 * \brief   Test CRC8 functions
 *
 * \author  Fabian Greif
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <outpost/coding/crc8.h>

using outpost::Crc8CcittReversed;
using outpost::Crc8Ccitt;

static uint8_t
crc8_update_bitwise_msb_first(uint8_t crc, uint8_t data)
{
    uint8_t c = crc ^ data;
    for (uint_fast8_t i = 0; i < 8; i++)
    {
        if (c & 0x80)
        {
            c = 0x07 ^ (c << 1);
        }
        else
        {
            c = c << 1;
        }
    }
    return c;
}

TEST(Crc8CcittTest, bitwiseTest)
{
    uint8_t data[] =
    {
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF
    };

    uint8_t crc = 0;
    for (auto d : data)
    {
        crc = crc8_update_bitwise_msb_first(crc, d);
    }

    EXPECT_EQ(crc, Crc8Ccitt::calculate(outpost::toArray(data)));
}

// ----------------------------------------------------------------------------
TEST(Crc8CcittReversedTest, initialValue)
{
    Crc8CcittReversed crc;

    EXPECT_EQ(0x00U, crc.getValue());
}

/**
 * The test is based on the example given in
 * ECSS-E-50-11 Draft F (Version from December 2006)
 */
TEST(Crc8CcittReversedTest, rmapTestPattern1)
{
    uint8_t data[] = {
        0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08
    };

    EXPECT_EQ(0xB0U, Crc8CcittReversed::calculate(outpost::toArray(data)));
}

/**
 * The test is based on the example given in
 * ECSS-E-50-11 Draft F (Version from December 2006)
 */
TEST(Crc8CcittReversedTest, rmapTestPattern2)
{
    uint8_t data[] = {
        0x53, 0x70, 0x61, 0x63,
        0x65, 0x57, 0x69, 0x72,
        0x65, 0x20, 0x69, 0x73,
        0x20, 0x62, 0x65, 0x61,
        0x75, 0x74, 0x69, 0x66,
        0x75, 0x6C, 0x21, 0x21
    };

    EXPECT_EQ(0x84U, Crc8CcittReversed::calculate(outpost::toArray(data)));
}

/**
 * The test is based on the example given in
 * ECSS-E-50-11 Draft F (Version from December 2006)
 */
TEST(Crc8CcittReversedTest, rmapTestPattern3)
{
    uint8_t data[] = {
        0x10, 0x56, 0xC3, 0x95,
        0xA5, 0x75, 0x38, 0x63,
        0x2F, 0x86, 0x7B, 0x01,
        0x32, 0xDE, 0x35, 0x7A
    };

    EXPECT_EQ(0x18U, Crc8CcittReversed::calculate(outpost::toArray(data)));
}

/**
 * Test the result of a CRC over the message + CRC, should be 0
 */
TEST(Crc8CcittReversedTest, rmapTestPattern3Total)
{
    uint8_t data[] = {
        0x10, 0x56, 0xC3, 0x95,
        0xA5, 0x75, 0x38, 0x63,
        0x2F, 0x86, 0x7B, 0x01,
        0x32, 0xDE, 0x35, 0x7A,
        0x18
    };

    EXPECT_EQ(0U, Crc8CcittReversed::calculate(outpost::toArray(data)));
}

/**
 * see ECSS‐E‐ST‐50‐52C Appendix A.4 (February 2010)
 */
TEST(Crc8CcittReversedTest, rmapCrcTestPattern1)
{
    uint8_t header[] = {
        0xFE, 0x01, 0x6C, 0x00,
        0x67,
        0x00, 0x00,
        0x00,
        0xA0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x10
    };

    uint8_t data[] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17
    };

    EXPECT_EQ(0x9FU, Crc8CcittReversed::calculate(outpost::toArray(header)));
    EXPECT_EQ(0x56U, Crc8CcittReversed::calculate(outpost::toArray(data)));
}

/**
 * see ECSS‐E‐ST‐50‐52C Appendix A.4 (February 2010)
 *
 * RMAP incrementing read command.
 */
TEST(Crc8CcittReversedTest, rmapCrcTestPatternCommand)
{
    uint8_t header[] = {
        0xFE, 0x01, 0x4D, 0x00,
        0x99, 0xAA, 0xBB, 0xCC, 0x67,
        0x00, 0x03,
        0x00,
        0xA0, 0x00, 0x00, 0x10,
        0x00, 0x00, 0x10
    };

    EXPECT_EQ(0xF7U, Crc8CcittReversed::calculate(outpost::toArray(header)));
}


/**
 * see ECSS‐E‐ST‐50‐52C Appendix A.4 (February 2010)
 *
 * Expected RMAP successful read reply to the previous command.
 */
TEST(Crc8CcittReversedTest, rmapCrcTestPatternReply)
{
    uint8_t header[] = {
        0x67, 0x01, 0x0D, 0x00,
        0xFE,
        0x00, 0x03,
        0x00,
        0x00, 0x00, 0x10
    };

    uint8_t data[] = {
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF
    };

    EXPECT_EQ(0x52U, Crc8CcittReversed::calculate(outpost::toArray(header)));
    EXPECT_EQ(0xB4U, Crc8CcittReversed::calculate(outpost::toArray(data)));
}

static uint8_t
crc8_update_bitwise(uint8_t crc, uint8_t data)
{
    uint8_t c = crc ^ data;
    for (uint_fast8_t i = 0; i < 8; i++)
    {
        if (c & 1)
        {
            c = 0xE0 ^ (c >> 1);
        }
        else
        {
            c = c >> 1;
        }
    }
    return c;
}

TEST(Crc8CcittReversedTest, bitwiseTest)
{
    uint8_t data[] =
    {
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
        0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF
    };

    uint8_t crc = 0;
    for (auto d : data)
    {
        crc = crc8_update_bitwise(crc, d);
    }

    EXPECT_EQ(crc, Crc8CcittReversed::calculate(outpost::toArray(data)));
}
