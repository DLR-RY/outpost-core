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
 * - 2021, Jan Sommer (DLR SC-SRV)
 */

#include <outpost/utils/storage/bitfield.h>

#include <unittest/harness.h>

using outpost::BitfieldLittleEndian;

class BitfieldLittleEndianTest : public testing::Test
{
public:
    BitfieldLittleEndianTest() : data{0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34}
    {
    }

    uint8_t data[10];
};

TEST_F(BitfieldLittleEndianTest, readBits)
{
    bool value;

    value = BitfieldLittleEndian::read<0>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<1>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<2>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<3>(data);
    EXPECT_TRUE(value);

    value = BitfieldLittleEndian::read<4>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<5>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<6>(data);
    EXPECT_TRUE(value);

    value = BitfieldLittleEndian::read<7>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<8>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<9>(data);
    EXPECT_FALSE(value);

    value = BitfieldLittleEndian::read<10>(data);
    EXPECT_TRUE(value);

    value = BitfieldLittleEndian::read<11>(data);
    EXPECT_TRUE(value);
}

TEST_F(BitfieldLittleEndianTest, readSingleByte)
{
    uint16_t value = BitfieldLittleEndian::read<0, 7>(data);
    EXPECT_EQ(0x12U, value);
}

TEST_F(BitfieldLittleEndianTest, readSingleByte2)
{
    uint16_t value = BitfieldLittleEndian::read<16, 23>(data);
    EXPECT_EQ(0x56U, value);
}

TEST_F(BitfieldLittleEndianTest, readoverThreeBytes)
{
    uint16_t value = BitfieldLittleEndian::read<4, 19>(data);

    EXPECT_EQ(0x4523U, value);
}

TEST_F(BitfieldLittleEndianTest, readoverFiveBytes)
{
    uint32_t value = BitfieldLittleEndian::read<4, 35>(data);
    EXPECT_EQ(0x89674523U, value);
}

TEST_F(BitfieldLittleEndianTest, readoverNineBytes)
{
    uint64_t value = BitfieldLittleEndian::read<4, 67>(data);
    EXPECT_EQ(0xF1DEBC0A89674523U, value);
}

TEST_F(BitfieldLittleEndianTest, readTwoBytesFromStart)
{
    uint16_t value = BitfieldLittleEndian::read<0, 15>(data);
    EXPECT_EQ(0x3412U, value);
}

TEST_F(BitfieldLittleEndianTest, readFourBytesFromStart)
{
    uint32_t value = BitfieldLittleEndian::read<0, 31>(data);
    EXPECT_EQ(0x78563412U, value);
}

TEST_F(BitfieldLittleEndianTest, readEightBytesFromStart)
{
    uint64_t value = BitfieldLittleEndian::read<0, 63>(data);
    EXPECT_EQ(0xEFCDAB9078563412U, value);
}

TEST_F(BitfieldLittleEndianTest, readTwoBytesWithOffset)
{
    uint16_t value = BitfieldLittleEndian::read<8, 23>(data);
    EXPECT_EQ(0x5634U, value);
}

TEST_F(BitfieldLittleEndianTest, readTwoBytesWithOffset2)
{
    uint8_t reference[4] = {0xFF, 0x00, 0x00, 0xFF};

    uint16_t value = BitfieldLittleEndian::read<8, 23>(reference);
    EXPECT_EQ(0x0000U, value);
}

TEST_F(BitfieldLittleEndianTest, readFourBytesWithOffset)
{
    uint32_t value = BitfieldLittleEndian::read<8, 39>(data);
    EXPECT_EQ(0x90785634U, value);
}

TEST_F(BitfieldLittleEndianTest, readEightBytesWithOffset)
{
    uint64_t value = BitfieldLittleEndian::read<8, 71>(data);
    EXPECT_EQ(0x12EFCDAB90785634U, value);
}

TEST_F(BitfieldLittleEndianTest, readShorterValue)
{
    uint16_t value = BitfieldLittleEndian::read<4, 7>(data);
    EXPECT_EQ(0x2U, value);
}

TEST_F(BitfieldLittleEndianTest, readValueOverlappingTwoBytes)
{
    uint16_t value = BitfieldLittleEndian::read<4, 11>(data);
    EXPECT_EQ(0x23U, value);
}

TEST_F(BitfieldLittleEndianTest, writeSingleByte)
{
    uint8_t buffer[8] = {};
    BitfieldLittleEndian::write<0, 7>(buffer, 0x12);
    EXPECT_EQ(0x12U, buffer[0]);
}

TEST_F(BitfieldLittleEndianTest, writeSingleByte2)
{
    uint8_t buffer[8] = {};
    BitfieldLittleEndian::write<16, 23>(buffer, 0x56);
    EXPECT_EQ(0x00U, buffer[0]);
    EXPECT_EQ(0x00U, buffer[1]);
    EXPECT_EQ(0x56U, buffer[2]);
    EXPECT_EQ(0x00U, buffer[3]);
}

TEST_F(BitfieldLittleEndianTest, writeTwoBytesFromStart)
{
    uint8_t buffer[8] = {};
    BitfieldLittleEndian::write<0, 15>(buffer, 0x1234);
    EXPECT_EQ(0x34U, buffer[0]);
    EXPECT_EQ(0x12U, buffer[1]);
}

TEST_F(BitfieldLittleEndianTest, writeTwoBytesWithOffset)
{
    uint8_t buffer[8] = {};
    BitfieldLittleEndian::write<8, 23>(buffer, 0x3456);
    EXPECT_EQ(0x56U, buffer[1]);
    EXPECT_EQ(0x34U, buffer[2]);
}

TEST_F(BitfieldLittleEndianTest, writeTwoBytesWithOffset2)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<8, 23>(buffer, 0);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0x00U, buffer[1]);
    EXPECT_EQ(0x00U, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldLittleEndianTest, writeIntoTwoBytes)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<13, 20>(buffer, 0x34);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0xF9U, buffer[1]);
    EXPECT_EQ(0xA7U, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldLittleEndianTest, writeIntoThreeBytes)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    volatile uint16_t test = 0x1234;

    BitfieldLittleEndian::write<4, 19>(buffer, test);
    EXPECT_EQ(0xF3U, buffer[0]);
    EXPECT_EQ(0x41U, buffer[1]);
    EXPECT_EQ(0x2FU, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldLittleEndianTest, writeIntoThreeBytes2)
{
    uint8_t buffer[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<5, 20>(buffer, 0x1234);
    EXPECT_EQ(0xF9U, buffer[0]);
    EXPECT_EQ(0xA0U, buffer[1]);
    EXPECT_EQ(0x97U, buffer[2]);
    EXPECT_EQ(0xFFU, buffer[3]);
}

TEST_F(BitfieldLittleEndianTest, writeShorterbuffer)
{
    uint8_t buffer[8] = {};
    BitfieldLittleEndian::write<4, 7>(buffer, 0x02);
    EXPECT_EQ(0x2U, buffer[0]);
}

TEST_F(BitfieldLittleEndianTest, writebufferOverlappingTwoBytes)
{
    uint8_t buffer[8] = {};
    BitfieldLittleEndian::write<4, 11>(buffer, 0x23);
    EXPECT_EQ(0x02U, buffer[0]);
    EXPECT_EQ(0x30U, buffer[1]);
}

TEST_F(BitfieldLittleEndianTest, shouldWriteWithBiggerOffset)
{
    uint8_t buffer[8] = {};
    BitfieldLittleEndian::write<41, 55>(buffer, 0x123);
    EXPECT_EQ(0x23U, buffer[5]);
    EXPECT_EQ(0x01U, buffer[6]);
}

TEST_F(BitfieldLittleEndianTest, writeFull4Bytes)
{
    uint8_t buffer[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<0, 31>(buffer, 0x12345678);
    EXPECT_EQ(0x78U, buffer[0]);
    EXPECT_EQ(0x56U, buffer[1]);
    EXPECT_EQ(0x34U, buffer[2]);
    EXPECT_EQ(0x12U, buffer[3]);
    EXPECT_EQ(0xFFU, buffer[4]);
}

TEST_F(BitfieldLittleEndianTest, writeFull4BytesWithByteOffset)
{
    uint8_t buffer[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<8, 39>(buffer, 0x12345678);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0x78U, buffer[1]);
    EXPECT_EQ(0x56U, buffer[2]);
    EXPECT_EQ(0x34U, buffer[3]);
    EXPECT_EQ(0x12U, buffer[4]);
}

TEST_F(BitfieldLittleEndianTest, writeInto5Bytes)
{
    uint8_t buffer[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<10, 38>(buffer, 0x12345678);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0xF0U, buffer[1]);
    EXPECT_EQ(0xACU, buffer[2]);
    EXPECT_EQ(0x68U, buffer[3]);
    EXPECT_EQ(0x25U, buffer[4]);
}

TEST_F(BitfieldLittleEndianTest, writeFull8Bytes)
{
    uint8_t buffer[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<0, 63>(buffer, 0x12345678deadbeef);
    EXPECT_EQ(0xEFU, buffer[0]);
    EXPECT_EQ(0xBEU, buffer[1]);
    EXPECT_EQ(0xADU, buffer[2]);
    EXPECT_EQ(0xDEU, buffer[3]);
    EXPECT_EQ(0x78U, buffer[4]);
    EXPECT_EQ(0x56U, buffer[5]);
    EXPECT_EQ(0x34U, buffer[6]);
    EXPECT_EQ(0x12U, buffer[7]);
    EXPECT_EQ(0xFFU, buffer[8]);
}

TEST_F(BitfieldLittleEndianTest, writeFull8BytesWithOffset)
{
    uint8_t buffer[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<8, 71>(buffer, 0x12345678deadbeef);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0xEFU, buffer[1]);
    EXPECT_EQ(0xBEU, buffer[2]);
    EXPECT_EQ(0xADU, buffer[3]);
    EXPECT_EQ(0xDEU, buffer[4]);
    EXPECT_EQ(0x78U, buffer[5]);
    EXPECT_EQ(0x56U, buffer[6]);
    EXPECT_EQ(0x34U, buffer[7]);
    EXPECT_EQ(0x12U, buffer[8]);
}

TEST_F(BitfieldLittleEndianTest, writeInto9Bytes)
{
    uint8_t buffer[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    BitfieldLittleEndian::write<10, 70>(buffer, 0x12345678deadbeef);
    EXPECT_EQ(0xFFU, buffer[0]);
    EXPECT_EQ(0xDFU, buffer[1]);
    EXPECT_EQ(0x7DU, buffer[2]);
    EXPECT_EQ(0x5BU, buffer[3]);
    EXPECT_EQ(0xBCU, buffer[4]);
    EXPECT_EQ(0xF0U, buffer[5]);
    EXPECT_EQ(0xACU, buffer[6]);
    EXPECT_EQ(0x68U, buffer[7]);
    EXPECT_EQ(0x25U, buffer[8]);
}
