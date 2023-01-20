/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/utils/storage/variable_width_integer.h>

#include <unittest/harness.h>

using namespace outpost::utils;

class VariableWidthIntTest : public testing::Test
{
};

TEST_F(VariableWidthIntTest, testSize)
{
    EXPECT_EQ((Integer<2>::getBits()), 2u);
    EXPECT_EQ((Integer<7>::getBits()), 7u);
    EXPECT_EQ((Integer<8>::getBits()), 8u);
    EXPECT_EQ((Integer<12>::getBits()), 12u);
    EXPECT_EQ((Integer<16>::getBits()), 16u);
    EXPECT_EQ((Integer<20>::getBits()), 20u);
    EXPECT_EQ((Integer<32>::getBits()), 32u);
    EXPECT_EQ((Integer<40>::getBits()), 40u);
    EXPECT_EQ((Integer<64>::getBits()), 64u);

    EXPECT_EQ((UInteger<2>::getBits()), 2u);
    EXPECT_EQ((UInteger<7>::getBits()), 7u);
    EXPECT_EQ((UInteger<8>::getBits()), 8u);
    EXPECT_EQ((UInteger<12>::getBits()), 12u);
    EXPECT_EQ((UInteger<16>::getBits()), 16u);
    EXPECT_EQ((UInteger<20>::getBits()), 20u);
    EXPECT_EQ((UInteger<32>::getBits()), 32u);
    EXPECT_EQ((UInteger<40>::getBits()), 40u);
    EXPECT_EQ((UInteger<64>::getBits()), 64u);
}

TEST_F(VariableWidthIntTest, test1ByteSigned)
{
    int8_t i = -1;
    Integer<4> uut4(i);
    EXPECT_EQ(static_cast<int8_t>(uut4), i);

    i = 7;
    uut4 = i;
    EXPECT_EQ(static_cast<int8_t>(uut4), i);

    // test correct cut
    uut4 = (1 << 4) + 3;
    EXPECT_EQ(static_cast<int8_t>(uut4), 3);

    Integer<4> uut(i);
    uut4 = uut;
    EXPECT_EQ(static_cast<int8_t>(uut4), i);
}

TEST_F(VariableWidthIntTest, test2ByteSigned)
{
    int16_t i = -1;
    Integer<12> uut(i);
    EXPECT_EQ(static_cast<int16_t>(uut), i);

    i = 0x234;
    uut = i;
    EXPECT_EQ(static_cast<int16_t>(uut), i);

    // test correct cut
    uut = (1 << 12) + 3;
    EXPECT_EQ(static_cast<int16_t>(uut), 3);

    Integer<12> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<int16_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test3ByteSigned)
{
    int32_t i = -1;
    Integer<20> uut(i);
    EXPECT_EQ(static_cast<int32_t>(uut), i);

    i = 0x23456;
    uut = i;
    EXPECT_EQ(static_cast<int32_t>(uut), i);

    // test correct cut
    uut = (1 << 20) + 3;
    EXPECT_EQ(static_cast<int32_t>(uut), 3);

    Integer<20> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<int32_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test4ByteSigned)
{
    int32_t i = -1;
    Integer<28> uut(i);
    EXPECT_EQ(static_cast<int32_t>(uut), i);

    i = 0x2345678;
    uut = i;
    EXPECT_EQ(static_cast<int32_t>(uut), i);

    // test correct cut
    uut = (1 << 28) + 3;
    EXPECT_EQ(static_cast<int32_t>(uut), 3);

    Integer<28> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<int32_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test5ByteSigned)
{
    int64_t i = -1;
    Integer<36> uut(i);
    EXPECT_EQ(static_cast<int64_t>(uut), i);

    i = 0x23456789aL;
    uut = i;
    EXPECT_EQ(static_cast<int64_t>(uut), i);

    // test correct cut
    uut = (1L << 36) + 3;
    EXPECT_EQ(static_cast<int64_t>(uut), 3);

    Integer<36> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<int64_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test6ByteSigned)
{
    int64_t i = -1;
    Integer<44> uut(i);
    EXPECT_EQ(static_cast<int64_t>(uut), i);

    i = 0x23456789abcL;
    uut = i;
    EXPECT_EQ(static_cast<int64_t>(uut), i);

    // test correct cut
    uut = (1L << 44) + 3;
    EXPECT_EQ(static_cast<int64_t>(uut), 3);

    Integer<44> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<int64_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test7ByteSigned)
{
    int64_t i = -1;
    Integer<52> uut(i);
    EXPECT_EQ(static_cast<int64_t>(uut), i);

    i = 0x23456789abcdL;
    uut = i;
    EXPECT_EQ(static_cast<int64_t>(uut), i);

    // test correct cut
    uut = (1L << 52) + 3;
    EXPECT_EQ(static_cast<int64_t>(uut), 3);

    Integer<52> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<int64_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test1ByteUnsigned)
{
    uint8_t i = 1;
    UInteger<4> uut(i);
    EXPECT_EQ(static_cast<uint8_t>(uut), i);

    i = 0xf;
    uut = i;
    EXPECT_EQ(static_cast<uint8_t>(uut), i);

    // test correct cut
    uut = (1 << 4) + 3;
    EXPECT_EQ(static_cast<uint8_t>(uut), 3u);

    UInteger<4> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<uint8_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test2ByteUnsigned)
{
    uint16_t i = 1;
    UInteger<12> uut(i);
    EXPECT_EQ(static_cast<uint16_t>(uut), i);

    i = 0xf12;
    uut = i;
    EXPECT_EQ(static_cast<uint16_t>(uut), i);

    // test correct cut
    uut = (1 << 12) + 3;
    EXPECT_EQ(static_cast<uint16_t>(uut), 3u);

    UInteger<12> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<uint16_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test3ByteUnsigned)
{
    uint32_t i = 1;
    UInteger<20> uut(i);
    EXPECT_EQ(static_cast<uint32_t>(uut), i);

    i = 0xf3456;
    uut = i;
    EXPECT_EQ(static_cast<uint32_t>(uut), i);

    // test correct cut
    uut = (1 << 20) + 3;
    EXPECT_EQ(static_cast<uint32_t>(uut), 3u);

    UInteger<20> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<uint32_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test4ByteUnsigned)
{
    uint32_t i = 1;
    UInteger<28> uut(i);
    EXPECT_EQ(static_cast<uint32_t>(uut), i);

    i = 0xf345678;
    uut = i;
    EXPECT_EQ(static_cast<uint32_t>(uut), i);

    // test correct cut
    uut = (1 << 28) + 3;
    EXPECT_EQ(static_cast<uint32_t>(uut), 3u);

    UInteger<28> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<uint32_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test5ByteUnsigned)
{
    uint64_t i = 1;
    UInteger<36> uut(i);
    EXPECT_EQ(static_cast<uint64_t>(uut), i);

    i = 0x23456789aL;
    uut = i;
    EXPECT_EQ(static_cast<uint64_t>(uut), i);

    // test correct cut
    uut = (1L << 36) + 3;
    EXPECT_EQ(static_cast<uint64_t>(uut), 3u);

    UInteger<36> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<uint64_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test6ByteUnsigned)
{
    uint64_t i = 1;
    UInteger<44> uut(i);
    EXPECT_EQ(static_cast<uint64_t>(uut), i);

    i = 0xf3456789abcL;
    uut = i;
    EXPECT_EQ(static_cast<uint64_t>(uut), i);

    // test correct cut
    uut = (1L << 44) + 3;
    EXPECT_EQ(static_cast<uint64_t>(uut), 3u);

    UInteger<44> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<uint64_t>(uut), i);
}

TEST_F(VariableWidthIntTest, test7ByteUnsigned)
{
    uint64_t i = 1;
    UInteger<52> uut(i);
    EXPECT_EQ(static_cast<uint64_t>(uut), i);

    i = 0xf3456789abcdL;
    uut = i;
    EXPECT_EQ(static_cast<uint64_t>(uut), i);

    // test correct cut
    uut = (1L << 52) + 3;
    EXPECT_EQ(static_cast<uint64_t>(uut), 3u);

    UInteger<52> uut2(i);
    uut = uut2;
    EXPECT_EQ(static_cast<uint64_t>(uut), i);
}

TEST_F(VariableWidthIntTest, typeTraitsTest)
{
    EXPECT_FALSE((is_variable_width_integer<VariableWidthIntTest>::value));
    EXPECT_FALSE((is_variable_width_unsigned_integer<VariableWidthIntTest>::value));
    EXPECT_FALSE((is_variable_width_signed_integer<VariableWidthIntTest>::value));

    EXPECT_FALSE((is_variable_width_signed_integer<UInteger<52>>::value));
    EXPECT_TRUE((is_variable_width_unsigned_integer<UInteger<52>>::value));
    EXPECT_TRUE((is_variable_width_integer<UInteger<52>>::value));

    EXPECT_TRUE((is_variable_width_signed_integer<Integer<52>>::value));
    EXPECT_FALSE((is_variable_width_unsigned_integer<Integer<52>>::value));
    EXPECT_TRUE((is_variable_width_integer<Integer<52>>::value));

    EXPECT_FALSE((is_variable_width_integer<int>::value));
    EXPECT_FALSE((is_variable_width_unsigned_integer<unsigned int>::value));
    EXPECT_FALSE((is_variable_width_signed_integer<signed int>::value));
}
