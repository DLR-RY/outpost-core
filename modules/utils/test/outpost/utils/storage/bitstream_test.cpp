/*
 * Copyright (c) 2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#include <outpost/utils/storage/bitstream.h>
#include <outpost/utils/storage/serialize.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unittest/harness.h>

using namespace outpost;

constexpr uint16_t ARRAY_LENGTH = 256U;

uint8_t ref[ARRAY_LENGTH];

uint8_t buffer_in[ARRAY_LENGTH];
outpost::Slice<uint8_t> data_in(buffer_in);

uint8_t buffer_out[ARRAY_LENGTH];
outpost::Slice<uint8_t> data_out(buffer_out);

TEST(BitstreamTest, constructor)
{
    memset(ref, 0, ARRAY_LENGTH);
    memset(buffer_in, 0, ARRAY_LENGTH);

    outpost::Bitstream bitstream(data_in);
    EXPECT_EQ(bitstream.getSize(), 0U);
    EXPECT_EQ(bitstream.getSerializedSize(), 3U);
    EXPECT_FALSE(bitstream.isFull());

    EXPECT_ARRAY_EQ(uint8_t, ref, buffer_in, ARRAY_LENGTH);
}

TEST(BitstreamTest, isFull)
{
    memset(buffer_in, 0, ARRAY_LENGTH);

    outpost::Bitstream bitstream(data_in);
    for (uint16_t i = 0; i < ARRAY_LENGTH - 3; i++)
    {
        for (uint16_t j = 0; j < 8; j++)
        {
            EXPECT_FALSE(bitstream.isFull());
            bitstream.pushBit(true);
        }
        ref[i] = 0xFF;
    }
    EXPECT_TRUE(bitstream.isFull());
}

TEST(BistreamTest, push)
{
    memset(buffer_in, 0, ARRAY_LENGTH);
    memset(ref, 0, ARRAY_LENGTH);
    outpost::Bitstream bitstream(data_in);

    bitstream.pushBit(true);

    EXPECT_EQ(bitstream.getSize(), 1U);
    EXPECT_EQ(bitstream.getSerializedSize(), 4U);

    bitstream.pushBit(false);
    bitstream.pushBit(true);
    bitstream.pushBit(false);
    bitstream.pushBit(true);
    bitstream.pushBit(false);
    bitstream.pushBit(true);
    bitstream.pushBit(false);

    EXPECT_EQ(bitstream.getSize(), 1U);
    EXPECT_EQ(bitstream.getSerializedSize(), 4U);

    EXPECT_EQ(bitstream.getByte(0), 0xAA);

    bitstream.pushBit(true);

    EXPECT_EQ(bitstream.getSize(), 2U);
    EXPECT_EQ(bitstream.getSerializedSize(), 5U);

    ref[3] = 0xAA;
    ref[4] = 0x80;

    EXPECT_ARRAY_EQ(uint8_t, ref, buffer_in, ARRAY_LENGTH);
}

TEST(BistreamTest, getBit)
{
    memset(buffer_in, 0, ARRAY_LENGTH);
    outpost::Bitstream bitstream(data_in);

    bitstream.pushBit(true);
    bitstream.pushBit(false);
    bitstream.pushBit(true);
    bitstream.pushBit(false);
    bitstream.pushBit(true);
    bitstream.pushBit(false);
    bitstream.pushBit(true);
    bitstream.pushBit(false);
    bitstream.pushBit(true);

    EXPECT_EQ(bitstream.getBit(0), true);
    EXPECT_EQ(bitstream.getBit(1), false);
    EXPECT_EQ(bitstream.getBit(2), true);
    EXPECT_EQ(bitstream.getBit(3), false);
    EXPECT_EQ(bitstream.getBit(4), true);
    EXPECT_EQ(bitstream.getBit(5), false);
    EXPECT_EQ(bitstream.getBit(6), true);
    EXPECT_EQ(bitstream.getBit(7), false);
    EXPECT_EQ(bitstream.getBit(8), true);

    EXPECT_EQ(bitstream.getBit(256U), false);  // OUT OF CURRENT BITSTREAM'S BOUNDS

    EXPECT_EQ(bitstream.getBit(314U << 3), false);  // OUT OF ARRAY BOUNDS
}

TEST(BistreamTest, serialize)
{
    memset(ref, 0, ARRAY_LENGTH);
    memset(buffer_in, 0, ARRAY_LENGTH);
    memset(buffer_out, 0, ARRAY_LENGTH);
    outpost::Bitstream bitstream(data_in);

    for (uint16_t i = 0; i < 25; i++)
    {
        // 0xA5
        bitstream.pushBit(true);
        bitstream.pushBit(false);
        bitstream.pushBit(true);
        bitstream.pushBit(false);
        bitstream.pushBit(false);
        bitstream.pushBit(true);
        bitstream.pushBit(false);
        bitstream.pushBit(true);

        ref[3 + i] = 0xA5;
    }
    ref[0] = 0;
    ref[1] = 28;
    ref[2] = 7;

    EXPECT_EQ(bitstream.getSize(), 25U);
    EXPECT_EQ(bitstream.getSerializedSize(), 28U);

    outpost::Serialize stream(data_in);
    bitstream.serialize(stream);

    EXPECT_ARRAY_EQ(uint8_t, ref, buffer_in, ARRAY_LENGTH);

    outpost::Serialize stream_ext(data_out);
    bitstream.serialize(stream_ext);
    EXPECT_ARRAY_EQ(uint8_t, ref, buffer_out, ARRAY_LENGTH);
}

TEST(BistreamTest, serializeAndCut)
{
    memset(ref, 0, ARRAY_LENGTH);
    memset(buffer_in, 0, ARRAY_LENGTH);
    memset(buffer_out, 0, ARRAY_LENGTH);
    outpost::Bitstream bitstream(data_in);

    for (uint16_t i = 0; i < 25; i++)
    {
        // 0xA5
        bitstream.pushBit(true);
        bitstream.pushBit(false);
        bitstream.pushBit(true);
        bitstream.pushBit(false);
        bitstream.pushBit(false);
        bitstream.pushBit(true);
        bitstream.pushBit(false);
        bitstream.pushBit(true);
    }
    ref[0] = 0;
    ref[1] = 28;
    ref[2] = 7;
    ref[3] = 0xA5;

    EXPECT_EQ(bitstream.getSize(), 25U);
    EXPECT_EQ(bitstream.getSerializedSize(), 28U);

    outpost::Serialize stream(data_in);
    bitstream.serialize(stream, ARRAY_LENGTH * 2U);

    EXPECT_ARRAY_EQ(uint8_t, ref, buffer_in, 4U);

    outpost::Serialize stream_ext(data_out);
    bitstream.serialize(stream_ext, 4);
    ref[1] = 5;
    EXPECT_ARRAY_EQ(uint8_t, ref, buffer_out, ARRAY_LENGTH);
}

TEST(BistreamTest, deserialize)
{
    memset(buffer_in, 0, ARRAY_LENGTH);
    memset(buffer_out, 0, ARRAY_LENGTH);

    buffer_in[0] = 0;
    buffer_in[1] = 28;
    buffer_in[2] = 7;

    for (uint16_t i = 3; i < 28; i++)
    {
        buffer_in[i] = 0xAA;
    }

    outpost::Bitstream bitstream(data_in);
    outpost::Deserialize stream(buffer_in);
    bitstream.deserialize(stream);

    EXPECT_FALSE(bitstream.isFull());
    EXPECT_EQ(bitstream.getSize(), 25U);

    for (uint16_t i = 0; i < 25; i++)
    {
        EXPECT_EQ(bitstream.getByte(i), 0xAA);
    }
    EXPECT_EQ(bitstream.getByte(25), 0);

    outpost::Bitstream bitstream_copy(data_out);
    stream.reset();
    bitstream_copy.deserialize(stream);

    EXPECT_EQ(bitstream_copy.getSize(), 25U);
    EXPECT_EQ(bitstream_copy.getSerializedSize(), 28U);

    EXPECT_ARRAY_EQ(uint8_t, &buffer_in[3], &buffer_out[3], ARRAY_LENGTH - 3U);
}

TEST(BitstreamTest, deserializeFail)
{
    memset(buffer_in, 0, ARRAY_LENGTH);
    memset(buffer_out, 0, ARRAY_LENGTH);

    buffer_in[0] = 0;
    buffer_in[1] = 28;
    buffer_in[2] = 7;

    for (uint16_t i = 3; i < 28; i++)
    {
        buffer_in[i] = 0xAA;
    }

    outpost::Slice<uint8_t> shortBuffer = data_out.first(10);
    outpost::Deserialize stream(buffer_in);

    {
        outpost::Bitstream bitstream(shortBuffer);
        EXPECT_FALSE(bitstream.deserialize(stream));

        EXPECT_EQ(bitstream.getSize(), 0U);
        EXPECT_EQ(bitstream.getSerializedSize(), 3U);
    }
}
