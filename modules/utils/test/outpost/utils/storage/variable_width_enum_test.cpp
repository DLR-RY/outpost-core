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

#include <outpost/utils/storage/variable_width_enum.h>

#include <unittest/harness.h>

using namespace outpost::utils;

enum class small
{
    one,
    two,
    three,
    four
};

enum class big : uint64_t
{
    a = 0x10000000L,
    b = 0x100000000L,
    c = 0x200000000L,
    d = 0x1ffffffffL,
};

class VariableWidthEnumTest : public testing::Test
{
};

TEST_F(VariableWidthEnumTest, smallTest)
{
    small e = small::one;
    Enumeration<small, 2> uut(e);

    EXPECT_EQ((Enumeration<small, 2>::getBits()), 2u);

    EXPECT_EQ(static_cast<small>(uut), e);

    e = small::two;
    uut = e;
    EXPECT_EQ(static_cast<small>(uut), e);

    e = small::three;
    uut = e;
    EXPECT_EQ(static_cast<small>(uut), e);

    e = small::four;
    uut = e;
    EXPECT_EQ(static_cast<small>(uut), e);
}

TEST_F(VariableWidthEnumTest, defaultTest)
{
    small e = small::one;
    Enumeration<small> uut(e);

    EXPECT_EQ(Enumeration<small>::getBits(), 32u);

    EXPECT_EQ(static_cast<small>(uut), e);

    e = small::two;
    uut = e;
    EXPECT_EQ(static_cast<small>(uut), e);

    e = small::three;
    uut = e;
    EXPECT_EQ(static_cast<small>(uut), e);

    e = small::four;
    uut = e;
    EXPECT_EQ(static_cast<small>(uut), e);
}

TEST_F(VariableWidthEnumTest, bigTest)
{
    big e = big::a;
    Enumeration<big, 34> uut(e);
    EXPECT_EQ((Enumeration<big, 34>::getBits()), 34u);

    EXPECT_EQ(static_cast<big>(uut), e);

    e = big::b;
    uut = e;
    EXPECT_EQ(static_cast<big>(uut), e);

    e = big::c;
    uut = e;
    EXPECT_EQ(static_cast<big>(uut), e);
}

TEST_F(VariableWidthEnumTest, compareTest)
{
    Enumeration<big, 34> uut1(big::a);
    Enumeration<big, 34> uut2(big::a);
    Enumeration<big, 34> uut3(big::b);

    EXPECT_TRUE(uut1 == uut2);
    EXPECT_TRUE(uut1 == uut1);
    EXPECT_TRUE(uut1 != uut3);

    EXPECT_FALSE(uut1 != uut2);
    EXPECT_FALSE(uut1 != uut1);
    EXPECT_FALSE(uut1 == uut3);
}

TEST_F(VariableWidthEnumTest, rawTanstation)
{
    UInteger<6> value = 10;
    Enumeration<big, 6> uut(value);

    EXPECT_EQ(static_cast<UInteger<6>>(uut), value);
}

TEST_F(VariableWidthEnumTest, cuts)
{
    Enumeration<big, 3> uut(big::d);

    big value = static_cast<big>(uut);
    EXPECT_EQ(static_cast<uint64_t>(value), 0x7u);
}

TEST_F(VariableWidthEnumTest, typeTestTest)
{
    EXPECT_FALSE(is_variable_width_enumeration<VariableWidthEnumTest>::value);
    EXPECT_FALSE(is_variable_width_enumeration<int>::value);
    EXPECT_FALSE(is_variable_width_enumeration<big>::value);
    EXPECT_TRUE((is_variable_width_enumeration<Enumeration<big, 6>>::value));
    EXPECT_TRUE((is_variable_width_enumeration<Enumeration<small>>::value));
}
