/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include <outpost/utils/storage/serialize.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace outpost;

TEST(DeserialzeTest, getPosition)
{
    uint8_t data[32] = {};

    Deserialize payload(data);

    EXPECT_EQ(0, payload.getPosition());

    payload.read<uint8_t>();
    EXPECT_EQ(1, payload.getPosition());

    payload.read<uint16_t>();
    EXPECT_EQ(3, payload.getPosition());

    payload.read<uint8_t>();
    EXPECT_EQ(4, payload.getPosition());

    payload.read<uint32_t>();
    EXPECT_EQ(8, payload.getPosition());

    payload.read<uint8_t>();
    EXPECT_EQ(9, payload.getPosition());

    payload.read<uint32_t>();
    EXPECT_EQ(13, payload.getPosition());

    payload.read<float>();
    EXPECT_EQ(17, payload.getPosition());

    payload.read<double>();
    EXPECT_EQ(25, payload.getPosition());
}

TEST(DeserialzeTest, skipPositions)
{
    uint8_t data[32] = {};

    Deserialize payload(data);

    payload.skip(1);
    EXPECT_EQ(1, payload.getPosition());

    payload.skip(2);
    EXPECT_EQ(3, payload.getPosition());

    payload.skip<uint8_t>();
    EXPECT_EQ(4, payload.getPosition());

    payload.skip<uint32_t>();
    EXPECT_EQ(8, payload.getPosition());

    payload.skip(1);
    EXPECT_EQ(9, payload.getPosition());

    payload.skip(4);
    EXPECT_EQ(13, payload.getPosition());

    payload.skip(12);
    EXPECT_EQ(25, payload.getPosition());
}

TEST(DeserialzeTest, shouldReadData)
{
    uint8_t data[18] = {0xAB,
                        0xEF,
                        0x12,
                        0xA6,
                        0xC0,
                        0x1A,
                        0x61,
                        0xA9,
                        0xF5,
                        0x1E,
                        0xAE,
                        0x5F,
                        0xA2,
                        0xB0,
                        0x00,
                        0x7D,
                        0x32,
                        0xC2};

    Deserialize payload(data);

    uint8_t d8 = payload.read<uint8_t>();
    EXPECT_EQ(0xAB, d8);
    EXPECT_EQ(1, payload.getPosition());

    uint16_t d16 = payload.read<uint16_t>();
    EXPECT_EQ(0xEF12, d16);
    EXPECT_EQ(3, payload.getPosition());

    uint32_t d24 = payload.read24();
    EXPECT_EQ(0xA6C01AU, d24);
    EXPECT_EQ(6, payload.getPosition());

    uint32_t d32 = payload.read<uint32_t>();
    EXPECT_EQ(0x61A9F51EUL, d32);
    EXPECT_EQ(10, payload.getPosition());

    uint64_t d64 = payload.read<uint64_t>();
    EXPECT_EQ(0xAE5FA2B0007D32C2ULL, d64);
    EXPECT_EQ(18, payload.getPosition());
}

TEST(DeserializeTest, shouldReadPackedData)
{
    uint8_t data[6] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};

    Deserialize payload(data);

    uint16_t p1;
    uint16_t p2;
    uint16_t p3;
    uint16_t p4;

    payload.readPacked12(p1, p2);
    payload.readPacked12(p3, p4);

    EXPECT_EQ(0x123U, p1);
    EXPECT_EQ(0x456U, p2);

    EXPECT_EQ(0x789U, p3);
    EXPECT_EQ(0xABCU, p4);
}

TEST(DeserializeTest, shouldPeekPackedData)
{
    uint8_t data[6] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC};

    Deserialize payload(data);

    uint16_t p1;
    uint16_t p2;

    payload.peekPacked12(1, p1, p2);

    EXPECT_EQ(0x345U, p1);
    EXPECT_EQ(0x678U, p2);
}

TEST(DeserialzeTest, peekFloat)
{
    uint8_t data[4] = {0x40, 0x49, 0x0F, 0xD0};

    Deserialize payload(data);

    float f = payload.peek<float>(0);

    EXPECT_FLOAT_EQ(3.14159f, f);
    EXPECT_EQ(0, payload.getPosition());
}

TEST(DeserialzeTest, peekDouble)
{
    uint8_t data[8] = {0x40, 0x09, 0x21, 0xFB, 0x54, 0x44, 0x2D, 0x18};

    Deserialize payload(data);

    double d = payload.peek<double>(0);

    EXPECT_DOUBLE_EQ(3.1415926535897931, d);
    EXPECT_EQ(0, payload.getPosition());
}

TEST(DeserialzeTest, readFloat)
{
    uint8_t data[4] = {0x40, 0x49, 0x0F, 0xD0};

    Deserialize payload(data);

    float f = payload.read<float>();

    EXPECT_FLOAT_EQ(3.14159f, f);
    EXPECT_EQ(4, payload.getPosition());
}

TEST(DeserialzeTest, readDouble)
{
    uint8_t data[8] = {0x40, 0x09, 0x21, 0xFB, 0x54, 0x44, 0x2D, 0x18};

    Deserialize payload(data);

    double d = payload.read<double>();

    EXPECT_DOUBLE_EQ(3.1415926535897931, d);
    EXPECT_EQ(8, payload.getPosition());
}

TEST(DeserialzeTest, peekTemplate)
{
    uint8_t data[18] = {0xAB,
                        0xEF,
                        0x12,
                        0xA6,
                        0xC0,
                        0x1A,
                        0x61,
                        0xA9,
                        0xF5,
                        0x1E,
                        0xAE,
                        0x5F,
                        0xA2,
                        0xB0,
                        0x00,
                        0x7D,
                        0x32,
                        0xC2};

    Deserialize payload(data);

    uint8_t d8 = payload.peek<uint8_t>(0);
    EXPECT_EQ(0xAB, d8);

    uint16_t d16 = payload.peek<uint16_t>(1);
    EXPECT_EQ(0xEF12, d16);

    uint32_t d24 = payload.peek24(3);
    EXPECT_EQ(0xA6C01AU, d24);

    uint32_t d32 = payload.peek<uint32_t>(6);
    EXPECT_EQ(0x61A9F51EUL, d32);

    uint64_t d64 = payload.peek<uint64_t>(10);
    EXPECT_EQ(0xAE5FA2B0007D32C2ULL, d64);
}

TEST(DeserialzeTest, readTemplate)
{
    uint8_t data[18] = {0xAB,
                        0xEF,
                        0x12,
                        0x61,
                        0xA9,
                        0xF5,
                        0x1E,
                        0xAE,
                        0x5F,
                        0xA2,
                        0xB0,
                        0x00,
                        0x7D,
                        0x32,
                        0xC2};

    Deserialize payload(data);

    uint8_t d8 = payload.read<uint8_t>();
    EXPECT_EQ(0xAB, d8);

    uint16_t d16 = payload.read<uint16_t>();
    EXPECT_EQ(0xEF12, d16);

    uint32_t d32 = payload.read<uint32_t>();
    EXPECT_EQ(0x61A9F51EUL, d32);

    uint64_t d64 = payload.read<uint64_t>();
    EXPECT_EQ(0xAE5FA2B0007D32C2ULL, d64);
}
