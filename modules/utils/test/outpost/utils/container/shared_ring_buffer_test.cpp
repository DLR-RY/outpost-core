/*
 * Copyright (c) 2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Felix Passenberg(DLR RY-AVS)
 */

#include <outpost/rtos/thread.h>
#include <outpost/utils/container/shared_buffer.h>
#include <outpost/utils/container/shared_object_pool.h>
#include <outpost/utils/container/shared_ring_buffer.h>
#include <outpost/utils/storage/serialize.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

static constexpr size_t poolSize = 15;
static constexpr size_t slots = 10;
static constexpr size_t objectSize = 160;

class SharedRingBufferTest : public testing::Test
{
public:
    outpost::utils::SharedBufferPool<objectSize, poolSize> mPool;
    outpost::utils::SharedRingBufferStorage<slots> mRing;

    SharedRingBufferTest()
    {
    }

    virtual void
    SetUp()
    {
    }

    virtual void
    TearDown()
    {
    }
};

TEST_F(SharedRingBufferTest, peekEmpty)
{
    outpost::utils::SharedBufferPointer p;
    p = mRing.peek();
    EXPECT_FALSE(p.isValid());

    p = mRing.peek(4096);
    EXPECT_FALSE(p.isValid());
}

TEST_F(SharedRingBufferTest, resetRingbuffer)
{
    mRing.reset();
    EXPECT_TRUE(mRing.isEmpty());
    EXPECT_EQ(mRing.getFreeSlots(), slots);
    EXPECT_EQ(mRing.getUsedSlots(), 0u);

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
    mRing.append(p);

    EXPECT_FALSE(mRing.isEmpty());
    EXPECT_EQ(mRing.getFreeSlots(), slots - 1);
    EXPECT_EQ(mRing.getUsedSlots(), 1u);

    mRing.reset();
    EXPECT_TRUE(mRing.isEmpty());
    EXPECT_EQ(mRing.getFreeSlots(), slots);
    EXPECT_EQ(mRing.getUsedSlots(), 0u);
}

TEST_F(SharedRingBufferTest, queueBuffer)
{
    EXPECT_TRUE(mRing.isEmpty());
    EXPECT_EQ(mRing.getFreeSlots(), slots);
    EXPECT_EQ(mRing.getUsedSlots(), 0u);

    {  // scoping lifetime of shared buffers
        outpost::utils::SharedBufferPointer p1;
        ASSERT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            p1[i] = 0x01;
        }
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        EXPECT_TRUE(mRing.append(p1));
        EXPECT_FALSE(mRing.isEmpty());
        EXPECT_EQ(mRing.getFreeSlots(), slots - 1);
        EXPECT_EQ(mRing.getUsedSlots(), 1u);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    {
        outpost::utils::SharedBufferPointer p1;
        ASSERT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());

        for (size_t i = 0; i < objectSize; i++)
        {
            p1[i] = 0x02;
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
        EXPECT_TRUE(mRing.append(p1, 0x11));
        EXPECT_FALSE(mRing.isEmpty());
        EXPECT_EQ(mRing.getFreeSlots(), slots - 2);
        EXPECT_EQ(mRing.getUsedSlots(), 2u);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);

    {
        outpost::utils::SharedBufferPointer p2;
        // checking peek
        p2 = mRing.peek();
        EXPECT_TRUE(p2.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            EXPECT_EQ(p2[i], 0x01);
        }
        // checking peek with offset
        p2 = mRing.peek(1);
        EXPECT_TRUE(p2.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            EXPECT_EQ(p2[i], 0x02);
        }
        EXPECT_EQ(mRing.peekFlags(), 0x00);
        EXPECT_EQ(mRing.peekFlags(mRing.getUsedSlots() - 1), 0x11);
        EXPECT_TRUE(mRing.setFlags(0xef, mRing.getUsedSlots() - 1));

        // check removal of the first element
        p2 = mRing.peek();
        EXPECT_TRUE(mRing.pop());
        EXPECT_EQ(mRing.peekFlags(), 0xef);
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
        EXPECT_FALSE(mRing.isEmpty());
    }
    // p2 has been freed now
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

    {
        outpost::utils::SharedBufferPointer p2;
        p2 = mRing.peek(0);

        ASSERT_TRUE(p2.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            EXPECT_EQ(p2[i], 0x02);
        }
        EXPECT_EQ(mRing.peekFlags(), 0xef);
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        EXPECT_TRUE(mRing.pop());
        EXPECT_TRUE(mRing.isEmpty());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);

    {  // nothing to get from the queue
        outpost::utils::SharedBufferPointer p2;
        p2 = mRing.peek();

        ASSERT_TRUE(!p2.isValid());
        EXPECT_EQ(mRing.peekFlags(), 0x00);
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
        EXPECT_FALSE(mRing.pop());
        EXPECT_TRUE(mRing.isEmpty());
    }
}

TEST_F(SharedRingBufferTest, indexOutOfBounds)
{
    EXPECT_EQ(mRing.peekFlags(0), 0U);
    EXPECT_EQ(mRing.peekFlags(mRing.getFreeSlots() - 1), 0U);

    EXPECT_FALSE(mRing.setFlags(0xff, 0));
    EXPECT_EQ(mRing.peekFlags(0), 0U);
    EXPECT_FALSE(mRing.setFlags(0xff, mRing.getFreeSlots()));
    EXPECT_EQ(mRing.peekFlags(mRing.getFreeSlots()), 0U);

    EXPECT_EQ(mRing.peek(), outpost::utils::SharedBufferPointer());
    EXPECT_EQ(mRing.peek(mRing.getFreeSlots() - 1), outpost::utils::SharedBufferPointer());
    EXPECT_EQ(mRing.peek(mRing.getFreeSlots()), outpost::utils::SharedBufferPointer());
}
