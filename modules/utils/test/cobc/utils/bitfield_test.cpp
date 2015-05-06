/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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

#include <unittest/harness.h>

#include <cobc/utils/bitfield.h>

using cobc::Bitfield;

class BitfieldTest : public testing::Test
{
public:
    BitfieldTest() :
        data { 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xBC, 0xDE }
    {
    }

    uint8_t data[8];
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

TEST_F(BitfieldTest, readTwoBytesFromStart)
{
    uint16_t value = Bitfield::read<0, 15>(data);
    EXPECT_EQ(0x1234U, value);
}

TEST_F(BitfieldTest, readTwoBytesWithOffset)
{
    uint16_t value = Bitfield::read<8, 23>(data);
    EXPECT_EQ(0x3456U, value);
}

TEST_F(BitfieldTest, readTwoBytesWithOffset2)
{
    uint8_t reference[4] = { 0xFF, 0x00, 0x00, 0xFF };

    uint16_t value = Bitfield::read<8, 23>(reference);
    EXPECT_EQ(0x0000U, value);
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
