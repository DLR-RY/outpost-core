/*
 * Copyright (c) 2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2022, Tobias Pfeffer (DLR RY-AVS)
 */

#include <outpost/utils/storage/variable_width_integer.h>

#include <unittest/harness.h>

using namespace outpost::utils;

TEST(VariableWidthIntegerCastTest, uint8Casts)
{
    uint8_t u0 = 1U << 0;
    uint8_t u1 = 1U << 1;
    uint8_t u2 = 1U << 2;
    uint8_t u3 = 1U << 3;
    uint8_t u4 = 1U << 4;
    uint8_t u5 = 1U << 5;
    uint8_t u6 = 1U << 6;
    uint8_t u7 = 1U << 7;

    int16_t i0 = 1 << 0;
    int16_t i1 = 1 << 1;
    int16_t i2 = 1 << 2;
    int16_t i3 = 1 << 3;
    int16_t i4 = 1 << 4;
    int16_t i5 = 1 << 5;
    int16_t i6 = 1 << 6;
    int16_t i7 = 1 << 7;

    UInteger<1> vu0 = u0;
    UInteger<2> vu1 = u1;
    UInteger<3> vu2 = u2;
    UInteger<4> vu3 = u3;
    UInteger<5> vu4 = u4;
    UInteger<6> vu5 = u5;
    UInteger<7> vu6 = u6;
    UInteger<8> vu7 = u7;

    EXPECT_EQ(u0, static_cast<uint8_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint8_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint8_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint8_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint8_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint8_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint8_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint8_t>(vu7));

    EXPECT_EQ(u0, static_cast<uint16_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint16_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint16_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint16_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint16_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint16_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint16_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint16_t>(vu7));

    EXPECT_EQ(u0, static_cast<uint32_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint32_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint32_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint32_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint32_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint32_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint32_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint32_t>(vu7));

    EXPECT_EQ(u0, static_cast<uint64_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint64_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint64_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint64_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint64_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint64_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint64_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint64_t>(vu7));

    EXPECT_EQ(i0, static_cast<int16_t>(vu0));
    EXPECT_EQ(i1, static_cast<int16_t>(vu1));
    EXPECT_EQ(i2, static_cast<int16_t>(vu2));
    EXPECT_EQ(i3, static_cast<int16_t>(vu3));
    EXPECT_EQ(i4, static_cast<int16_t>(vu4));
    EXPECT_EQ(i5, static_cast<int16_t>(vu5));
    EXPECT_EQ(i6, static_cast<int16_t>(vu6));
    EXPECT_EQ(i7, static_cast<int16_t>(vu7));

    EXPECT_EQ(i0, static_cast<int32_t>(vu0));
    EXPECT_EQ(i1, static_cast<int32_t>(vu1));
    EXPECT_EQ(i2, static_cast<int32_t>(vu2));
    EXPECT_EQ(i3, static_cast<int32_t>(vu3));
    EXPECT_EQ(i4, static_cast<int32_t>(vu4));
    EXPECT_EQ(i5, static_cast<int32_t>(vu5));
    EXPECT_EQ(i6, static_cast<int32_t>(vu6));
    EXPECT_EQ(i7, static_cast<int32_t>(vu7));

    EXPECT_EQ(i0, static_cast<int64_t>(vu0));
    EXPECT_EQ(i1, static_cast<int64_t>(vu1));
    EXPECT_EQ(i2, static_cast<int64_t>(vu2));
    EXPECT_EQ(i3, static_cast<int64_t>(vu3));
    EXPECT_EQ(i4, static_cast<int64_t>(vu4));
    EXPECT_EQ(i5, static_cast<int64_t>(vu5));
    EXPECT_EQ(i6, static_cast<int64_t>(vu6));
    EXPECT_EQ(i7, static_cast<int64_t>(vu7));
}

TEST(VariableWidthIntegerCastTest, int8CastsPositive)
{
    // below limits: should stay positive in the VWIs
    int8_t i0 = 0;
    int8_t i1 = 1 << 0;
    int8_t i2 = 1 << 1;
    int8_t i3 = 1 << 2;
    int8_t i4 = 1 << 3;
    int8_t i5 = 1 << 4;
    int8_t i6 = 1 << 5;
    int8_t i7 = 1 << 6;

    Integer<1> vi0 = i0;
    Integer<2> vi1 = i1;
    Integer<3> vi2 = i2;
    Integer<4> vi3 = i3;
    Integer<5> vi4 = i4;
    Integer<6> vi5 = i5;
    Integer<7> vi6 = i6;
    Integer<8> vi7 = i7;

    EXPECT_EQ(i0, static_cast<int8_t>(vi0));
    EXPECT_EQ(i1, static_cast<int8_t>(vi1));
    EXPECT_EQ(i2, static_cast<int8_t>(vi2));
    EXPECT_EQ(i3, static_cast<int8_t>(vi3));
    EXPECT_EQ(i4, static_cast<int8_t>(vi4));
    EXPECT_EQ(i5, static_cast<int8_t>(vi5));
    EXPECT_EQ(i6, static_cast<int8_t>(vi6));
    EXPECT_EQ(i7, static_cast<int8_t>(vi7));

    EXPECT_EQ(i0, static_cast<int16_t>(vi0));
    EXPECT_EQ(i1, static_cast<int16_t>(vi1));
    EXPECT_EQ(i2, static_cast<int16_t>(vi2));
    EXPECT_EQ(i3, static_cast<int16_t>(vi3));
    EXPECT_EQ(i4, static_cast<int16_t>(vi4));
    EXPECT_EQ(i5, static_cast<int16_t>(vi5));
    EXPECT_EQ(i6, static_cast<int16_t>(vi6));
    EXPECT_EQ(i7, static_cast<int16_t>(vi7));

    EXPECT_EQ(i0, static_cast<int32_t>(vi0));
    EXPECT_EQ(i1, static_cast<int32_t>(vi1));
    EXPECT_EQ(i2, static_cast<int32_t>(vi2));
    EXPECT_EQ(i3, static_cast<int32_t>(vi3));
    EXPECT_EQ(i4, static_cast<int32_t>(vi4));
    EXPECT_EQ(i5, static_cast<int32_t>(vi5));
    EXPECT_EQ(i6, static_cast<int32_t>(vi6));
    EXPECT_EQ(i7, static_cast<int32_t>(vi7));

    EXPECT_EQ(i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(i7, static_cast<int64_t>(vi7));
}

TEST(VariableWidthIntegerCastTest, int8CastsNegative)
{
    // range limits: should turn negative in the VWIs
    int16_t i0 = 1 << 0;
    int16_t i1 = 1 << 1;
    int16_t i2 = 1 << 2;
    int16_t i3 = 1 << 3;
    int16_t i4 = 1 << 4;
    int16_t i5 = 1 << 5;
    int16_t i6 = 1 << 6;
    int16_t i7 = 1 << 7;

    Integer<1> vi0 = i0;
    Integer<2> vi1 = i1;
    Integer<3> vi2 = i2;
    Integer<4> vi3 = i3;
    Integer<5> vi4 = i4;
    Integer<6> vi5 = i5;
    Integer<7> vi6 = i6;
    Integer<8> vi7 = i7;

    EXPECT_EQ(-i0, static_cast<int8_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int8_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int8_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int8_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int8_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int8_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int8_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int8_t>(vi7));

    EXPECT_EQ(-i0, static_cast<int16_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int16_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int16_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int16_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int16_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int16_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int16_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int16_t>(vi7));

    EXPECT_EQ(-i0, static_cast<int32_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int32_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int32_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int32_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int32_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int32_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int32_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int32_t>(vi7));

    EXPECT_EQ(-i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int64_t>(vi7));
}

TEST(VariableWidthIntegerCastTest, uint16Casts)
{
    uint16_t u0 = 1U << 8;
    uint16_t u1 = 1U << 9;
    uint16_t u2 = 1U << 10;
    uint16_t u3 = 1U << 11;
    uint16_t u4 = 1U << 12;
    uint16_t u5 = 1U << 13;
    uint16_t u6 = 1U << 14;
    uint16_t u7 = 1U << 15;

    int32_t i0 = 1 << 8;
    int32_t i1 = 1 << 9;
    int32_t i2 = 1 << 10;
    int32_t i3 = 1 << 11;
    int32_t i4 = 1 << 12;
    int32_t i5 = 1 << 13;
    int32_t i6 = 1 << 14;
    int32_t i7 = 1 << 15;

    UInteger<9> vu0 = u0;
    UInteger<10> vu1 = u1;
    UInteger<11> vu2 = u2;
    UInteger<12> vu3 = u3;
    UInteger<13> vu4 = u4;
    UInteger<14> vu5 = u5;
    UInteger<15> vu6 = u6;
    UInteger<16> vu7 = u7;

    EXPECT_EQ(u0, static_cast<uint16_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint16_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint16_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint16_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint16_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint16_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint16_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint16_t>(vu7));

    EXPECT_EQ(u0, static_cast<uint32_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint32_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint32_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint32_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint32_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint32_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint32_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint32_t>(vu7));

    EXPECT_EQ(u0, static_cast<uint64_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint64_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint64_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint64_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint64_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint64_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint64_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint64_t>(vu7));

    EXPECT_EQ(i0, static_cast<int32_t>(vu0));
    EXPECT_EQ(i1, static_cast<int32_t>(vu1));
    EXPECT_EQ(i2, static_cast<int32_t>(vu2));
    EXPECT_EQ(i3, static_cast<int32_t>(vu3));
    EXPECT_EQ(i4, static_cast<int32_t>(vu4));
    EXPECT_EQ(i5, static_cast<int32_t>(vu5));
    EXPECT_EQ(i6, static_cast<int32_t>(vu6));
    EXPECT_EQ(i7, static_cast<int32_t>(vu7));

    EXPECT_EQ(i0, static_cast<int64_t>(vu0));
    EXPECT_EQ(i1, static_cast<int64_t>(vu1));
    EXPECT_EQ(i2, static_cast<int64_t>(vu2));
    EXPECT_EQ(i3, static_cast<int64_t>(vu3));
    EXPECT_EQ(i4, static_cast<int64_t>(vu4));
    EXPECT_EQ(i5, static_cast<int64_t>(vu5));
    EXPECT_EQ(i6, static_cast<int64_t>(vu6));
    EXPECT_EQ(i7, static_cast<int64_t>(vu7));
}

TEST(VariableWidthIntegerCastTest, int16CastsPositive)
{
    // below limits: should stay positive in the VWIs
    int16_t i0 = 1 << 7;
    int16_t i1 = 1 << 8;
    int16_t i2 = 1 << 9;
    int16_t i3 = 1 << 10;
    int16_t i4 = 1 << 11;
    int16_t i5 = 1 << 12;
    int16_t i6 = 1 << 13;
    int16_t i7 = 1 << 14;

    Integer<9> vi0 = i0;
    Integer<10> vi1 = i1;
    Integer<11> vi2 = i2;
    Integer<12> vi3 = i3;
    Integer<13> vi4 = i4;
    Integer<14> vi5 = i5;
    Integer<15> vi6 = i6;
    Integer<16> vi7 = i7;

    EXPECT_EQ(i0, static_cast<int16_t>(vi0));
    EXPECT_EQ(i1, static_cast<int16_t>(vi1));
    EXPECT_EQ(i2, static_cast<int16_t>(vi2));
    EXPECT_EQ(i3, static_cast<int16_t>(vi3));
    EXPECT_EQ(i4, static_cast<int16_t>(vi4));
    EXPECT_EQ(i5, static_cast<int16_t>(vi5));
    EXPECT_EQ(i6, static_cast<int16_t>(vi6));
    EXPECT_EQ(i7, static_cast<int16_t>(vi7));

    EXPECT_EQ(i0, static_cast<int32_t>(vi0));
    EXPECT_EQ(i1, static_cast<int32_t>(vi1));
    EXPECT_EQ(i2, static_cast<int32_t>(vi2));
    EXPECT_EQ(i3, static_cast<int32_t>(vi3));
    EXPECT_EQ(i4, static_cast<int32_t>(vi4));
    EXPECT_EQ(i5, static_cast<int32_t>(vi5));
    EXPECT_EQ(i6, static_cast<int32_t>(vi6));
    EXPECT_EQ(i7, static_cast<int32_t>(vi7));

    EXPECT_EQ(i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(i7, static_cast<int64_t>(vi7));
}

TEST(VariableWidthIntegerCastTest, int16CastsNegative)
{
    // range limits: should turn negative in the VWIs
    int32_t i0 = 1 << 8;
    int32_t i1 = 1 << 9;
    int32_t i2 = 1 << 10;
    int32_t i3 = 1 << 11;
    int32_t i4 = 1 << 12;
    int32_t i5 = 1 << 13;
    int32_t i6 = 1 << 14;
    int32_t i7 = 1 << 15;

    Integer<9> vi0 = i0;
    Integer<10> vi1 = i1;
    Integer<11> vi2 = i2;
    Integer<12> vi3 = i3;
    Integer<13> vi4 = i4;
    Integer<14> vi5 = i5;
    Integer<15> vi6 = i6;
    Integer<16> vi7 = i7;

    EXPECT_EQ(-i0, static_cast<int16_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int16_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int16_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int16_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int16_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int16_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int16_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int16_t>(vi7));

    EXPECT_EQ(-i0, static_cast<int32_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int32_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int32_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int32_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int32_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int32_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int32_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int32_t>(vi7));

    EXPECT_EQ(-i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int64_t>(vi7));
}

TEST(VariableWidthIntegerCastTest, uint32Casts)
{
    uint32_t u0 = 1U << 16;
    uint32_t u1 = 1U << 17;
    uint32_t u2 = 1U << 18;
    uint32_t u3 = 1U << 23;
    uint32_t u4 = 1U << 24;
    uint32_t u5 = 1U << 28;
    uint32_t u6 = 1U << 30;
    uint32_t u7 = 1U << 31;

    int64_t i0 = 1L << 16;
    int64_t i1 = 1L << 17;
    int64_t i2 = 1L << 18;
    int64_t i3 = 1L << 23;
    int64_t i4 = 1L << 24;
    int64_t i5 = 1L << 28;
    int64_t i6 = 1L << 30;
    int64_t i7 = 1L << 31;

    UInteger<17> vu0 = u0;
    UInteger<18> vu1 = u1;
    UInteger<19> vu2 = u2;
    UInteger<24> vu3 = u3;
    UInteger<25> vu4 = u4;
    UInteger<29> vu5 = u5;
    UInteger<31> vu6 = u6;
    UInteger<32> vu7 = u7;

    EXPECT_EQ(u0, static_cast<uint32_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint32_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint32_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint32_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint32_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint32_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint32_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint32_t>(vu7));

    EXPECT_EQ(u0, static_cast<uint64_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint64_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint64_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint64_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint64_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint64_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint64_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint64_t>(vu7));

    EXPECT_EQ(i0, static_cast<int64_t>(vu0));
    EXPECT_EQ(i1, static_cast<int64_t>(vu1));
    EXPECT_EQ(i2, static_cast<int64_t>(vu2));
    EXPECT_EQ(i3, static_cast<int64_t>(vu3));
    EXPECT_EQ(i4, static_cast<int64_t>(vu4));
    EXPECT_EQ(i5, static_cast<int64_t>(vu5));
    EXPECT_EQ(i6, static_cast<int64_t>(vu6));
    EXPECT_EQ(i7, static_cast<int64_t>(vu7));
}

TEST(VariableWidthIntegerCastTest, int32CastsPositive)
{
    // below limits: should stay positive in the VWIs
    int32_t i0 = 1 << 15;
    int32_t i1 = 1 << 16;
    int32_t i2 = 1 << 17;
    int32_t i3 = 1 << 22;
    int32_t i4 = 1 << 23;
    int32_t i5 = 1 << 27;
    int32_t i6 = 1 << 29;
    int32_t i7 = 1 << 30;

    Integer<17> vi0 = i0;
    Integer<18> vi1 = i1;
    Integer<19> vi2 = i2;
    Integer<24> vi3 = i3;
    Integer<25> vi4 = i4;
    Integer<29> vi5 = i5;
    Integer<31> vi6 = i6;
    Integer<32> vi7 = i7;

    EXPECT_EQ(i0, static_cast<int32_t>(vi0));
    EXPECT_EQ(i1, static_cast<int32_t>(vi1));
    EXPECT_EQ(i2, static_cast<int32_t>(vi2));
    EXPECT_EQ(i3, static_cast<int32_t>(vi3));
    EXPECT_EQ(i4, static_cast<int32_t>(vi4));
    EXPECT_EQ(i5, static_cast<int32_t>(vi5));
    EXPECT_EQ(i6, static_cast<int32_t>(vi6));
    EXPECT_EQ(i7, static_cast<int32_t>(vi7));

    EXPECT_EQ(i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(i7, static_cast<int64_t>(vi7));
}

TEST(VariableWidthIntegerCastTest, int32CastsNegative)
{
    // range limits: should turn negative in the VWIs
    int64_t i0 = 1L << 16;
    int64_t i1 = 1L << 17;
    int64_t i2 = 1L << 18;
    int64_t i3 = 1L << 23;
    int64_t i4 = 1L << 24;
    int64_t i5 = 1L << 28;
    int64_t i6 = 1L << 30;
    int64_t i7 = 1L << 31;

    Integer<17> vi0 = i0;
    Integer<18> vi1 = i1;
    Integer<19> vi2 = i2;
    Integer<24> vi3 = i3;
    Integer<25> vi4 = i4;
    Integer<29> vi5 = i5;
    Integer<31> vi6 = i6;
    Integer<32> vi7 = i7;

    EXPECT_EQ(-i0, static_cast<int32_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int32_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int32_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int32_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int32_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int32_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int32_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int32_t>(vi7));

    EXPECT_EQ(-i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(-i7, static_cast<int64_t>(vi7));
}

TEST(VariableWidthIntegerCastTest, uint64Casts)
{
    uint64_t u0 = 1UL << 16;
    uint64_t u1 = 1UL << 17;
    uint64_t u2 = 1UL << 18;
    uint64_t u3 = 1UL << 23;
    uint64_t u4 = 1UL << 24;
    uint64_t u5 = 1UL << 28;
    uint64_t u6 = 1UL << 30;
    uint64_t u7 = 1UL << 31;

    UInteger<33> vu0 = u0;
    UInteger<36> vu1 = u1;
    UInteger<43> vu2 = u2;
    UInteger<48> vu3 = u3;
    UInteger<52> vu4 = u4;
    UInteger<61> vu5 = u5;
    UInteger<63> vu6 = u6;
    UInteger<64> vu7 = u7;

    EXPECT_EQ(u0, static_cast<uint64_t>(vu0));
    EXPECT_EQ(u1, static_cast<uint64_t>(vu1));
    EXPECT_EQ(u2, static_cast<uint64_t>(vu2));
    EXPECT_EQ(u3, static_cast<uint64_t>(vu3));
    EXPECT_EQ(u4, static_cast<uint64_t>(vu4));
    EXPECT_EQ(u5, static_cast<uint64_t>(vu5));
    EXPECT_EQ(u6, static_cast<uint64_t>(vu6));
    EXPECT_EQ(u7, static_cast<uint64_t>(vu7));
}

TEST(VariableWidthIntegerCastTest, int64CastsPositive)
{
    // below limits: should stay positive in the VWIs
    int64_t i0 = 1L << 31;
    int64_t i1 = 1L << 34;
    int64_t i2 = 1L << 41;
    int64_t i3 = 1L << 46;
    int64_t i4 = 1L << 50;
    int64_t i5 = 1L << 59;
    int64_t i6 = 1L << 61;
    int64_t i7 = 1L << 62;

    Integer<33> vi0 = i0;
    Integer<36> vi1 = i1;
    Integer<43> vi2 = i2;
    Integer<48> vi3 = i3;
    Integer<52> vi4 = i4;
    Integer<61> vi5 = i5;
    Integer<63> vi6 = i6;
    Integer<64> vi7 = i7;

    EXPECT_EQ(i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(i7, static_cast<int64_t>(vi7));
}

TEST(VariableWidthIntegerCastTest, int64CastsNegative)
{
    // range limits: should turn negative in the VWIs
    int64_t i0 = 1L << 32;
    int64_t i1 = 1L << 35;
    int64_t i2 = 1L << 42;
    int64_t i3 = 1L << 47;
    int64_t i4 = 1L << 51;
    int64_t i5 = 1L << 60;
    int64_t i6 = 1L << 62;
    // special case: already negative here
    // cppcheck-suppress shiftTooManyBitsSigned
    int64_t i7 = 1L << 63;
    EXPECT_EQ(i7, std::numeric_limits<int64_t>::min());

    Integer<33> vi0 = i0;
    Integer<36> vi1 = i1;
    Integer<43> vi2 = i2;
    Integer<48> vi3 = i3;
    Integer<52> vi4 = i4;
    Integer<61> vi5 = i5;
    Integer<63> vi6 = i6;
    Integer<64> vi7 = i7;

    EXPECT_EQ(-i0, static_cast<int64_t>(vi0));
    EXPECT_EQ(-i1, static_cast<int64_t>(vi1));
    EXPECT_EQ(-i2, static_cast<int64_t>(vi2));
    EXPECT_EQ(-i3, static_cast<int64_t>(vi3));
    EXPECT_EQ(-i4, static_cast<int64_t>(vi4));
    EXPECT_EQ(-i5, static_cast<int64_t>(vi5));
    EXPECT_EQ(-i6, static_cast<int64_t>(vi6));
    EXPECT_EQ(i7, static_cast<int64_t>(vi7));
}
