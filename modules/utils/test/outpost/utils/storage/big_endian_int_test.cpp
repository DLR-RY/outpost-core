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

// As we cannot test being on another CPU arch this
// here is just a test whether it compiles and basic
// properties

#include <outpost/utils/storage/big_endian_int.h>

#include <unittest/harness.h>

using namespace outpost::utils;

class BigEndianIntTest : public testing::Test
{
};

TEST_F(BigEndianIntTest, testInt64)
{
    beint64 a;  // default constructor
    int64_t v = 1;
    beint64 b(v);  // constructor from value
    a = b;         // same assignment
    EXPECT_EQ(v, a);
    beint64 c(a);  // copy constructor
    EXPECT_EQ(b, c);

    v = 2;
    b = v;  // assignment from std type
    EXPECT_EQ(v, b);

    v = 0x0011223344556677l;
    a = v;

    uint8_t* mem = reinterpret_cast<uint8_t*>(&a);
    EXPECT_EQ(mem[0], 0x00);
    EXPECT_EQ(mem[1], 0x11);
    EXPECT_EQ(mem[2], 0x22);
    EXPECT_EQ(mem[3], 0x33);
    EXPECT_EQ(mem[4], 0x44);
    EXPECT_EQ(mem[5], 0x55);
    EXPECT_EQ(mem[6], 0x66);
    EXPECT_EQ(mem[7], 0x77);
}

TEST_F(BigEndianIntTest, testUInt64)
{
    beuint64 a;  // default constructor
    uint64_t v = 1;
    beuint64 b(v);  // constructor from value
    a = b;          // same assignment
    EXPECT_EQ(v, a);
    beuint64 c(a);  // copy constructor
    EXPECT_EQ(b, c);

    v = 2;
    b = v;  // assignment from std type
    EXPECT_EQ(v, b);

    v = 0x0011223344556677l;
    a = v;

    uint8_t* mem = reinterpret_cast<uint8_t*>(&a);
    EXPECT_EQ(mem[0], 0x00);
    EXPECT_EQ(mem[1], 0x11);
    EXPECT_EQ(mem[2], 0x22);
    EXPECT_EQ(mem[3], 0x33);
    EXPECT_EQ(mem[4], 0x44);
    EXPECT_EQ(mem[5], 0x55);
    EXPECT_EQ(mem[6], 0x66);
    EXPECT_EQ(mem[7], 0x77);
}

TEST_F(BigEndianIntTest, testInt32)
{
    beint32 a;  // default constructor
    int32_t v = 1;
    beint32 b(v);  // constructor from value
    a = b;         // same assignment
    EXPECT_EQ(v, a);
    beint32 c(a);  // copy constructor
    EXPECT_EQ(b, c);

    v = 2;
    b = v;  // assignment from std type
    EXPECT_EQ(v, b);

    v = 0x00112233;
    a = v;

    uint8_t* mem = reinterpret_cast<uint8_t*>(&a);
    EXPECT_EQ(mem[0], 0x00);
    EXPECT_EQ(mem[1], 0x11);
    EXPECT_EQ(mem[2], 0x22);
    EXPECT_EQ(mem[3], 0x33);
}

TEST_F(BigEndianIntTest, testUInt32)
{
    beuint32 a;  // default constructor
    uint32_t v = 1;
    beuint32 b(v);  // constructor from value
    a = b;          // same assignment
    EXPECT_EQ(v, a);
    beuint32 c(a);  // copy constructor
    EXPECT_EQ(b, c);

    v = 2;
    b = v;  // assignment from std type
    EXPECT_EQ(v, b);

    v = 0x00112233;
    a = v;

    uint8_t* mem = reinterpret_cast<uint8_t*>(&a);
    EXPECT_EQ(mem[0], 0x00);
    EXPECT_EQ(mem[1], 0x11);
    EXPECT_EQ(mem[2], 0x22);
    EXPECT_EQ(mem[3], 0x33);
}

TEST_F(BigEndianIntTest, testInt16)
{
    beint16 a;  // default constructor
    int16_t v = 1;
    beint16 b(v);  // constructor from value
    a = b;         // same assignment
    EXPECT_EQ(v, a);
    beint16 c(a);  // copy constructor
    EXPECT_EQ(b, c);

    v = 2;
    b = v;  // assignment from std type
    EXPECT_EQ(v, b);

    v = 0x0011;
    a = v;

    uint8_t* mem = reinterpret_cast<uint8_t*>(&a);
    EXPECT_EQ(mem[0], 0x00);
    EXPECT_EQ(mem[1], 0x11);
}

TEST_F(BigEndianIntTest, testUInt16)
{
    beuint16 a;  // default constructor
    uint16_t v = 1;
    beuint16 b(v);  // constructor from value
    a = b;          // same assignment
    EXPECT_EQ(v, a);
    beuint16 c(a);  // copy constructor
    EXPECT_EQ(b, c);

    v = 2;
    b = v;  // assignment from std type
    EXPECT_EQ(v, b);

    v = 0x0011;
    a = v;

    uint8_t* mem = reinterpret_cast<uint8_t*>(&a);
    EXPECT_EQ(mem[0], 0x00);
    EXPECT_EQ(mem[1], 0x11);
}
