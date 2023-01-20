/*
 * Copyright (c) 2014-2015, 2017, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the development version of OUTPOST.
 *
 * May contain parts that are subject to export control under
 * the terms of AL Nr.: 9D005 (EG Nr. 428/2009 Dual Use) for
 * export to countries outside the EU.
 *
 * Authors:
 * - 2014-2015, 2017, Fabian Greif (DLR RY-AVS)
 */

/**
 * \file
 * \brief   Test RingBuffer functions
 *
 * \author  Fabian Greif
 */

#include <outpost/utils/container/byte_array_ring_buffer.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace outpost::utils;

class ByteArrayRingBufferTest : public testing::Test
{
public:
    static const size_t maximumElementLength = 4;
    static const size_t numberOfElements = 10;

    void
    fillWithTestData(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            defaultData[0] = i;
            defaultData[1] = i + 10;
            EXPECT_TRUE(buffer.append(outpost::asSlice(defaultData).first(2)));
        }
    }

    ByteArrayRingBufferStorage<maximumElementLength, numberOfElements> buffer;

    uint8_t defaultData[maximumElementLength];
};

const size_t ByteArrayRingBufferTest::maximumElementLength;
const size_t ByteArrayRingBufferTest::numberOfElements;

TEST_F(ByteArrayRingBufferTest, initialValues)
{
    EXPECT_EQ(numberOfElements, buffer.getFreeSlots());
    EXPECT_EQ(maximumElementLength, buffer.getElementSize());
    EXPECT_EQ(0U, buffer.getUsedSlots());
    EXPECT_TRUE(buffer.isEmpty());
}

TEST_F(ByteArrayRingBufferTest, rejectOverlongData)
{
    uint8_t data[maximumElementLength + 1];
    EXPECT_FALSE(buffer.append(outpost::asSlice(data)));
}

TEST_F(ByteArrayRingBufferTest, acceptShorterData)
{
    EXPECT_TRUE(buffer.append(outpost::asSlice(defaultData).first(maximumElementLength - 1)));
    EXPECT_TRUE(buffer.append(outpost::asSlice(defaultData).first(maximumElementLength)));
}

TEST_F(ByteArrayRingBufferTest, writeIncressesElementCount)
{
    EXPECT_TRUE(buffer.append(outpost::asSlice(defaultData)));
    EXPECT_TRUE(buffer.append(outpost::asSlice(defaultData)));
    EXPECT_EQ(2U, buffer.getUsedSlots());
}

TEST_F(ByteArrayRingBufferTest, appendIsRejectedAfterBufferIsFull)
{
    for (size_t i = 0; i < numberOfElements; ++i)
    {
        EXPECT_TRUE(buffer.append(outpost::asSlice(defaultData)));
    }
    EXPECT_EQ(numberOfElements, buffer.getUsedSlots());

    EXPECT_FALSE(buffer.append(outpost::asSlice(defaultData)));
    EXPECT_EQ(numberOfElements, buffer.getUsedSlots());
}

TEST_F(ByteArrayRingBufferTest, retrieveAndVerifyElements)
{
    fillWithTestData(5);

    for (size_t i = 0; i < 5; ++i)
    {
        ASSERT_FALSE(buffer.isEmpty());
        outpost::Slice<const uint8_t> element = buffer.read();
        EXPECT_EQ(2U, element.getNumberOfElements());

        EXPECT_EQ(i, element[0]);
        EXPECT_EQ(i + 10, element[1]);
        EXPECT_TRUE(buffer.pop());
    }
    EXPECT_TRUE(buffer.isEmpty());
}

TEST_F(ByteArrayRingBufferTest, peek)
{
    fillWithTestData(5);

    for (size_t i = 0; i < 5; ++i)
    {
        outpost::Slice<const uint8_t> element = buffer.peek(i);
        EXPECT_EQ(2U, element.getNumberOfElements());

        EXPECT_EQ(i, element[0]);
        EXPECT_EQ(i + 10, element[1]);
    }
}

TEST_F(ByteArrayRingBufferTest, invalidPeekValueReturnsZero)
{
    EXPECT_EQ(0U, buffer.peek(100000).getNumberOfElements());
}

TEST_F(ByteArrayRingBufferTest, resetDropsAllContent)
{
    fillWithTestData(5);

    buffer.reset();

    EXPECT_EQ(numberOfElements, buffer.getFreeSlots());
    EXPECT_EQ(0U, buffer.getUsedSlots());
    EXPECT_TRUE(buffer.isEmpty());
    EXPECT_EQ(0U, buffer.peek(1).getNumberOfElements());
}

TEST_F(ByteArrayRingBufferTest, resetAfterDropsSomeContent)
{
    fillWithTestData(5);

    buffer.resetElementsFrom(3);

    // Elements 0, 1 and 2 are left => 3 elements
    EXPECT_EQ(3U, buffer.getUsedSlots());
    EXPECT_FALSE(buffer.isEmpty());
    EXPECT_EQ(0U, buffer.peek(4).getNumberOfElements());
}

TEST_F(ByteArrayRingBufferTest, resetBuffers)
{
    fillWithTestData(10);

    EXPECT_EQ(0U, buffer.getFreeSlots());

    buffer.pop();
    uint8_t data[1] = {0xff};
    EXPECT_TRUE(buffer.append(outpost::asSlice(data), 0, false));

    outpost::Slice<const uint8_t> temp = buffer.peek(9);
    EXPECT_EQ(temp[0], 0xff);
    EXPECT_EQ(temp[1], 0x0a);

    buffer.pop();
    EXPECT_TRUE(buffer.append(outpost::asSlice(data)));
    temp = buffer.peek(9);
    EXPECT_EQ(temp[0], 0xff);
    EXPECT_EQ(temp[1], 0x00);
}
