/*
 * Copyright (c) 2014-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 * - 2018, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/utils/storage/bitfield.h>

#include <unittest/harness.h>

using outpost::Bitfield;

class BitfieldTest : public testing::Test
{
public:
    BitfieldTest() : data{0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34}
    {
    }

    uint8_t data[10];
};

TEST_F(BitfieldTest, readBits)
{
    bool value;

    value = Bitfield::read<0>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<1>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<2>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<3>(data);
    EXPECT_TRUE(value);

    value = Bitfield::read<4>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<5>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<6>(data);
    EXPECT_TRUE(value);

    value = Bitfield::read<7>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<8>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<9>(data);
    EXPECT_FALSE(value);

    value = Bitfield::read<10>(data);
    EXPECT_TRUE(value);

    value = Bitfield::read<11>(data);
    EXPECT_TRUE(value);
}

TEST_F(BitfieldTest, readSingleByte)
{
    uint16_t value = Bitfield::read<0, 7>(data);
    EXPECT_EQ(0x12U, value);
}

TEST_F(BitfieldTest, readSingleByte2)
{
    uint16_t value = Bitfield::read<16, 23>(data);
    EXPECT_EQ(0x56U, value);
}

TEST_F(BitfieldTest, readoverThreeBytes)
{
    uint16_t value = Bitfield::read<4, 19>(data);
    EXPECT_EQ(0x2345U, value);
}

TEST_F(BitfieldTest, readoverFiveBytes)
{
    uint32_t value = Bitfield::read<4, 35>(data);
    EXPECT_EQ(0x23456789U, value);
}

TEST_F(BitfieldTest, FFFreadoverFiveBytes)
{
    uint64_t value = Bitfield::read<0, 39>(data);
    EXPECT_EQ(0x1234567890U, value);
}

TEST_F(BitfieldTest, readoverNineBytes)
{
    uint64_t value = Bitfield::read<4, 67>(data);
    EXPECT_EQ(0x234567890ABCDEF1U, value);
}

TEST_F(BitfieldTest, readTwoBytesFromStart)
{
    uint16_t value = Bitfield::read<0, 15>(data);
    EXPECT_EQ(0x1234U, value);
}

TEST_F(BitfieldTest, readFourBytesFromStart)
{
    uint32_t value = Bitfield::read<0, 31>(data);
    EXPECT_EQ(0x12345678U, value);
}

TEST_F(BitfieldTest, readEightBytesFromStart)
{
    uint64_t value = Bitfield::read<0, 63>(data);
    EXPECT_EQ(0x1234567890ABCDEFU, value);
}

TEST_F(BitfieldTest, readTwoBytesWithOffset)
{
    uint16_t value = Bitfield::read<8, 23>(data);
    EXPECT_EQ(0x3456U, value);
}

TEST_F(BitfieldTest, readTwoBytesWithOffset2)
{
    uint8_t reference[4] = {0xFF, 0x00, 0x00, 0xFF};

    uint16_t value = Bitfield::read<8, 23>(reference);
    EXPECT_EQ(0x0000U, value);
}

TEST_F(BitfieldTest, readFourBytesWithOffset)
{
    uint32_t value = Bitfield::read<8, 39>(data);
    EXPECT_EQ(0x34567890U, value);
}

TEST_F(BitfieldTest, readEightBytesWithOffset)
{
    uint64_t value = Bitfield::read<8, 71>(data);
    EXPECT_EQ(0x34567890ABCDEF12U, value);
}

TEST_F(BitfieldTest, readShorterValue)
{
    uint16_t value = Bitfield::read<4, 7>(data);
    EXPECT_EQ(0x2U, value);
}

TEST_F(BitfieldTest, readValueOverlappingTwoBytes)
{
    uint16_t value = Bitfield::read<4, 11>(data);
    EXPECT_EQ(0x23U, value);
}

TEST_F(BitfieldTest, writeSingleByte)
{
    uint8_t buffer[8] = {};
    Bitfield::write<0, 7>(buffer, 0x12);
    EXPECT_EQ(0x12U, buffer[0]);
}

TEST_F(BitfieldTest, writeSingleByte2)
{
    uint8_t buffer[8] = {};
    Bitfield::write<16, 23>(buffer, 0x56);
    EXPECT_EQ(0x00U, buffer[0]);
    EXPECT_EQ(0x00U, buffer[1]);
    EXPECT_EQ(0x56U, buffer[2]);
    EXPECT_EQ(0x00U, buffer[3]);
}

TEST_F(BitfieldTest, writeTwoBytesFromStart)
{
    uint8_t buffer[8] = {};
    Bitfield::write<0, 15>(buffer, 0x1234);
    EXPECT_EQ(0x12U, buffer[0]);
    EXPECT_EQ(0x34U, buffer[1]);
}

TEST_F(BitfieldTest, writeTwoBytesWithOffset)
{
    uint8_t buffer[8] = {};
    Bitfield::write<8, 23>(buffer, 0x3456);
    EXPECT_EQ(0x34U, buffer[1]);
    EXPECT_EQ(0x56U, buffer[2]);
}

TEST_F(BitfieldTest, writeTwoBytesWithOffset2)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<8, 23>(buffer, 0);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0x00U, buffer[1]);
    EXPECT_EQ(0x00U, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldTest, writeIntoTowBytes)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<13, 20>(buffer, 0x34);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0xF9U, buffer[1]);
    EXPECT_EQ(0xA7U, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldTest, writeIntoThreeBytes)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<4, 19>(buffer, 0x1234);
    EXPECT_EQ(0xF1U, buffer[0]);
    EXPECT_EQ(0x23U, buffer[1]);
    EXPECT_EQ(0x4FU, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldTest, writeIntoThreeBytes2)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<5, 20>(buffer, 0x1234);
    EXPECT_EQ(0xF8U, buffer[0]);
    EXPECT_EQ(0x91U, buffer[1]);
    EXPECT_EQ(0xA7U, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldTest, writeShorterbuffer)
{
    uint8_t buffer[8] = {};
    Bitfield::write<4, 7>(buffer, 0x02);
    EXPECT_EQ(0x2U, buffer[0]);
}

TEST_F(BitfieldTest, writebufferOverlappingTwoBytes)
{
    uint8_t buffer[8] = {};
    Bitfield::write<4, 11>(buffer, 0x23);
    EXPECT_EQ(0x02U, buffer[0]);
    EXPECT_EQ(0x30U, buffer[1]);
}

TEST_F(BitfieldTest, shouldWriteWithBiggerOffset)
{
    uint8_t buffer[8] = {};
    Bitfield::write<41, 55>(buffer, 0x123);
    EXPECT_EQ(0x01U, buffer[5]);
    EXPECT_EQ(0x23U, buffer[6]);
}

TEST_F(BitfieldTest, writeFull4Bytes)
{
    uint8_t buffer[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<0, 31>(buffer, 0x12345678);
    EXPECT_EQ(0x12U, buffer[0]);
    EXPECT_EQ(0x34U, buffer[1]);
    EXPECT_EQ(0x56U, buffer[2]);
    EXPECT_EQ(0x78U, buffer[3]);
    EXPECT_EQ(0xFFU, buffer[4]);
}

TEST_F(BitfieldTest, writeFull4BytesWithByteOffset)
{
    uint8_t buffer[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<8, 39>(buffer, 0x12345678);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0x12U, buffer[1]);
    EXPECT_EQ(0x34U, buffer[2]);
    EXPECT_EQ(0x56U, buffer[3]);
    EXPECT_EQ(0x78U, buffer[4]);
}

TEST_F(BitfieldTest, writeInto5Bytes)
{
    uint8_t buffer[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<10, 38>(buffer, 0x12345678);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0xE4U, buffer[1]);
    EXPECT_EQ(0x68U, buffer[2]);
    EXPECT_EQ(0xACU, buffer[3]);
    EXPECT_EQ(0xF1U, buffer[4]);
}

TEST_F(BitfieldTest, writeFull8Bytes)
{
    uint8_t buffer[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<0, 63>(buffer, 0x12345678deadbeef);
    EXPECT_EQ(0x12U, buffer[0]);
    EXPECT_EQ(0x34U, buffer[1]);
    EXPECT_EQ(0x56U, buffer[2]);
    EXPECT_EQ(0x78U, buffer[3]);
    EXPECT_EQ(0xDEU, buffer[4]);
    EXPECT_EQ(0xADU, buffer[5]);
    EXPECT_EQ(0xBEU, buffer[6]);
    EXPECT_EQ(0xEFU, buffer[7]);
    EXPECT_EQ(0xFFU, buffer[8]);
}

TEST_F(BitfieldTest, writeFull8BytesWithOffset)
{
    uint8_t buffer[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<8, 71>(buffer, 0x12345678deadbeef);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0x12U, buffer[1]);
    EXPECT_EQ(0x34U, buffer[2]);
    EXPECT_EQ(0x56U, buffer[3]);
    EXPECT_EQ(0x78U, buffer[4]);
    EXPECT_EQ(0xDEU, buffer[5]);
    EXPECT_EQ(0xADU, buffer[6]);
    EXPECT_EQ(0xBEU, buffer[7]);
    EXPECT_EQ(0xEFU, buffer[8]);
}

TEST_F(BitfieldTest, writeInto9Bytes)
{
    uint8_t buffer[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    Bitfield::write<10, 70>(buffer, 0x12345678deadbeef);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0xE4U, buffer[1]);
    EXPECT_EQ(0x68U, buffer[2]);
    EXPECT_EQ(0xACU, buffer[3]);
    EXPECT_EQ(0xF1U, buffer[4]);
    EXPECT_EQ(0xBDU, buffer[5]);
    EXPECT_EQ(0x5BU, buffer[6]);
    EXPECT_EQ(0x7DU, buffer[7]);
    EXPECT_EQ(0xDFU, buffer[8]);
}
