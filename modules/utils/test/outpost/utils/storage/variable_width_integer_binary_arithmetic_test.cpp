/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/utils/storage/variable_width_integer.h>

#include <unittest/harness.h>

using namespace outpost::utils;

class VariableWidthIntBinaryArithemticTest : public testing::Test
{
};

TEST_F(VariableWidthIntBinaryArithemticTest, Int4BinaryOp)
{
    Integer<4> uut1 = 0xf;
    Integer<4> uut2 = 0x8;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    Integer<4> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x5;
    uut2 = 0x6;
    EXPECT_EQ(Integer<4>(0x4), uut1 & uut2);
    EXPECT_EQ(Integer<4>(0x4), uut2 & uut1);
    EXPECT_EQ(Integer<4>(0x7), uut1 | uut2);
    EXPECT_EQ(Integer<4>(0x7), uut2 | uut1);

    uut1 = 0x0;
    uut2 = 0x8;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, UInt4BinaryOp)
{
    UInteger<4> uut1 = 0xf;
    UInteger<4> uut2 = 0x8;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    UInteger<4> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x5;
    uut2 = 0x6;
    EXPECT_EQ(UInteger<4>(0x4), uut1 & uut2);
    EXPECT_EQ(UInteger<4>(0x4), uut2 & uut1);
    EXPECT_EQ(UInteger<4>(0x7), uut1 | uut2);
    EXPECT_EQ(UInteger<4>(0x7), uut2 | uut1);

    uut1 = 0x0;
    uut2 = 0x8;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, Int12BinaryOp)
{
    Integer<12> uut1 = 0xfff;
    Integer<12> uut2 = 0x85d;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    Integer<12> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A;
    uut2 = 0x668;
    EXPECT_EQ(Integer<12>(0x428), uut1 & uut2);
    EXPECT_EQ(Integer<12>(0x428), uut2 & uut1);
    EXPECT_EQ(Integer<12>(0x77A), uut1 | uut2);
    EXPECT_EQ(Integer<12>(0x77A), uut2 | uut1);

    uut1 = 0x000;
    uut2 = 0x85d;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, UInt12BinaryOp)
{
    UInteger<12> uut1 = 0xfff;
    UInteger<12> uut2 = 0x85d;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    UInteger<12> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A;
    uut2 = 0x668;
    EXPECT_EQ(UInteger<12>(0x428), uut1 & uut2);
    EXPECT_EQ(UInteger<12>(0x428), uut2 & uut1);
    EXPECT_EQ(UInteger<12>(0x77A), uut1 | uut2);
    EXPECT_EQ(UInteger<12>(0x77A), uut2 | uut1);

    uut1 = 0x000;
    uut2 = 0x85d;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, Int24BinaryOp)
{
    Integer<24> uut1 = 0xffffff;
    Integer<24> uut2 = 0x85d85d;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    Integer<24> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A;
    uut2 = 0x668668;
    EXPECT_EQ(Integer<24>(0x428428), uut1 & uut2);
    EXPECT_EQ(Integer<24>(0x428428), uut2 & uut1);
    EXPECT_EQ(Integer<24>(0x77A77A), uut1 | uut2);
    EXPECT_EQ(Integer<24>(0x77A77A), uut2 | uut1);

    uut1 = 0x00000;
    uut2 = 0x85d85d;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, UInt24BinaryOp)
{
    UInteger<24> uut1 = 0xffffff;
    UInteger<24> uut2 = 0x85d85d;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    UInteger<24> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A;
    uut2 = 0x668668;
    EXPECT_EQ(UInteger<24>(0x428428), uut1 & uut2);
    EXPECT_EQ(UInteger<24>(0x428428), uut2 & uut1);
    EXPECT_EQ(UInteger<24>(0x77A77A), uut1 | uut2);
    EXPECT_EQ(UInteger<24>(0x77A77A), uut2 | uut1);

    uut1 = 0x00000;
    uut2 = 0x85d85d;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, Int28BinaryOp)
{
    Integer<28> uut1 = 0xffffffF;
    Integer<28> uut2 = 0x85d85d2;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    Integer<28> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A5;
    uut2 = 0x6686686;
    EXPECT_EQ(Integer<28>(0x4284284), uut1 & uut2);
    EXPECT_EQ(Integer<28>(0x4284284), uut2 & uut1);
    EXPECT_EQ(Integer<28>(0x77A77A7), uut1 | uut2);
    EXPECT_EQ(Integer<28>(0x77A77A7), uut2 | uut1);

    uut1 = 0x000000;
    uut2 = 0x85d85d2;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, UInt28BinaryOp)
{
    UInteger<28> uut1 = 0xffffffF;
    UInteger<28> uut2 = 0x85d85d2;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    UInteger<28> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A5;
    uut2 = 0x6686686;
    EXPECT_EQ(UInteger<28>(0x4284284), uut1 & uut2);
    EXPECT_EQ(UInteger<28>(0x4284284), uut2 & uut1);
    EXPECT_EQ(UInteger<28>(0x77A77A7), uut1 | uut2);
    EXPECT_EQ(UInteger<28>(0x77A77A7), uut2 | uut1);

    uut1 = 0x000000;
    uut2 = 0x85d85d2;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, Int40BinaryOp)
{
    Integer<40> uut1 = 0xffffffffffL;
    Integer<40> uut2 = 0x85d85d2d85L;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    Integer<40> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A5347;
    uut2 = 0x6686686832;
    EXPECT_EQ(Integer<40>(0x4284284002L), uut1 & uut2);
    EXPECT_EQ(Integer<40>(0x4284284002L), uut2 & uut1);
    EXPECT_EQ(Integer<40>(0x77A77A7B77L), uut1 | uut2);
    EXPECT_EQ(Integer<40>(0x77A77A7B77L), uut2 | uut1);

    uut1 = 0x0000000000L;
    uut2 = 0x85d85d2295L;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, UInt40BinaryOp)
{
    UInteger<40> uut1 = 0xffffffffffL;
    UInteger<40> uut2 = 0x85d85d2d85L;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    UInteger<40> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A5347;
    uut2 = 0x6686686832;
    EXPECT_EQ(UInteger<40>(0x4284284002L), uut1 & uut2);
    EXPECT_EQ(UInteger<40>(0x4284284002L), uut2 & uut1);
    EXPECT_EQ(UInteger<40>(0x77A77A7B77L), uut1 | uut2);
    EXPECT_EQ(UInteger<40>(0x77A77A7B77L), uut2 | uut1);

    uut1 = 0x0000000000L;
    uut2 = 0x85d85d2295L;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, Int44BinaryOp)
{
    Integer<44> uut1 = 0xfffffffffffL;
    Integer<44> uut2 = 0x85d85d2d85cL;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    Integer<44> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A5347C;
    uut2 = 0x6686686832E;
    EXPECT_EQ(Integer<44>(0x4284284002CL), uut1 & uut2);
    EXPECT_EQ(Integer<44>(0x4284284002CL), uut2 & uut1);
    EXPECT_EQ(Integer<44>(0x77A77A7B77EL), uut1 | uut2);
    EXPECT_EQ(Integer<44>(0x77A77A7B77EL), uut2 | uut1);

    uut1 = 0x00000000000L;
    uut2 = 0x85d85d2295cL;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}

TEST_F(VariableWidthIntBinaryArithemticTest, UInt44BinaryOp)
{
    UInteger<44> uut1 = 0xfffffffffffL;
    UInteger<44> uut2 = 0x85d85d2d85cL;
    EXPECT_EQ(uut2, uut1 & uut2);
    EXPECT_EQ(uut2, uut2 & uut1);
    EXPECT_EQ(uut1, uut1 | uut2);
    EXPECT_EQ(uut1, uut2 | uut1);

    UInteger<44> uut3 = uut1;
    uut3 &= uut2;
    EXPECT_EQ(uut3, uut2);
    uut3 = uut1;
    uut3 |= uut2;
    EXPECT_EQ(uut3, uut1);

    uut1 = 0x53A53A5347C;
    uut2 = 0x6686686832E;
    EXPECT_EQ(UInteger<44>(0x4284284002CL), uut1 & uut2);
    EXPECT_EQ(UInteger<44>(0x4284284002CL), uut2 & uut1);
    EXPECT_EQ(UInteger<44>(0x77A77A7B77EL), uut1 | uut2);
    EXPECT_EQ(UInteger<44>(0x77A77A7B77EL), uut2 | uut1);

    uut1 = 0x00000000000L;
    uut2 = 0x85d85d2295cL;
    EXPECT_EQ(uut1, uut1 & uut2);
    EXPECT_EQ(uut1, uut2 & uut1);
    EXPECT_EQ(uut2, uut1 | uut2);
    EXPECT_EQ(uut2, uut2 | uut1);
}
