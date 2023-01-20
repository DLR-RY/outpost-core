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

class VariableWidthIntArithemticTest : public testing::Test
{
};

TEST_F(VariableWidthIntArithemticTest, Int4Increment)
{
    // zero -> +
    Integer<4> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 7;
    EXPECT_EQ(static_cast<int8_t>(++uut), -8);
    EXPECT_EQ(uut, -8);

    // negative -> zero
    uut = -1;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(++uut, 4);
    EXPECT_EQ(uut, 4);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(++uut, -2);
    EXPECT_EQ(uut, -2);
}

TEST_F(VariableWidthIntArithemticTest, Int4Decrement)
{
    // + -> zero
    Integer<4> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = -8;
    EXPECT_EQ(--uut, 7);
    EXPECT_EQ(uut, 7);

    // zero -> neg
    uut = 0;
    EXPECT_EQ(--uut, -1);
    EXPECT_EQ(uut, -1);

    // pos -> pos
    uut = 4;
    EXPECT_EQ(--uut, 3);
    EXPECT_EQ(uut, 3);

    // neg -> neg
    uut = -2;
    EXPECT_EQ(--uut, -3);
    EXPECT_EQ(uut, -3);
}

TEST_F(VariableWidthIntArithemticTest, Int12Increment)
{
    // zero -> +
    Integer<12> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 2047;
    EXPECT_EQ(++uut, -2048);
    EXPECT_EQ(uut, -2048);

    // negative -> zero
    uut = -1;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 1500;
    EXPECT_EQ(++uut, 1501);
    EXPECT_EQ(uut, 1501);

    // neg -> neg
    uut = -1500;
    EXPECT_EQ(++uut, -1499);
    EXPECT_EQ(uut, -1499);
}

TEST_F(VariableWidthIntArithemticTest, Int12Decrement)
{
    // + -> zero
    Integer<12> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = -2048;
    EXPECT_EQ(--uut, 2047);
    EXPECT_EQ(uut, 2047);

    // zero -> neg
    uut = 0;
    EXPECT_EQ(--uut, -1);
    EXPECT_EQ(uut, -1);

    // pos -> pos
    uut = 1501;
    EXPECT_EQ(--uut, 1500);
    EXPECT_EQ(uut, 1500);

    // neg -> neg
    uut = -170;
    EXPECT_EQ(--uut, -171);
    EXPECT_EQ(uut, -171);
}

TEST_F(VariableWidthIntArithemticTest, Int24Increment)
{
    // zero -> +
    Integer<24> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 8388607;
    EXPECT_EQ(++uut, -8388608);
    EXPECT_EQ(uut, -8388608);

    // negative -> zero
    uut = -1;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 1500;
    EXPECT_EQ(++uut, 1501);
    EXPECT_EQ(uut, 1501);

    // neg -> neg
    uut = -1500;
    EXPECT_EQ(++uut, -1499);
    EXPECT_EQ(uut, -1499);
}

TEST_F(VariableWidthIntArithemticTest, Int24Decrement)
{
    // + -> zero
    Integer<24> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = -8388608;
    EXPECT_EQ(--uut, 8388607);
    EXPECT_EQ(uut, 8388607);

    // zero -> neg
    uut = 0;
    EXPECT_EQ(--uut, -1);
    EXPECT_EQ(uut, -1);

    // pos -> pos
    uut = 1501;
    EXPECT_EQ(--uut, 1500);
    EXPECT_EQ(uut, 1500);

    // neg -> neg
    uut = -17000;
    EXPECT_EQ(--uut, -17001);
    EXPECT_EQ(uut, -17001);
}

TEST_F(VariableWidthIntArithemticTest, Int28Increment)
{
    // zero -> +
    Integer<28> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 134217727;
    EXPECT_EQ(++uut, -134217728);
    EXPECT_EQ(uut, -134217728);

    // negative -> zero
    uut = -1;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 1500;
    EXPECT_EQ(++uut, 1501);
    EXPECT_EQ(uut, 1501);

    // neg -> neg
    uut = -1500;
    EXPECT_EQ(++uut, -1499);
    EXPECT_EQ(uut, -1499);
}

TEST_F(VariableWidthIntArithemticTest, Int28Decrement)
{
    // + -> zero
    Integer<28> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = -134217728;
    EXPECT_EQ(--uut, 134217727);
    EXPECT_EQ(uut, 134217727);

    // zero -> neg
    uut = 0;
    EXPECT_EQ(--uut, -1);
    EXPECT_EQ(uut, -1);

    // pos -> pos
    uut = 1501;
    EXPECT_EQ(--uut, 1500);
    EXPECT_EQ(uut, 1500);

    // neg -> neg
    uut = -17000;
    EXPECT_EQ(--uut, -17001);
    EXPECT_EQ(uut, -17001);
}

TEST_F(VariableWidthIntArithemticTest, Int50Increment)
{
    // zero -> +
    Integer<50> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 562949953421311L;
    EXPECT_EQ(static_cast<int64_t>(++uut), -562949953421312L);
    EXPECT_EQ(uut, -562949953421312L);

    // negative -> zero
    uut = -1;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 1500;
    EXPECT_EQ(++uut, 1501);
    EXPECT_EQ(uut, 1501);

    // neg -> neg
    uut = -1500;
    EXPECT_EQ(++uut, -1499);
    EXPECT_EQ(uut, -1499);
}

TEST_F(VariableWidthIntArithemticTest, Int50Decrement)
{
    // + -> zero
    Integer<50> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = -562949953421312L;
    EXPECT_EQ(--uut, 562949953421311L);
    EXPECT_EQ(uut, 562949953421311L);

    // zero -> neg
    uut = 0;
    EXPECT_EQ(--uut, -1);
    EXPECT_EQ(uut, -1);

    // pos -> pos
    uut = 1501234561L;
    EXPECT_EQ(--uut, 1501234560L);
    EXPECT_EQ(uut, 1501234560L);

    // neg -> neg
    uut = -1700456000L;
    EXPECT_EQ(--uut, -1700456001L);
    EXPECT_EQ(uut, -1700456001L);
}

TEST_F(VariableWidthIntArithemticTest, Int56Increment)
{
    // zero -> +
    Integer<56> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 36028797018963967L;
    EXPECT_EQ(static_cast<int64_t>(++uut), -36028797018963968L);
    EXPECT_EQ(uut, -36028797018963968L);

    // negative -> zero
    uut = -1;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 1500;
    EXPECT_EQ(++uut, 1501);
    EXPECT_EQ(uut, 1501);

    // neg -> neg
    uut = -1500;
    EXPECT_EQ(++uut, -1499);
    EXPECT_EQ(uut, -1499);
}

TEST_F(VariableWidthIntArithemticTest, Int56Decrement)
{
    // + -> zero
    Integer<56> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = -36028797018963968L;
    EXPECT_EQ(--uut, 36028797018963967L);
    EXPECT_EQ(uut, 36028797018963967L);

    // zero -> neg
    uut = 0;
    EXPECT_EQ(--uut, -1);
    EXPECT_EQ(uut, -1);

    // pos -> pos
    uut = 1501234561L;
    EXPECT_EQ(--uut, 1501234560L);
    EXPECT_EQ(uut, 1501234560L);

    // neg -> neg
    uut = -1700456000L;
    EXPECT_EQ(--uut, -1700456001L);
    EXPECT_EQ(uut, -1700456001L);
}

TEST_F(VariableWidthIntArithemticTest, UInt4Increment)
{
    // zero -> +
    UInteger<4> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 15;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(++uut, 4);
    EXPECT_EQ(uut, 4);
}

TEST_F(VariableWidthIntArithemticTest, UInt4Decrement)
{
    // + -> 0
    UInteger<4> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 0;
    EXPECT_EQ(--uut, 15);
    EXPECT_EQ(uut, 15);

    // pos -> pos
    uut = 4;
    EXPECT_EQ(--uut, 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt12Increment)
{
    // zero -> +
    UInteger<12> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 4095;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 3456;
    EXPECT_EQ(++uut, 3457);
    EXPECT_EQ(uut, 3457);
}

TEST_F(VariableWidthIntArithemticTest, UInt12Decrement)
{
    // + -> 0
    UInteger<12> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 0;
    EXPECT_EQ(--uut, 4095);
    EXPECT_EQ(uut, 4095);

    // pos -> pos
    uut = 3456;
    EXPECT_EQ(--uut, 3455);
    EXPECT_EQ(uut, 3455);
}

TEST_F(VariableWidthIntArithemticTest, UInt24Increment)
{
    // zero -> +
    UInteger<24> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 16777215;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 123456;
    EXPECT_EQ(++uut, 123457);
    EXPECT_EQ(uut, 123457);
}

TEST_F(VariableWidthIntArithemticTest, UInt24Decrement)
{
    // + -> 0
    UInteger<24> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 0;
    EXPECT_EQ(--uut, 16777215);
    EXPECT_EQ(uut, 16777215);

    // pos -> pos
    uut = 123457;
    EXPECT_EQ(--uut, 123456);
    EXPECT_EQ(uut, 123456);
}

TEST_F(VariableWidthIntArithemticTest, UInt28Increment)
{
    // zero -> +
    UInteger<28> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 268435455;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 168435455;
    EXPECT_EQ(++uut, 168435456);
    EXPECT_EQ(uut, 168435456);
}

TEST_F(VariableWidthIntArithemticTest, UInt28Decrement)
{
    // + -> 0
    UInteger<28> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 0;
    EXPECT_EQ(--uut, 268435455);
    EXPECT_EQ(uut, 268435455);

    // pos -> pos
    uut = 168435455;
    EXPECT_EQ(--uut, 168435454);
    EXPECT_EQ(uut, 168435454);
}

TEST_F(VariableWidthIntArithemticTest, UInt50Increment)
{
    // zero -> +
    UInteger<50> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 1125899906842623L;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 125899906842622L;
    EXPECT_EQ(++uut, 125899906842623L);
    EXPECT_EQ(uut, 125899906842623L);
}

TEST_F(VariableWidthIntArithemticTest, UInt50Decrement)
{
    // + -> 0
    UInteger<50> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 0;
    EXPECT_EQ(--uut, 1125899906842623L);
    EXPECT_EQ(uut, 1125899906842623L);

    // pos -> pos
    uut = 125899906842623L;
    EXPECT_EQ(--uut, 125899906842622L);
    EXPECT_EQ(uut, 125899906842622L);
}

TEST_F(VariableWidthIntArithemticTest, UInt56Increment)
{
    // zero -> +
    UInteger<56> uut = 0;
    EXPECT_EQ(++uut, 1);
    EXPECT_EQ(uut, 1);

    // overflow
    uut = 72057594037927935L;
    EXPECT_EQ(++uut, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 7205759403792793L;
    EXPECT_EQ(++uut, 7205759403792794L);
    EXPECT_EQ(uut, 7205759403792794L);
}

TEST_F(VariableWidthIntArithemticTest, UInt56Decrement)
{
    // + -> 0
    UInteger<56> uut = 1;
    EXPECT_EQ(--uut, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 0;
    EXPECT_EQ(--uut, 72057594037927935L);
    EXPECT_EQ(uut, 72057594037927935L);

    // pos -> pos
    uut = 7205759403792793L;
    EXPECT_EQ(--uut, 7205759403792792L);
    EXPECT_EQ(uut, 7205759403792792L);
}

TEST_F(VariableWidthIntArithemticTest, Int4AddAssignPrimitivPos)
{
    // zero -> +
    Integer<4> uut = 0;
    EXPECT_EQ(uut += 2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 6;
    EXPECT_EQ(uut += 2, -8);
    EXPECT_EQ(uut, -8);

    // negative -> zero
    uut = -3;
    EXPECT_EQ(uut += 3, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += 3, 6);
    EXPECT_EQ(uut, 6);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut += 2, -1);
    EXPECT_EQ(uut, -1);
}

TEST_F(VariableWidthIntArithemticTest, Int4AddAssignPrimitivNeg)
{
    // zero -> -
    Integer<4> uut = 0;
    EXPECT_EQ(uut += -2, -2);
    EXPECT_EQ(uut, -2);

    // overflow
    uut = -8;
    EXPECT_EQ(uut += -2, 6);
    EXPECT_EQ(uut, 6);

    // pos -> zero
    uut = 3;
    EXPECT_EQ(uut += -3, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut += -3, 3);
    EXPECT_EQ(uut, 3);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut += -2, -5);
    EXPECT_EQ(uut, -5);
}

TEST_F(VariableWidthIntArithemticTest, Int4AddAssignOtherPos)
{
    // zero -> +
    Integer<4> uut = 0;
    EXPECT_EQ(uut += Integer<4>(2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 6;
    EXPECT_EQ(uut += Integer<4>(2), -8);
    EXPECT_EQ(uut, -8);

    // negative -> zero
    uut = -3;
    EXPECT_EQ(uut += Integer<4>(3), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += Integer<4>(3), 6);
    EXPECT_EQ(uut, 6);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut += Integer<4>(2), -1);
    EXPECT_EQ(uut, -1);
}

TEST_F(VariableWidthIntArithemticTest, Int4AddAssignOtherNeg)
{
    // zero -> -
    Integer<4> uut = 0;
    EXPECT_EQ(uut += Integer<4>(-2), -2);
    EXPECT_EQ(uut, -2);

    // overflow
    uut = -8;
    EXPECT_EQ(uut += Integer<4>(-2), 6);
    EXPECT_EQ(uut, 6);

    // pos -> zero
    uut = 3;
    EXPECT_EQ(uut += Integer<4>(-3), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut += Integer<4>(-3), 3);
    EXPECT_EQ(uut, 3);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut += Integer<4>(-2), -5);
    EXPECT_EQ(uut, -5);
}

TEST_F(VariableWidthIntArithemticTest, Int4SubAssignPrimitivNeg)
{
    // zero -> +
    Integer<4> uut = 0;
    EXPECT_EQ(uut -= -2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 6;
    EXPECT_EQ(uut -= -2, -8);
    EXPECT_EQ(uut, -8);

    // negative -> zero
    uut = -3;
    EXPECT_EQ(uut -= -3, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut -= -3, 6);
    EXPECT_EQ(uut, 6);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut -= -2, -1);
    EXPECT_EQ(uut, -1);
}

TEST_F(VariableWidthIntArithemticTest, Int4SubAssignPrimitivPos)
{
    // zero -> -
    Integer<4> uut = 0;
    EXPECT_EQ(uut -= 2, -2);
    EXPECT_EQ(uut, -2);

    // overflow
    uut = -8;
    EXPECT_EQ(uut -= 2, 6);
    EXPECT_EQ(uut, 6);

    // pos -> zero
    uut = 3;
    EXPECT_EQ(uut -= 3, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= 3, 3);
    EXPECT_EQ(uut, 3);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut -= 2, -5);
    EXPECT_EQ(uut, -5);
}

TEST_F(VariableWidthIntArithemticTest, Int4SubAssignOtherNeg)
{
    // zero -> +
    Integer<4> uut = 0;
    EXPECT_EQ(uut -= Integer<4>(-2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 6;
    EXPECT_EQ(uut -= Integer<4>(-2), -8);
    EXPECT_EQ(uut, -8);

    // negative -> zero
    uut = -3;
    EXPECT_EQ(uut -= Integer<4>(-3), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut -= Integer<4>(-3), 6);
    EXPECT_EQ(uut, 6);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut -= Integer<4>(-2), -1);
    EXPECT_EQ(uut, -1);
}

TEST_F(VariableWidthIntArithemticTest, Int4SubAssignOtherPos)
{
    // zero -> -
    Integer<4> uut = 0;
    EXPECT_EQ(uut -= Integer<4>(2), -2);
    EXPECT_EQ(uut, -2);

    // overflow
    uut = -8;
    EXPECT_EQ(uut -= Integer<4>(2), 6);
    EXPECT_EQ(uut, 6);

    // pos -> zero
    uut = 3;
    EXPECT_EQ(uut -= Integer<4>(3), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= Integer<4>(3), 3);
    EXPECT_EQ(uut, 3);

    // neg -> neg
    uut = -3;
    EXPECT_EQ(uut -= Integer<4>(2), -5);
    EXPECT_EQ(uut, -5);
}

TEST_F(VariableWidthIntArithemticTest, Int12AddAssignPrimitivPos)
{
    // zero -> +
    Integer<12> uut = 0;
    EXPECT_EQ(uut += 200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 2046;
    EXPECT_EQ(uut += 2, -2048);
    EXPECT_EQ(uut, -2048);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += 300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += 200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int12AddAssignPrimitivNeg)
{
    // zero -> -
    Integer<12> uut = 0;
    EXPECT_EQ(uut += -200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -2048;
    EXPECT_EQ(uut += -2, 2046);
    EXPECT_EQ(uut, 2046);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += -300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += -200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int12AddAssignOtherPos)
{
    // zero -> +
    Integer<12> uut = 0;
    EXPECT_EQ(uut += Integer<12>(200), 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 2046;
    EXPECT_EQ(uut += Integer<12>(2), -2048);
    EXPECT_EQ(uut, -2048);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += Integer<12>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += Integer<12>(300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<12>(200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int12AddAssignOtherNeg)
{
    // zero -> -
    Integer<12> uut = 0;
    EXPECT_EQ(uut += Integer<12>(-200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -2048;
    EXPECT_EQ(uut += Integer<12>(-2), 2046);
    EXPECT_EQ(uut, 2046);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += Integer<12>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += Integer<12>(-300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<12>(-200), -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int12SubAssignPrimitivNeg)
{
    // zero -> +
    Integer<12> uut = 0;
    EXPECT_EQ(uut -= -200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 2047;
    EXPECT_EQ(uut -= -2, -2047);
    EXPECT_EQ(uut, -2047);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= -300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= -200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int12SubAssignPrimitivPos)
{
    // zero -> -
    Integer<12> uut = 0;
    EXPECT_EQ(uut -= 200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -2048;
    EXPECT_EQ(uut -= 2, 2046);
    EXPECT_EQ(uut, 2046);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= 300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= 200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int12SubAssignOtherNeg)
{
    // zero -> +
    Integer<12> uut = 0;
    EXPECT_EQ(uut -= Integer<12>(-2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 2047;
    EXPECT_EQ(uut -= Integer<12>(-2), -2047);
    EXPECT_EQ(uut, -2047);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= Integer<12>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= Integer<12>(-300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= Integer<12>(-200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int12SubAssignOtherPos)
{
    // zero -> -
    Integer<12> uut = 0;
    EXPECT_EQ(uut -= Integer<12>(200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -2048;
    EXPECT_EQ(uut -= Integer<12>(2), 2046);
    EXPECT_EQ(uut, 2046);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= Integer<12>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= Integer<12>(300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= Integer<12>(200), -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int24AddAssignPrimitivPos)
{
    // zero -> +
    Integer<24> uut = 0;
    EXPECT_EQ(uut += 200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 8388606;
    EXPECT_EQ(uut += 2, -8388608);
    EXPECT_EQ(uut, -8388608);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += 300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += 200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int24AddAssignPrimitivNeg)
{
    // zero -> -
    Integer<24> uut = 0;
    EXPECT_EQ(uut += -200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -8388608;
    EXPECT_EQ(uut += -2, 8388606);
    EXPECT_EQ(uut, 8388606);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += -300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += -200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int24AddAssignOtherPos)
{
    // zero -> +
    Integer<24> uut = 0;
    EXPECT_EQ(uut += Integer<24>(200), 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 8388606;
    EXPECT_EQ(uut += Integer<24>(2), -8388608);
    EXPECT_EQ(uut, -8388608);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += Integer<24>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += Integer<24>(300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<24>(200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int24AddAssignOtherNeg)
{
    // zero -> -
    Integer<24> uut = 0;
    EXPECT_EQ(uut += Integer<24>(-200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -8388608;
    EXPECT_EQ(uut += Integer<24>(-2), 8388606);
    EXPECT_EQ(uut, 8388606);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += Integer<24>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += Integer<24>(-300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<24>(-200), -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int24SubAssignPrimitivNeg)
{
    // zero -> +
    Integer<24> uut = 0;
    EXPECT_EQ(uut -= -200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 8388606;
    EXPECT_EQ(uut -= -2, -8388608);
    EXPECT_EQ(uut, -8388608);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= -300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= -200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int24SubAssignPrimitivPos)
{
    // zero -> -
    Integer<24> uut = 0;
    EXPECT_EQ(uut -= 200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -8388608;
    EXPECT_EQ(uut -= 2, 8388606);
    EXPECT_EQ(uut, 8388606);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= 300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= 200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int24SubAssignOtherNeg)
{
    // zero -> +
    Integer<24> uut = 0;
    EXPECT_EQ(uut -= Integer<24>(-2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 8388606;
    EXPECT_EQ(uut -= Integer<24>(-2), -8388608);
    EXPECT_EQ(uut, -8388608);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= Integer<24>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= Integer<24>(-300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= Integer<24>(-200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int24SubAssignOtherPos)
{
    // zero -> -
    Integer<24> uut = 0;
    EXPECT_EQ(uut -= Integer<24>(200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -8388608;
    EXPECT_EQ(uut -= Integer<24>(2), 8388606);
    EXPECT_EQ(uut, 8388606);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= Integer<24>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= Integer<24>(300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300000;
    EXPECT_EQ(uut -= Integer<24>(200000), -500000);
    EXPECT_EQ(uut, -500000);
}

TEST_F(VariableWidthIntArithemticTest, Int28AddAssignPrimitivPos)
{
    // zero -> +
    Integer<28> uut = 0;
    EXPECT_EQ(uut += 200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 134217726;
    EXPECT_EQ(uut += 2, -134217728);
    EXPECT_EQ(uut, -134217728);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += 300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += 200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int28AddAssignPrimitivNeg)
{
    // zero -> -
    Integer<28> uut = 0;
    EXPECT_EQ(uut += -200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -134217728;
    EXPECT_EQ(uut += -2, 134217726);
    EXPECT_EQ(uut, 134217726);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += -300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += -200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int28AddAssignOtherPos)
{
    // zero -> +
    Integer<28> uut = 0;
    EXPECT_EQ(uut += Integer<28>(200), 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 134217726;
    EXPECT_EQ(uut += Integer<28>(2), -134217728);
    EXPECT_EQ(uut, -134217728);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += Integer<28>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += Integer<28>(300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<28>(200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int28AddAssignOtherNeg)
{
    // zero -> -
    Integer<28> uut = 0;
    EXPECT_EQ(uut += Integer<28>(-200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -134217728;
    EXPECT_EQ(uut += Integer<28>(-2), 134217726);
    EXPECT_EQ(uut, 134217726);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += Integer<28>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += Integer<28>(-300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<28>(-200), -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int28SubAssignPrimitivNeg)
{
    // zero -> +
    Integer<28> uut = 0;
    EXPECT_EQ(uut -= -200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 134217726;
    EXPECT_EQ(uut -= -2, -134217728);
    EXPECT_EQ(uut, -134217728);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= -300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= -200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int28SubAssignPrimitivPos)
{
    // zero -> -
    Integer<28> uut = 0;
    EXPECT_EQ(uut -= 200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -134217728;
    EXPECT_EQ(uut -= 2, 134217726);
    EXPECT_EQ(uut, 134217726);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= 300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= 200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int28SubAssignOtherNeg)
{
    // zero -> +
    Integer<28> uut = 0;
    EXPECT_EQ(uut -= Integer<28>(-2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 134217726;
    EXPECT_EQ(uut -= Integer<28>(-2), -134217728);
    EXPECT_EQ(uut, -134217728);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= Integer<28>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= Integer<28>(-300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= Integer<28>(-200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int28SubAssignOtherPos)
{
    // zero -> -
    Integer<28> uut = 0;
    EXPECT_EQ(uut -= Integer<28>(200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -134217728;
    EXPECT_EQ(uut -= Integer<28>(2), 134217726);
    EXPECT_EQ(uut, 134217726);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= Integer<28>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= Integer<28>(300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300000;
    EXPECT_EQ(uut -= Integer<28>(200000), -500000);
    EXPECT_EQ(uut, -500000);
}

TEST_F(VariableWidthIntArithemticTest, Int50AddAssignPrimitivPos)
{
    // zero -> +
    Integer<50> uut = 0;
    EXPECT_EQ(uut += 200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 562949953421310L;
    EXPECT_EQ(uut += 2, -562949953421312L);
    EXPECT_EQ(uut, -562949953421312L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += 300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += 200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int50AddAssignPrimitivNeg)
{
    // zero -> -
    Integer<50> uut = 0;
    EXPECT_EQ(uut += -200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -562949953421312L;
    EXPECT_EQ(uut += -2, 562949953421310L);
    EXPECT_EQ(uut, 562949953421310L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += -300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += -200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int50AddAssignOtherPos)
{
    // zero -> +
    Integer<50> uut = 0;
    EXPECT_EQ(uut += Integer<50>(200), 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 562949953421310L;
    EXPECT_EQ(uut += Integer<50>(2), -562949953421312L);
    EXPECT_EQ(uut, -562949953421312L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += Integer<50>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += Integer<50>(300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<50>(200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int50AddAssignOtherNeg)
{
    // zero -> -
    Integer<50> uut = 0;
    EXPECT_EQ(uut += Integer<50>(-200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -562949953421312L;
    EXPECT_EQ(uut += Integer<50>(-2), 562949953421310L);
    EXPECT_EQ(uut, 562949953421310L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += Integer<50>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += Integer<50>(-300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<50>(-200), -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int50SubAssignPrimitivNeg)
{
    // zero -> +
    Integer<50> uut = 0;
    EXPECT_EQ(uut -= -200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 562949953421310L;
    EXPECT_EQ(uut -= -2, -562949953421312L);
    EXPECT_EQ(uut, -562949953421312L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= -300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= -200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int50SubAssignPrimitivPos)
{
    // zero -> -
    Integer<50> uut = 0;
    EXPECT_EQ(uut -= 200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -562949953421312L;
    EXPECT_EQ(uut -= 2, 562949953421310L);
    EXPECT_EQ(uut, 562949953421310L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= 300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= 200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int50SubAssignOtherNeg)
{
    // zero -> +
    Integer<50> uut = 0;
    EXPECT_EQ(uut -= Integer<50>(-2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 562949953421310L;
    EXPECT_EQ(uut -= Integer<50>(-2), -562949953421312L);
    EXPECT_EQ(uut, -562949953421312L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= Integer<50>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= Integer<50>(-300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= Integer<50>(-200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int50SubAssignOtherPos)
{
    // zero -> -
    Integer<50> uut = 0;
    EXPECT_EQ(uut -= Integer<50>(200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -562949953421312L;
    EXPECT_EQ(uut -= Integer<50>(2), 562949953421310L);
    EXPECT_EQ(uut, 562949953421310L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= Integer<50>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= Integer<50>(300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300000;
    EXPECT_EQ(uut -= Integer<50>(200000), -500000);
    EXPECT_EQ(uut, -500000);
}

TEST_F(VariableWidthIntArithemticTest, Int56AddAssignPrimitivPos)
{
    // zero -> +
    Integer<56> uut = 0;
    EXPECT_EQ(uut += 200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 36028797018963966L;
    EXPECT_EQ(uut += 2, -36028797018963968L);
    EXPECT_EQ(uut, -36028797018963968L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += 300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += 200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int56AddAssignPrimitivNeg)
{
    // zero -> -
    Integer<56> uut = 0;
    EXPECT_EQ(uut += -200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -36028797018963968L;
    EXPECT_EQ(uut += -2, 36028797018963966L);
    EXPECT_EQ(uut, 36028797018963966L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += -300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += -200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int56AddAssignOtherPos)
{
    // zero -> +
    Integer<56> uut = 0;
    EXPECT_EQ(uut += Integer<56>(200), 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 36028797018963966L;
    EXPECT_EQ(uut += Integer<56>(2), -36028797018963968L);
    EXPECT_EQ(uut, -36028797018963968L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut += Integer<56>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut += Integer<56>(300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<56>(200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int56AddAssignOtherNeg)
{
    // zero -> -
    Integer<56> uut = 0;
    EXPECT_EQ(uut += Integer<56>(-200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -36028797018963968L;
    EXPECT_EQ(uut += Integer<56>(-2), 36028797018963966L);
    EXPECT_EQ(uut, 36028797018963966L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut += Integer<56>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut += Integer<56>(-300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut += Integer<56>(-200), -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int56SubAssignPrimitivNeg)
{
    // zero -> +
    Integer<56> uut = 0;
    EXPECT_EQ(uut -= -200, 200);
    EXPECT_EQ(uut, 200);

    // overflow
    uut = 36028797018963966L;
    EXPECT_EQ(uut -= -2, -36028797018963968L);
    EXPECT_EQ(uut, -36028797018963968L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= -300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= -300, 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= -200, -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int56SubAssignPrimitivPos)
{
    // zero -> -
    Integer<56> uut = 0;
    EXPECT_EQ(uut -= 200, -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -36028797018963968L;
    EXPECT_EQ(uut -= 2, 36028797018963966L);
    EXPECT_EQ(uut, 36028797018963966L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= 300, 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= 300, 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= 200, -500);
    EXPECT_EQ(uut, -500);
}

TEST_F(VariableWidthIntArithemticTest, Int56SubAssignOtherNeg)
{
    // zero -> +
    Integer<56> uut = 0;
    EXPECT_EQ(uut -= Integer<56>(-2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 36028797018963966L;
    EXPECT_EQ(uut -= Integer<56>(-2), -36028797018963968L);
    EXPECT_EQ(uut, -36028797018963968L);

    // negative -> zero
    uut = -300;
    EXPECT_EQ(uut -= Integer<56>(-300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 300;
    EXPECT_EQ(uut -= Integer<56>(-300), 600);
    EXPECT_EQ(uut, 600);

    // neg -> neg
    uut = -300;
    EXPECT_EQ(uut -= Integer<56>(-200), -100);
    EXPECT_EQ(uut, -100);
}

TEST_F(VariableWidthIntArithemticTest, Int56SubAssignOtherPos)
{
    // zero -> -
    Integer<56> uut = 0;
    EXPECT_EQ(uut -= Integer<56>(200), -200);
    EXPECT_EQ(uut, -200);

    // overflow
    uut = -36028797018963968L;
    EXPECT_EQ(uut -= Integer<56>(2), 36028797018963966L);
    EXPECT_EQ(uut, 36028797018963966L);

    // pos -> zero
    uut = 300;
    EXPECT_EQ(uut -= Integer<56>(300), 0);
    EXPECT_EQ(uut, 0);

    // pos -> pos
    uut = 600;
    EXPECT_EQ(uut -= Integer<56>(300), 300);
    EXPECT_EQ(uut, 300);

    // neg -> neg
    uut = -300000;
    EXPECT_EQ(uut -= Integer<56>(200000), -500000);
    EXPECT_EQ(uut, -500000);
}

TEST_F(VariableWidthIntArithemticTest, UInt4AddAssignPrimitivPos)
{
    // zero -> +
    UInteger<4> uut = 0;
    EXPECT_EQ(uut += 2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 14;
    EXPECT_EQ(uut += 3, 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += 3, 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt4AddAssignOtherPos)
{
    // zero -> +
    UInteger<4> uut = 0;
    EXPECT_EQ(uut += UInteger<4>(2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 14;
    EXPECT_EQ(uut += UInteger<4>(3), 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += UInteger<4>(3), 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt4SubAssignPrimitivPos)
{
    // + -> 0
    UInteger<4> uut = 2;
    EXPECT_EQ(uut -= 2, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= 3, 14);
    EXPECT_EQ(uut, 14);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= 3, 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt4SubAssignOtherPos)
{
    // + -> 0
    UInteger<4> uut = 2;
    EXPECT_EQ(uut -= UInteger<4>(2), 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= UInteger<4>(3), 14);
    EXPECT_EQ(uut, 14);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= UInteger<4>(3), 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt12AddAssignPrimitivPos)
{
    // zero -> +
    UInteger<12> uut = 0;
    EXPECT_EQ(uut += 2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 4094;
    EXPECT_EQ(uut += 3, 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += 3, 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt12AddAssignOtherPos)
{
    // zero -> +
    UInteger<12> uut = 0;
    EXPECT_EQ(uut += UInteger<12>(2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 4094;
    EXPECT_EQ(uut += UInteger<12>(3), 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += UInteger<12>(3), 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt12SubAssignPrimitivPos)
{
    // + -> 0
    UInteger<12> uut = 2;
    EXPECT_EQ(uut -= 2, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= 3, 4094);
    EXPECT_EQ(uut, 4094);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= 3, 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt12SubAssignOtherPos)
{
    // + -> 0
    UInteger<12> uut = 2;
    EXPECT_EQ(uut -= UInteger<12>(2), 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= UInteger<12>(3), 4094);
    EXPECT_EQ(uut, 4094);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= UInteger<12>(3), 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt24AddAssignPrimitivPos)
{
    // zero -> +
    UInteger<24> uut = 0;
    EXPECT_EQ(uut += 2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 16777214;
    EXPECT_EQ(uut += 3, 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += 3, 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt24AddAssignOtherPos)
{
    // zero -> +
    UInteger<24> uut = 0;
    EXPECT_EQ(uut += UInteger<24>(2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 16777214;
    EXPECT_EQ(uut += UInteger<24>(3), 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += UInteger<24>(3), 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt24SubAssignPrimitivPos)
{
    // + -> 0
    UInteger<24> uut = 2;
    EXPECT_EQ(uut -= 2, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= 3, 16777214);
    EXPECT_EQ(uut, 16777214);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= 3, 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt24SubAssignOtherPos)
{
    // + -> 0
    UInteger<24> uut = 2;
    EXPECT_EQ(uut -= UInteger<24>(2), 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= UInteger<24>(3), 16777214);
    EXPECT_EQ(uut, 16777214);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= UInteger<24>(3), 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt28AddAssignPrimitivPos)
{
    // zero -> +
    UInteger<28> uut = 0;
    EXPECT_EQ(uut += 2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 268435454;
    EXPECT_EQ(uut += 3, 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += 3, 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt28AddAssignOtherPos)
{
    // zero -> +
    UInteger<28> uut = 0;
    EXPECT_EQ(uut += UInteger<28>(2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 268435454;
    EXPECT_EQ(uut += UInteger<28>(3), 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += UInteger<28>(3), 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt28SubAssignPrimitivPos)
{
    // + -> 0
    UInteger<28> uut = 2;
    EXPECT_EQ(uut -= 2, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= 3, 268435454);
    EXPECT_EQ(uut, 268435454);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= 3, 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt28SubAssignOtherPos)
{
    // + -> 0
    UInteger<28> uut = 2;
    EXPECT_EQ(uut -= UInteger<28>(2), 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= UInteger<28>(3), 268435454);
    EXPECT_EQ(uut, 268435454);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= UInteger<28>(3), 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt50AddAssignPrimitivPos)
{
    // zero -> +
    UInteger<50> uut = 0;
    EXPECT_EQ(uut += 2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 1125899906842622L;
    EXPECT_EQ(uut += 3, 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += 3, 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt50AddAssignOtherPos)
{
    // zero -> +
    UInteger<50> uut = 0;
    EXPECT_EQ(uut += UInteger<50>(2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 1125899906842622L;
    EXPECT_EQ(uut += UInteger<50>(3), 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += UInteger<50>(3), 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt50SubAssignPrimitivPos)
{
    // + -> 0
    UInteger<50> uut = 2;
    EXPECT_EQ(uut -= 2, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= 3, 1125899906842622L);
    EXPECT_EQ(uut, 1125899906842622L);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= 3, 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt50SubAssignOtherPos)
{
    // + -> 0
    UInteger<50> uut = 2;
    EXPECT_EQ(uut -= UInteger<50>(2), 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= UInteger<50>(3), 1125899906842622L);
    EXPECT_EQ(uut, 1125899906842622L);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= UInteger<50>(3), 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt56AddAssignPrimitivPos)
{
    // zero -> +
    UInteger<56> uut = 0;
    EXPECT_EQ(uut += 2, 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 72057594037927934L;
    EXPECT_EQ(uut += 3, 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += 3, 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt56AddAssignOtherPos)
{
    // zero -> +
    UInteger<56> uut = 0;
    EXPECT_EQ(uut += UInteger<56>(2), 2);
    EXPECT_EQ(uut, 2);

    // overflow
    uut = 72057594037927934L;
    EXPECT_EQ(uut += UInteger<56>(3), 1);
    EXPECT_EQ(uut, 1);

    // pos -> pos
    uut = 3;
    EXPECT_EQ(uut += UInteger<56>(3), 6);
    EXPECT_EQ(uut, 6);
}

TEST_F(VariableWidthIntArithemticTest, UInt56SubAssignPrimitivPos)
{
    // + -> 0
    UInteger<56> uut = 2;
    EXPECT_EQ(uut -= 2, 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= 3, 72057594037927934L);
    EXPECT_EQ(uut, 72057594037927934L);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= 3, 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, UInt56SubAssignOtherPos)
{
    // + -> 0
    UInteger<56> uut = 2;
    EXPECT_EQ(uut -= UInteger<56>(2), 0);
    EXPECT_EQ(uut, 0);

    // overflow
    uut = 1;
    EXPECT_EQ(uut -= UInteger<56>(3), 72057594037927934L);
    EXPECT_EQ(uut, 72057594037927934L);

    // pos -> pos
    uut = 6;
    EXPECT_EQ(uut -= UInteger<56>(3), 3);
    EXPECT_EQ(uut, 3);
}

TEST_F(VariableWidthIntArithemticTest, Int4AddSub)
{
    constexpr size_t bits = 4;
    typename Integer<bits>::PrimitiveType max = std::numeric_limits<Integer<bits>>::max();
    typename Integer<bits>::PrimitiveType min = std::numeric_limits<Integer<bits>>::min();
    typename Integer<bits>::PrimitiveType diff =
            std::numeric_limits<Integer<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename Integer<bits>::PrimitiveType zero = 0;

    // zero
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(diff), diff);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(-diff), -diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(zero), -diff);

    // non zero
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(diff), 2 * diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(-diff), 2 * (-diff));

    // overflow
    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(1), min);
    EXPECT_EQ(Integer<bits>(1) + Integer<bits>(max), min);

    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(diff), min + diff - 1);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(max), min + diff - 1);

    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(diff), -diff);
    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(-diff), diff);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(zero), -diff);

    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(diff), zero);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(-diff), zero);

    // overflow
    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(1), max);

    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, Int12AddSub)
{
    constexpr size_t bits = 12;
    typename Integer<bits>::PrimitiveType max = std::numeric_limits<Integer<bits>>::max();
    typename Integer<bits>::PrimitiveType min = std::numeric_limits<Integer<bits>>::min();
    typename Integer<bits>::PrimitiveType diff =
            std::numeric_limits<Integer<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename Integer<bits>::PrimitiveType zero = 0;

    // zero
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(diff), diff);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(-diff), -diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(zero), -diff);

    // non tero
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(diff), 2 * diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(-diff), 2 * (-diff));

    // overflow
    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(1), min);
    EXPECT_EQ(Integer<bits>(1) + Integer<bits>(max), min);

    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(diff), min + diff - 1);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(max), min + diff - 1);

    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(diff), -diff);
    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(-diff), diff);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(zero), -diff);

    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(diff), zero);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(-diff), zero);

    // overflow
    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(1), max);

    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, Int24AddSub)
{
    constexpr size_t bits = 24;
    typename Integer<bits>::PrimitiveType max = std::numeric_limits<Integer<bits>>::max();
    typename Integer<bits>::PrimitiveType min = std::numeric_limits<Integer<bits>>::min();
    typename Integer<bits>::PrimitiveType diff =
            std::numeric_limits<Integer<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename Integer<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(diff), diff);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(-diff), -diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(zero), -diff);

    // non tero
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(diff), 2 * diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(-diff), 2 * (-diff));

    // overflow
    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(1), min);
    EXPECT_EQ(Integer<bits>(1) + Integer<bits>(max), min);

    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(diff), min + diff - 1);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(max), min + diff - 1);

    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(diff), -diff);
    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(-diff), diff);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(zero), -diff);

    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(diff), zero);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(-diff), zero);

    // overflow
    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(1), max);

    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, Int28AddSub)
{
    constexpr size_t bits = 28;
    typename Integer<bits>::PrimitiveType max = std::numeric_limits<Integer<bits>>::max();
    typename Integer<bits>::PrimitiveType min = std::numeric_limits<Integer<bits>>::min();
    typename Integer<bits>::PrimitiveType diff =
            std::numeric_limits<Integer<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename Integer<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(diff), diff);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(-diff), -diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(zero), -diff);

    // non tero
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(diff), 2 * diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(-diff), 2 * (-diff));

    // overflow
    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(1), min);
    EXPECT_EQ(Integer<bits>(1) + Integer<bits>(max), min);

    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(diff), min + diff - 1);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(max), min + diff - 1);

    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(diff), -diff);
    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(-diff), diff);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(zero), -diff);

    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(diff), zero);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(-diff), zero);

    // overflow
    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(1), max);

    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, Int50AddSub)
{
    constexpr size_t bits = 50;
    typename Integer<bits>::PrimitiveType max = std::numeric_limits<Integer<bits>>::max();
    typename Integer<bits>::PrimitiveType min = std::numeric_limits<Integer<bits>>::min();
    typename Integer<bits>::PrimitiveType diff =
            std::numeric_limits<Integer<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename Integer<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(diff), diff);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(-diff), -diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(zero), -diff);

    // non tero
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(diff), 2 * diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(-diff), 2 * (-diff));

    // overflow
    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(1), min);
    EXPECT_EQ(Integer<bits>(1) + Integer<bits>(max), min);

    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(diff), min + diff - 1);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(max), min + diff - 1);

    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(diff), -diff);
    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(-diff), diff);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(zero), -diff);

    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(diff), zero);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(-diff), zero);

    // overflow
    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(1), max);

    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, Int56AddSub)
{
    constexpr size_t bits = 56;
    typename Integer<bits>::PrimitiveType max = std::numeric_limits<Integer<bits>>::max();
    typename Integer<bits>::PrimitiveType min = std::numeric_limits<Integer<bits>>::min();
    typename Integer<bits>::PrimitiveType diff =
            std::numeric_limits<Integer<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename Integer<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(diff), diff);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) + Integer<bits>(-diff), -diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(zero), -diff);

    // non tero
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(diff), 2 * diff);
    EXPECT_EQ(Integer<bits>(-diff) + Integer<bits>(-diff), 2 * (-diff));

    // overflow
    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(1), min);
    EXPECT_EQ(Integer<bits>(1) + Integer<bits>(max), min);

    EXPECT_EQ(Integer<bits>(max) + Integer<bits>(diff), min + diff - 1);
    EXPECT_EQ(Integer<bits>(diff) + Integer<bits>(max), min + diff - 1);

    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(diff), -diff);
    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(zero), diff);
    EXPECT_EQ(Integer<bits>(zero) - Integer<bits>(-diff), diff);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(zero), -diff);

    EXPECT_EQ(Integer<bits>(diff) - Integer<bits>(diff), zero);
    EXPECT_EQ(Integer<bits>(-diff) - Integer<bits>(-diff), zero);

    // overflow
    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(1), max);

    EXPECT_EQ(Integer<bits>(min) - Integer<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt4AddSub)
{
    constexpr size_t bits = 4;
    typename UInteger<bits>::PrimitiveType max = std::numeric_limits<UInteger<bits>>::max();
    typename UInteger<bits>::PrimitiveType diff =
            std::numeric_limits<UInteger<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename UInteger<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(UInteger<bits>(zero) + UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(zero), diff);

    // non tero
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(diff), 2 * diff);

    // overflow
    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(1), zero);
    EXPECT_EQ(UInteger<bits>(1) + UInteger<bits>(max), zero);

    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(diff), diff - 1);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(max), diff - 1);

    EXPECT_EQ(UInteger<bits>(2 * diff) - UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(zero), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(diff), zero);

    // overflow
    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(1), max);

    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt12AddSub)
{
    constexpr size_t bits = 12;
    typename UInteger<bits>::PrimitiveType max = std::numeric_limits<UInteger<bits>>::max();
    typename UInteger<bits>::PrimitiveType diff =
            std::numeric_limits<UInteger<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename UInteger<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(UInteger<bits>(zero) + UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(zero), diff);

    // non tero
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(diff), 2 * diff);

    // overflow
    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(1), zero);
    EXPECT_EQ(UInteger<bits>(1) + UInteger<bits>(max), zero);

    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(diff), diff - 1);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(max), diff - 1);

    EXPECT_EQ(UInteger<bits>(2 * diff) - UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(zero), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(diff), zero);

    // overflow
    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(1), max);

    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt24AddSub)
{
    constexpr size_t bits = 24;
    typename UInteger<bits>::PrimitiveType max = std::numeric_limits<UInteger<bits>>::max();
    typename UInteger<bits>::PrimitiveType diff =
            std::numeric_limits<UInteger<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename UInteger<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(UInteger<bits>(zero) + UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(zero), diff);

    // non tero
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(diff), 2 * diff);

    // overflow
    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(1), zero);
    EXPECT_EQ(UInteger<bits>(1) + UInteger<bits>(max), zero);

    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(diff), diff - 1);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(max), diff - 1);

    EXPECT_EQ(UInteger<bits>(2 * diff) - UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(zero), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(diff), zero);

    // overflow
    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(1), max);

    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt28AddSub)
{
    constexpr size_t bits = 28;
    typename UInteger<bits>::PrimitiveType max = std::numeric_limits<UInteger<bits>>::max();
    typename UInteger<bits>::PrimitiveType diff =
            std::numeric_limits<UInteger<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename UInteger<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(UInteger<bits>(zero) + UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(zero), diff);

    // non tero
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(diff), 2 * diff);

    // overflow
    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(1), zero);
    EXPECT_EQ(UInteger<bits>(1) + UInteger<bits>(max), zero);

    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(diff), diff - 1);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(max), diff - 1);

    EXPECT_EQ(UInteger<bits>(2 * diff) - UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(zero), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(diff), zero);

    // overflow
    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(1), max);

    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt50AddSub)
{
    constexpr size_t bits = 50;
    typename UInteger<bits>::PrimitiveType max = std::numeric_limits<UInteger<bits>>::max();
    typename UInteger<bits>::PrimitiveType diff =
            std::numeric_limits<UInteger<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename UInteger<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(UInteger<bits>(zero) + UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(zero), diff);

    // non tero
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(diff), 2 * diff);

    // overflow
    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(1), zero);
    EXPECT_EQ(UInteger<bits>(1) + UInteger<bits>(max), zero);

    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(diff), diff - 1);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(max), diff - 1);

    EXPECT_EQ(UInteger<bits>(2 * diff) - UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(zero), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(diff), zero);

    // overflow
    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(1), max);

    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt56AddSub)
{
    constexpr size_t bits = 56;
    typename UInteger<bits>::PrimitiveType max = std::numeric_limits<UInteger<bits>>::max();
    typename UInteger<bits>::PrimitiveType diff =
            std::numeric_limits<UInteger<bits>>::max() / 2 - 1;  // 0 < diff < max/2
    typename UInteger<bits>::PrimitiveType zero = 0;
    // zero
    EXPECT_EQ(UInteger<bits>(zero) + UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(zero), diff);

    // non tero
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(diff), 2 * diff);

    // overflow
    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(1), zero);
    EXPECT_EQ(UInteger<bits>(1) + UInteger<bits>(max), zero);

    EXPECT_EQ(UInteger<bits>(max) + UInteger<bits>(diff), diff - 1);
    EXPECT_EQ(UInteger<bits>(diff) + UInteger<bits>(max), diff - 1);

    EXPECT_EQ(UInteger<bits>(2 * diff) - UInteger<bits>(diff), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(zero), diff);
    EXPECT_EQ(UInteger<bits>(diff) - UInteger<bits>(diff), zero);

    // overflow
    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(1), max);

    EXPECT_EQ(UInteger<bits>(zero) - UInteger<bits>(diff), (max - diff) + 1);
}

TEST_F(VariableWidthIntArithemticTest, numericalLimitsTest)
{
    EXPECT_EQ(std::numeric_limits<UInteger<4>>::max(), 0xFU);
    EXPECT_EQ(std::numeric_limits<UInteger<4>>::min(), 0x0U);
    EXPECT_EQ(std::numeric_limits<Integer<4>>::max(), 7);
    EXPECT_EQ(std::numeric_limits<Integer<4>>::min(), -8);

    EXPECT_EQ(std::numeric_limits<UInteger<13>>::max(), 0x1FFFU);
    EXPECT_EQ(std::numeric_limits<UInteger<13>>::min(), 0x0U);
    EXPECT_EQ(std::numeric_limits<Integer<13>>::max(), 0xFFF);
    EXPECT_EQ(std::numeric_limits<Integer<13>>::min(), -4096);

    EXPECT_EQ(std::numeric_limits<UInteger<23>>::max(), 0x7FFFFFU);
    EXPECT_EQ(std::numeric_limits<UInteger<23>>::min(), 0x0U);
    EXPECT_EQ(std::numeric_limits<Integer<23>>::max(), 4194303);
    EXPECT_EQ(std::numeric_limits<Integer<23>>::min(), -4194304);

    EXPECT_EQ(std::numeric_limits<UInteger<24>>::max(), 0xFFFFFFU);
    EXPECT_EQ(std::numeric_limits<UInteger<24>>::min(), 0x0U);
    EXPECT_EQ(std::numeric_limits<Integer<24>>::max(), 8388607);
    EXPECT_EQ(std::numeric_limits<Integer<24>>::min(), -8388608);

    EXPECT_EQ(std::numeric_limits<UInteger<48>>::max(), 0xFFFFFFFFFFFFLU);
    EXPECT_EQ(std::numeric_limits<UInteger<48>>::min(), 0x0U);
    EXPECT_EQ(std::numeric_limits<Integer<48>>::max(), 140737488355327L);
    EXPECT_EQ(std::numeric_limits<Integer<48>>::min(), -140737488355328L);

    EXPECT_EQ(std::numeric_limits<UInteger<55>>::max(), 0x7FFFFFFFFFFFFFLU);
    EXPECT_EQ(std::numeric_limits<UInteger<55>>::min(), 0x0U);
    EXPECT_EQ(std::numeric_limits<Integer<55>>::max(), 18014398509481983L);
    EXPECT_EQ(std::numeric_limits<Integer<55>>::min(), -18014398509481984L);
}

TEST_F(VariableWidthIntArithemticTest, Int4Div)
{
    Integer<4> v1 = 0;
    Integer<4> v2 = 2;
    Integer<4> v3 = 4;
    EXPECT_EQ(0, v1 / v2);
    EXPECT_EQ(2, v3 / v2);
    EXPECT_EQ(2, v3 / 2);
    EXPECT_EQ(2, 4 / v2);
    Integer<4> v4 = -1;
    Integer<4> v5 = -2;

    EXPECT_EQ(2, v5 / v4);
    EXPECT_EQ(4, -4 / v4);
    EXPECT_EQ(-2, v5 / 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt4Div)
{
    UInteger<4> v1 = 0;
    UInteger<4> v2 = 2;
    UInteger<4> v3 = 4;
    EXPECT_EQ(0u, v1 / v2);
    EXPECT_EQ(2u, v3 / v2);
    EXPECT_EQ(2u, v3 / 2);
    EXPECT_EQ(2u, 4 / v2);
}

TEST_F(VariableWidthIntArithemticTest, Int12Div)
{
    Integer<12> v1 = 0;
    Integer<12> v2 = 50;
    Integer<12> v3 = 250;
    EXPECT_EQ(0, v1 / v2);
    EXPECT_EQ(5, v3 / v2);
    EXPECT_EQ(125, v3 / 2);
    EXPECT_EQ(250, v3 / 1);
    EXPECT_EQ(2, 100 / v2);
    Integer<12> v4 = -1;
    Integer<12> v5 = -2;

    EXPECT_EQ(2, v5 / v4);
    EXPECT_EQ(4, -4 / v4);
    EXPECT_EQ(-2, v5 / 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt12Div)
{
    UInteger<12> v1 = 0;
    UInteger<12> v2 = 50;
    UInteger<12> v3 = 250;
    UInteger<12> v4 = 1000;
    EXPECT_EQ(0u, v1 / v2);
    EXPECT_EQ(5u, v3 / v2);
    EXPECT_EQ(125u, v3 / 2);
    EXPECT_EQ(125u, v3 / 2);
    EXPECT_EQ(500u, v4 / 2);
    EXPECT_EQ(2u, 100 / v2);
}

TEST_F(VariableWidthIntArithemticTest, Int24Div)
{
    Integer<24> v1 = 0;
    Integer<24> v2 = 50000;
    Integer<24> v3 = 250000;
    EXPECT_EQ(0, v1 / v2);
    EXPECT_EQ(5, v3 / v2);
    EXPECT_EQ(125, v3 / 2000);
    EXPECT_EQ(125000, v3 / 2);
    EXPECT_EQ(2, 100000 / v2);
    Integer<24> v4 = -1;
    Integer<24> v5 = -2;

    EXPECT_EQ(2, v5 / v4);
    EXPECT_EQ(4, -4 / v4);
    EXPECT_EQ(-2, v5 / 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt24Div)
{
    UInteger<24> v1 = 0;
    UInteger<24> v2 = 50000;
    UInteger<24> v3 = 250000;
    EXPECT_EQ(0u, v1 / v2);
    EXPECT_EQ(5u, v3 / v2);
    EXPECT_EQ(125u, v3 / 2000);
    EXPECT_EQ(125000u, v3 / 2);
    EXPECT_EQ(2u, 100000 / v2);
}

TEST_F(VariableWidthIntArithemticTest, Int50Div)
{
    Integer<50> v1 = 0L;
    Integer<50> v2 = 50000L;
    Integer<50> v3 = 250000000000L;
    EXPECT_EQ(0, v1 / v2);
    EXPECT_EQ(5000000L, v3 / v2);
    EXPECT_EQ(125000000L, v3 / 2000L);
    EXPECT_EQ(125000000000L, v3 / 2L);
    EXPECT_EQ(2L, 100000L / v2);
    Integer<50> v4 = -1;
    Integer<50> v5 = -2;

    EXPECT_EQ(2, v5 / v4);
    EXPECT_EQ(4, -4 / v4);
    EXPECT_EQ(-2, v5 / 1);
}

TEST_F(VariableWidthIntArithemticTest, UInt50Div)
{
    UInteger<50> v1 = 0Lu;
    UInteger<50> v2 = 50000Lu;
    UInteger<50> v3 = 250000000000Lu;
    EXPECT_EQ(0u, v1 / v2);
    EXPECT_EQ(5000000Lu, v3 / v2);
    EXPECT_EQ(125000000Lu, v3 / 2000Lu);
    EXPECT_EQ(125000000000Lu, v3 / 2Lu);
    EXPECT_EQ(2Lu, 100000Lu / v2);
}
