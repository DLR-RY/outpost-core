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

class VariableWidthIntRelationalTest : public testing::Test
{
};

TEST_F(VariableWidthIntRelationalTest, relationInt4)
{
    // equal negative
    int8_t i = -1;
    Integer<4> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // Negative and zero
    uint8_t i2 = 0;
    Integer<4> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both positive
    i = 1;
    i2 = 5;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both negative
    i = -3;
    i2 = -1;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 3;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}

TEST_F(VariableWidthIntRelationalTest, relationInt12)
{
    // equal negative
    int16_t i = -512;
    Integer<12> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // Negative and zero
    uint16_t i2 = 0;
    Integer<12> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both positive
    i = 600;
    i2 = 1200;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both negative
    i = -1500;
    i2 = -1;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 1500;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}

TEST_F(VariableWidthIntRelationalTest, relationInt24)
{
    // equal negative
    int32_t i = -512000;
    Integer<24> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // Negative and zero
    uint32_t i2 = 0;
    Integer<24> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both positive
    i = 60000;
    i2 = 1200000;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both negative
    i = -1500000;
    i2 = -10;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 2500000;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}

TEST_F(VariableWidthIntRelationalTest, relationInt63)
{
    // equal negative
    int64_t i = -2305843009213693952L;
    Integer<63> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // Negative and zero
    uint64_t i2 = 0L;
    Integer<63> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both positive
    i = 2305843009213693950L;
    i2 = 2305843009213693951L;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // both negative
    i = -2305843009213693952L;
    i2 = -10;

    uut = i;
    uut2 = i2;

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 2305843009213693951L;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}

TEST_F(VariableWidthIntRelationalTest, relationUInt4)
{
    // equal zero
    uint8_t i = 0;
    UInteger<4> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // both positive
    i = 1;
    uint8_t i2 = 5;

    uut = i;
    UInteger<4> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 3;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}

TEST_F(VariableWidthIntRelationalTest, relationUInt12)
{
    // equal zero
    uint16_t i = 0;
    UInteger<12> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // both positive
    i = 1;
    uint16_t i2 = 4095;

    uut = i;
    UInteger<12> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 3000;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}

TEST_F(VariableWidthIntRelationalTest, relationUInt23)
{
    // equal zero
    uint32_t i = 0;
    UInteger<23> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // both positive
    i = 1;
    uint32_t i2 = 8388607;

    uut = i;
    UInteger<23> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 8388600;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}

TEST_F(VariableWidthIntRelationalTest, relationUInt63)
{
    // equal zero
    uint64_t i = 0;
    UInteger<63> uut(i);
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);

    // both positive
    i = 1;
    uint64_t i2 = 9223372036854775800L;

    uut = i;
    UInteger<63> uut2(i2);

    EXPECT_EQ(uut2 == uut, false);
    EXPECT_EQ(uut == uut2, false);
    EXPECT_EQ(uut == i2, false);
    EXPECT_EQ(i2 == uut, false);

    EXPECT_EQ(uut2 != uut, true);
    EXPECT_EQ(uut != uut2, true);
    EXPECT_EQ(uut != i2, true);
    EXPECT_EQ(i2 != uut, true);

    EXPECT_EQ(uut < uut2, true);
    EXPECT_EQ(uut2 < uut, false);
    EXPECT_EQ(uut < i2, true);
    EXPECT_EQ(i2 < uut, false);

    EXPECT_EQ(uut <= uut2, true);
    EXPECT_EQ(uut2 <= uut, false);
    EXPECT_EQ(uut <= i2, true);
    EXPECT_EQ(i2 <= uut, false);

    EXPECT_EQ(uut >= uut2, false);
    EXPECT_EQ(uut2 >= uut, true);
    EXPECT_EQ(uut >= i2, false);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut2, false);
    EXPECT_EQ(uut2 > uut, true);
    EXPECT_EQ(uut > i2, false);
    EXPECT_EQ(i2 > uut, true);

    // equal positive
    i = 9223372036854775802L;
    uut = i;
    EXPECT_EQ(uut == uut, true);
    EXPECT_EQ(uut == i, true);
    EXPECT_EQ(i == uut, true);

    EXPECT_EQ(uut != uut, false);
    EXPECT_EQ(uut != i, false);
    EXPECT_EQ(i != uut, false);

    EXPECT_EQ(uut < uut, false);
    EXPECT_EQ(uut < i, false);
    EXPECT_EQ(i < uut, false);

    EXPECT_EQ(uut <= uut, true);
    EXPECT_EQ(uut <= i, true);
    EXPECT_EQ(i <= uut, true);

    EXPECT_EQ(uut >= uut, true);
    EXPECT_EQ(uut >= i, true);
    EXPECT_EQ(i >= uut, true);

    EXPECT_EQ(uut > uut, false);
    EXPECT_EQ(uut > i, false);
    EXPECT_EQ(i > uut, false);
}
