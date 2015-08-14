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

#include <cobc/utils/serialize.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace cobc;

TEST(SerialzeTest, store)
{
    uint8_t data[16] = { 0, };

    EXPECT_EQ(0, data[0]);

    Serialize payload(data);

    payload.store8(0xab);

    EXPECT_EQ(0xab, data[0]);

    payload.store16(0xef12);

    EXPECT_EQ(0xef, data[1]);
    EXPECT_EQ(0x12, data[2]);

    payload.store24(0xa6c01a);

    EXPECT_EQ(0xa6, data[3]);
    EXPECT_EQ(0xc0, data[4]);
    EXPECT_EQ(0x1a, data[5]);

    payload.store32(0x61a9f51e);

    EXPECT_EQ(0x61, data[6]);
    EXPECT_EQ(0xa9, data[7]);
    EXPECT_EQ(0xf5, data[8]);
    EXPECT_EQ(0x1e, data[9]);
}

TEST(SerializeTest, shouldStorePackedData)
{
    uint8_t data[6] = { 0, };

    Serialize payload(data);

    payload.storePacked12(0x123, 0x456);
    payload.storePacked12(0x789, 0xABC);

    EXPECT_EQ(0x12, data[0]);
    EXPECT_EQ(0x34, data[1]);
    EXPECT_EQ(0x56, data[2]);

    EXPECT_EQ(0x78, data[3]);
    EXPECT_EQ(0x9A, data[4]);
    EXPECT_EQ(0xBC, data[5]);
}

TEST(SerialzeTest, storeTemplateExplicit)
{
    uint8_t data[20] = { 0, };

    EXPECT_EQ(0, data[0]);

    Serialize payload(data);

    payload.store<uint8_t>(0xab);

    EXPECT_EQ(0xab, data[0]);

    payload.store<uint16_t>(0xef12);

    EXPECT_EQ(0xef, data[1]);
    EXPECT_EQ(0x12, data[2]);

    payload.store<uint32_t>(0x61a9f51e);

    EXPECT_EQ(0x61, data[3]);
    EXPECT_EQ(0xa9, data[4]);
    EXPECT_EQ(0xf5, data[5]);
    EXPECT_EQ(0x1e, data[6]);

    float f = 3.14159f;
    payload.store<float>(f);

    EXPECT_EQ(0x40, data[7]);
    EXPECT_EQ(0x49, data[8]);
    EXPECT_EQ(0x0F, data[9]);
    EXPECT_EQ(0xD0, data[10]);

    double d = 3.1415926535897931;
    payload.store<double>(d);

    EXPECT_EQ(0x40, data[11]);
    EXPECT_EQ(0x09, data[12]);
    EXPECT_EQ(0x21, data[13]);
    EXPECT_EQ(0xFB, data[14]);
    EXPECT_EQ(0x54, data[15]);
    EXPECT_EQ(0x44, data[16]);
    EXPECT_EQ(0x2D, data[17]);
    EXPECT_EQ(0x18, data[18]);

    EXPECT_EQ(19, payload.getPosition());
}

TEST(SerialzeTest, storeTemplateImplicit)
{
    uint8_t data[20] = { 0, };

    EXPECT_EQ(0, data[0]);

    Serialize payload(data);

    uint8_t u8 = 0xab;
    payload.store(u8);

    EXPECT_EQ(0xab, data[0]);
    EXPECT_EQ(1, payload.getPosition());

    uint16_t u16 = 0xef12;
    payload.store(u16);

    EXPECT_EQ(0xef, data[1]);
    EXPECT_EQ(0x12, data[2]);
    EXPECT_EQ(3, payload.getPosition());

    uint32_t u32 = 0x61a9f51e;
    payload.store(u32);

    EXPECT_EQ(0x61, data[3]);
    EXPECT_EQ(0xa9, data[4]);
    EXPECT_EQ(0xf5, data[5]);
    EXPECT_EQ(0x1e, data[6]);
    EXPECT_EQ(7, payload.getPosition());

    float f = 3.14159f;
    payload.store(f);

    EXPECT_EQ(0x40, data[7]);
    EXPECT_EQ(0x49, data[8]);
    EXPECT_EQ(0x0F, data[9]);
    EXPECT_EQ(0xD0, data[10]);
    EXPECT_EQ(11, payload.getPosition());

    double d = 3.1415926535897931;
    payload.store(d);

    EXPECT_EQ(0x40, data[11]);
    EXPECT_EQ(0x09, data[12]);
    EXPECT_EQ(0x21, data[13]);
    EXPECT_EQ(0xFB, data[14]);
    EXPECT_EQ(0x54, data[15]);
    EXPECT_EQ(0x44, data[16]);
    EXPECT_EQ(0x2D, data[17]);
    EXPECT_EQ(0x18, data[18]);

    EXPECT_EQ(19, payload.getPosition());
}

TEST(SerializeTest, storeFloat)
{
    float f = 3.14159f;

    uint8_t data[4];

    Serialize payload(data);

    payload.storeFloat(f);

    EXPECT_EQ(0x40, data[0]);
    EXPECT_EQ(0x49, data[1]);
    EXPECT_EQ(0x0F, data[2]);
    EXPECT_EQ(0xD0, data[3]);

    EXPECT_EQ(4, payload.getPosition());
}

TEST(SerializeTest, storeDouble)
{
    double d = 3.1415926535897931;

    uint8_t data[8];

    Serialize payload(data);

    payload.storeDouble(d);

    EXPECT_EQ(0x40, data[0]);
    EXPECT_EQ(0x09, data[1]);
    EXPECT_EQ(0x21, data[2]);
    EXPECT_EQ(0xFB, data[3]);
    EXPECT_EQ(0x54, data[4]);
    EXPECT_EQ(0x44, data[5]);
    EXPECT_EQ(0x2D, data[6]);
    EXPECT_EQ(0x18, data[7]);

    EXPECT_EQ(8, payload.getPosition());
}
