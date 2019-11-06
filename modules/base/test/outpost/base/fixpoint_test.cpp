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
 * - 2014-2018, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/base/fixpoint.h>

#include <unittest/harness.h>

#include <array>

// Instantiate template class and functions for correct lcov calculations
template class outpost::FP<16>;

template outpost::FP<16>
outpost::FP<16>::operator+(const double& x) const;

template outpost::FP<16>&
outpost::FP<16>::operator+=(const double& x);

template outpost::FP<16>
outpost::FP<16>::operator-(const double& x) const;

template outpost::FP<16>&
outpost::FP<16>::operator-=(const double& x);

template outpost::FP<16> outpost::FP<16>::operator*(const double& x) const;

template outpost::FP<16>&
outpost::FP<16>::operator*=(const double& x);

template outpost::FP<16>
outpost::FP<16>::operator/(const double& x) const;

template outpost::FP<16>&
outpost::FP<16>::operator/=(const double& x);

template bool
outpost::FP<16>::operator<(const double& x) const;

template bool
outpost::FP<16>::operator>(const double& x) const;

template bool
outpost::FP<16>::operator<=(const double& x) const;

template bool
outpost::FP<16>::operator>=(const double& x) const;

template bool
outpost::FP<16>::operator==(const double& x) const;

template bool
outpost::FP<16>::operator!=(const double& x) const;

TEST(FixpointTest, fixpointDefaultContructors)
{
    outpost::Fixpoint fp1;
    EXPECT_EQ(fp1.getValue(), 0);

    int16_t n16 = 135;
    outpost::Fixpoint fp2(n16);
    EXPECT_EQ(fp2.getValue(), 135 << 16);

    int32_t n32 = -16000;
    outpost::Fixpoint fp3(n32);
    EXPECT_EQ(fp3.getValue(), -1048576000);

    float f = 3.25f;
    outpost::Fixpoint fp4(f);
    EXPECT_EQ(fp4.getValue(), 0x00034000);

    double d = 1.125;
    outpost::Fixpoint fp5(d);
    EXPECT_EQ(fp5.getValue(), 0x00012000);

    outpost::Fixpoint fp6(fp5);
    EXPECT_EQ(fp6.getValue(), 0x00012000);
}

TEST(FixpointTest, castingOperators)
{
    double d = 1.125;
    outpost::Fixpoint fp1(d);
    EXPECT_EQ(static_cast<int32_t>(fp1), 1);
    EXPECT_EQ(static_cast<int16_t>(fp1), 1);
    EXPECT_EQ(static_cast<float>(fp1), 1.125f);
    EXPECT_EQ(static_cast<double>(fp1), 1.125);

    d = 1.5;
    outpost::Fixpoint fp2(d);
    EXPECT_EQ(static_cast<int32_t>(fp2), 2);
    EXPECT_EQ(static_cast<int16_t>(fp2), 2);
    EXPECT_EQ(static_cast<float>(fp2), 1.5f);
    EXPECT_EQ(static_cast<double>(fp2), 1.5);

    d = -1.125;
    outpost::Fixpoint fp3(d);
    EXPECT_EQ(static_cast<int32_t>(fp3), -1);
    EXPECT_EQ(static_cast<int16_t>(fp3), -1);
    EXPECT_EQ(static_cast<float>(fp3), -1.125f);
    EXPECT_EQ(static_cast<double>(fp3), -1.125);

    d = -3.5;
    outpost::Fixpoint fp4(d);
    EXPECT_EQ(static_cast<int32_t>(fp4), -4);
    EXPECT_EQ(static_cast<int16_t>(fp4), -4);
    EXPECT_EQ(static_cast<float>(fp4), -3.5f);
    EXPECT_EQ(static_cast<double>(fp4), -3.5);
}

TEST(FixpointTest, assignmentOperators)
{
    double d = -1.75;
    float f = -2.5f;
    int16_t i16 = 125;
    int32_t i32 = -3;

    outpost::Fixpoint fp;
    fp = d;
    EXPECT_EQ(static_cast<double>(fp), -1.75);

    fp = f;
    EXPECT_EQ(static_cast<double>(fp), -2.5);

    fp = i16;
    EXPECT_EQ(static_cast<double>(fp), 125.0);

    fp = i32;
    EXPECT_EQ(static_cast<double>(fp), -3.0);

    outpost::Fixpoint fp2;
    fp2 = fp;
    EXPECT_EQ(static_cast<double>(fp2), -3.0);
}

TEST(FixpointTest, additionOperators)
{
    double d = -1.75;
    float f = -2.5f;
    int16_t i16 = 125;
    int32_t i32 = -3;

    outpost::Fixpoint fp1(17.5);
    outpost::Fixpoint fp2(-16.5);

    outpost::Fixpoint res = fp1 + fp2;
    EXPECT_EQ(static_cast<double>(res), 1.0);

    res += f;
    EXPECT_EQ(static_cast<double>(res), -1.5);

    res = fp1 + d;
    EXPECT_EQ(static_cast<double>(res), 15.75);

    res += f;
    EXPECT_EQ(static_cast<double>(res), 13.25);

    res += i16;
    EXPECT_EQ(static_cast<double>(res), 138.25);

    res = res + i32;
    EXPECT_EQ(static_cast<double>(res), 135.25);
}

TEST(FixpointTest, subtractionOperators)
{
    double d = -1.75;
    float f = -2.5f;
    int16_t i16 = 125;
    int32_t i32 = -3;

    outpost::Fixpoint fp1(17.5);
    outpost::Fixpoint fp2(-16.5);

    outpost::Fixpoint res = fp1 - fp2;
    EXPECT_EQ(static_cast<double>(res), 34.0);

    res -= f;
    EXPECT_EQ(static_cast<double>(res), 36.5);

    res = fp1 - d;
    EXPECT_EQ(static_cast<double>(res), 19.25);

    res -= f;
    EXPECT_EQ(static_cast<double>(res), 21.75);

    res -= i16;
    EXPECT_EQ(static_cast<double>(res), -103.25);

    res = res - i32;
    EXPECT_EQ(static_cast<double>(res), -100.25);
}

TEST(FixpointTest, multiplicationOperators)
{
    double d = -1.75;
    float f = -2.5f;
    int16_t i16 = 125;
    int32_t i32 = -1;

    outpost::Fixpoint fp1(17.5);
    outpost::Fixpoint fp2(-16.5);

    outpost::Fixpoint res = fp1 * fp2;
    EXPECT_EQ(static_cast<double>(res), -288.75);

    res *= f;
    EXPECT_EQ(static_cast<double>(res), 721.875);

    res = fp1 * d;
    EXPECT_EQ(static_cast<double>(res), -30.625);

    res *= f;
    EXPECT_EQ(static_cast<double>(res), 76.5625);

    res *= i16;
    EXPECT_EQ(static_cast<double>(res), 9570.3125);

    res = res * i32;
    EXPECT_EQ(static_cast<double>(res), -9570.3125);
}

TEST(FixpointTest, divisionOperators)
{
    double d = -1.75;
    float f = -0.008f;
    int16_t i16 = 125;
    int32_t i32 = -3;

    outpost::Fixpoint fp1(17.5);
    outpost::Fixpoint fp2(-16.5);

    outpost::Fixpoint res = fp1 / fp2;
    EXPECT_LE(static_cast<double>(res) + 1.06060, 0.0001);
    EXPECT_GE(static_cast<double>(res) + 1.06060, -0.0001);

    res /= f;
    EXPECT_LE(static_cast<double>(res) - 132.6469, 0.0001);
    EXPECT_GE(static_cast<double>(res) - 132.6469, -0.0001);

    res = fp1 / d;
    EXPECT_EQ(static_cast<double>(res), -10.0);

    res /= f;
    EXPECT_LE(static_cast<double>(res) - 1250.6870, 0.0001);
    EXPECT_GE(static_cast<double>(res) - 1250.6870, -0.0001);

    res /= i16;
    EXPECT_LE(static_cast<double>(res) - 10.0055, 0.0001);
    EXPECT_GE(static_cast<double>(res) - 10.0055, -0.0001);

    res = res / i32;
    EXPECT_LE(static_cast<double>(res) + 3.3351, 0.0001);
    EXPECT_GE(static_cast<double>(res) + 3.3351, -0.0001);
}

TEST(FixpointTest, shiftOperator)
{
    int32_t t = 0xFFFF;
    outpost::Fixpoint fp(t);

    fp >>= 1;
    EXPECT_EQ(fp.getValue(), static_cast<int32_t>(0xFFFF8000));

    fp = fp >> 16;
    EXPECT_EQ(fp.getValue(), static_cast<int32_t>(0xFFFFFFFF));

    fp <<= 1;
    EXPECT_EQ(fp.getValue(), static_cast<int32_t>(0xFFFFFFFE));

    fp = fp << 31;
    EXPECT_EQ(fp.getValue(), static_cast<int32_t>(0));

    outpost::Fixpoint fp2(-1.5);
    fp2 <<= 3;
    EXPECT_EQ(static_cast<double>(fp2), -12.0);

    outpost::Fixpoint fp3(1.5);
    fp3 <<= 3;
    EXPECT_EQ(static_cast<double>(fp3), 12.0);
}

TEST(FixpointTest, setValue)
{
    outpost::Fixpoint fp(125.0);
    fp.setValue(0xFFFF8000);
    EXPECT_EQ(static_cast<double>(fp), -0.5);
}

TEST(FixpointTest, ltGtOperators)
{
    outpost::Fixpoint fp1(123.0);
    outpost::Fixpoint fp2(0.125);
    outpost::Fixpoint fp3(-123.0);
    outpost::Fixpoint fp4(-0.125);

    EXPECT_FALSE(fp1 < fp2);
    EXPECT_FALSE(fp1 < fp3);
    EXPECT_FALSE(fp2 < fp4);
    EXPECT_FALSE(fp4 < fp3);

    EXPECT_FALSE(fp1 < 0.0001);
    EXPECT_FALSE(fp1 < 122);
    EXPECT_FALSE(fp1 < 123);

    EXPECT_TRUE(fp1 > fp2);
    EXPECT_TRUE(fp1 > fp3);
    EXPECT_TRUE(fp2 > fp4);
    EXPECT_TRUE(fp4 > fp3);

    EXPECT_TRUE(fp1 > 0.0001f);
    EXPECT_TRUE(fp1 > 122);

    EXPECT_FALSE(fp1 > 123);
}

TEST(FixpointTest, lessOrEqualOperators)
{
    outpost::Fixpoint fp1(123.0);
    outpost::Fixpoint fp2(0.125);
    outpost::Fixpoint fp3(-123.0);
    outpost::Fixpoint fp4(-0.125);

    EXPECT_FALSE(fp1 <= fp2);
    EXPECT_FALSE(fp1 <= fp3);
    EXPECT_FALSE(fp2 <= fp4);
    EXPECT_FALSE(fp4 <= fp3);

    EXPECT_FALSE(fp1 <= 0.0001);
    EXPECT_FALSE(fp1 <= 122);

    EXPECT_TRUE(fp1 >= fp2);
    EXPECT_TRUE(fp1 >= fp3);
    EXPECT_TRUE(fp2 >= fp4);
    EXPECT_TRUE(fp4 >= fp3);

    EXPECT_TRUE(fp1 >= 0.0001);
    EXPECT_TRUE(fp1 >= static_cast<int16_t>(122));
    EXPECT_TRUE(fp1 >= static_cast<int16_t>(123));
    EXPECT_TRUE(fp1 <= static_cast<int16_t>(123));
}

TEST(FixpointTest, equalityOperators)
{
    outpost::Fixpoint fp1(1.125);
    outpost::Fixpoint fp2(1.135);

    EXPECT_TRUE(fp1 != fp2);
    EXPECT_TRUE(fp1 == 1.125);
    EXPECT_TRUE(fp2 == 1.135);
    EXPECT_TRUE(static_cast<double>(fp2) != 1.135);

    EXPECT_TRUE(fp1 != 1);
    EXPECT_TRUE(fp2 != 1.13);

    double d = 2.5;
    float f = 2.5f;
    outpost::Fixpoint fp3(d);
    outpost::Fixpoint fp4(f);

    EXPECT_EQ(fp3, fp4);
}

TEST(FixpointTest, absTest)
{
	outpost::Fixpoint fp1(1.125);
	outpost::Fixpoint fp2(-1.678);

	EXPECT_EQ(fp1.abs(), fp1);
	EXPECT_EQ(fp2.abs(), fp2*(-1.0));
}
