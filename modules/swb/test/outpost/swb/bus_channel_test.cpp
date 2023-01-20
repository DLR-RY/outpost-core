/*
 * Copyright (c) 2017-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan-Gerd Mess (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/swb/bus_channel.h>
#include <outpost/swb/types.h>
#include <outpost/utils/container/shared_object_pool.h>

#include <unittest/harness.h>

class BusChannelTest : public ::testing::Test
{
public:
    BusChannelTest()
    {
    }

    virtual void
    SetUp() override
    {
    }

    virtual void
    TearDown() override
    {
    }

    void
    fillBuffer(outpost::utils::SharedBufferPointer& m)
    {
        for (size_t i = 0; i < m.getLength(); i++)
        {
            m[i] = static_cast<uint8_t>(i);
        }
    }

    outpost::utils::SharedBufferPool<10, 10> mPool;
};

using namespace outpost::swb;

TEST_F(BusChannelTest, defaultTemplateTest)
{
    outpost::swb::BufferedBusChannelWithMemory<10, uint16_t> channel1;

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));

    outpost::swb::Message<uint16_t> m = {0x00, p};

    EXPECT_TRUE(channel1.matches(m));

    outpost::swb::FilterNone<uint16_t>& f = channel1.getFilter();
    EXPECT_TRUE(f(m));
}

TEST_F(BusChannelTest, constructorTest)
{
    outpost::swb::BufferedBusChannelWithMemory<10, uint16_t, SubscriptionFilter<uint16_t>> channel1;

    EXPECT_EQ(channel1.getNumberOfIncomingMessages(), 0U);
    EXPECT_EQ(channel1.getNumberOfAppendedMessages(), 0U);
    EXPECT_EQ(channel1.getNumberOfFailedReceptions(), 0U);
    EXPECT_EQ(channel1.getNumberOfRetrievedMessages(), 0U);

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
    outpost::swb::Message<uint16_t> m = {0x00, p};

    EXPECT_FALSE(channel1.matches(m));

    EXPECT_EQ(channel1.sendMessage(m), outpost::swb::OperationResult::sendFailed);
    EXPECT_EQ(channel1.receiveMessage(m, outpost::time::Duration::zero()),
              outpost::swb::OperationResult::noMessageAvailable);

    outpost::swb::BusSubscription<uint16_t> sub(0x1234);
    outpost::swb::BufferedBusChannelWithMemory<10, uint16_t, SubscriptionFilter<uint16_t>> channel2;
    channel2.getFilter().registerSubscription(sub);

    EXPECT_EQ(channel2.getNumberOfIncomingMessages(), 0U);
    EXPECT_EQ(channel2.getNumberOfAppendedMessages(), 0U);
    EXPECT_EQ(channel2.getNumberOfFailedReceptions(), 0U);
    EXPECT_EQ(channel2.getNumberOfRetrievedMessages(), 0U);
    EXPECT_EQ(channel2.getCurrentNumberOfMessages(), 0U);

    EXPECT_EQ(channel2.receiveMessage(m, outpost::time::Duration::zero()),
              outpost::swb::OperationResult::noMessageAvailable);

    m = {0x1234, p};
    EXPECT_TRUE(channel2.matches(m));
    EXPECT_EQ(channel2.sendMessage(m), outpost::swb::OperationResult::success);
}

TEST_F(BusChannelTest, subscriptionTest)
{
    outpost::swb::BufferedBusChannelWithMemory<10, uint16_t, SubscriptionFilter<uint16_t>> channel;
    outpost::swb::BusSubscription<uint16_t> sub(0x1234);

    EXPECT_EQ(channel.getFilter().getNumberOfSubscriptions(), 0U);
    channel.getFilter().registerSubscription(sub);
    EXPECT_EQ(channel.getFilter().getNumberOfSubscriptions(), 1U);

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
    outpost::swb::Message<uint16_t> m = {0x1234, p};
    EXPECT_EQ(channel.sendMessage(m), outpost::swb::OperationResult::success);
    EXPECT_EQ(channel.getCurrentNumberOfMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfIncomingMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfAppendedMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfFailedReceptions(), 0U);
    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 0U);

    m = {0x1235, p};
    EXPECT_EQ(channel.sendMessage(m), outpost::swb::OperationResult::sendFailed);
    EXPECT_EQ(channel.getCurrentNumberOfMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfIncomingMessages(), 2U);
    EXPECT_EQ(channel.getNumberOfAppendedMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfFailedReceptions(), 0U);
    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 0U);
}

TEST_F(BusChannelTest, multipleSubscriptionTest)
{
    outpost::swb::BufferedBusChannelWithMemory<10, uint16_t, SubscriptionFilter<uint16_t>> channel;
    outpost::swb::BusSubscription<uint16_t> sub1(0x1234);
    outpost::swb::BusSubscription<uint16_t> sub2(0x1234, 0xFF00);

    EXPECT_EQ(channel.getFilter().getNumberOfSubscriptions(), 0U);
    channel.getFilter().registerSubscription(sub1);
    channel.getFilter().registerSubscription(sub2);
    EXPECT_EQ(channel.getFilter().getNumberOfSubscriptions(), 2U);

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
    outpost::swb::Message<uint16_t> m = {0x1234, p};
    EXPECT_EQ(channel.sendMessage(m), outpost::swb::OperationResult::success);

    EXPECT_EQ(sub1.getNumberOfMatchedMessages(), 1U);
    EXPECT_EQ(sub2.getNumberOfMatchedMessages(), 0U);
    EXPECT_EQ(channel.getNumberOfIncomingMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfAppendedMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfFailedReceptions(), 0U);
    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 0U);

    m = {0x1235, p};
    EXPECT_EQ(channel.sendMessage(m), outpost::swb::OperationResult::success);

    EXPECT_EQ(sub1.getNumberOfMatchedMessages(), 1U);
    EXPECT_EQ(sub2.getNumberOfMatchedMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfIncomingMessages(), 2U);
    EXPECT_EQ(channel.getNumberOfAppendedMessages(), 2U);
    EXPECT_EQ(channel.getNumberOfFailedReceptions(), 0U);
    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 0U);

    m = {0x1135, p};
    EXPECT_EQ(channel.sendMessage(m), outpost::swb::OperationResult::sendFailed);
    EXPECT_EQ(sub1.getNumberOfMatchedMessages(), 1U);
    EXPECT_EQ(sub2.getNumberOfMatchedMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfIncomingMessages(), 3U);
    EXPECT_EQ(channel.getNumberOfAppendedMessages(), 2U);
    EXPECT_EQ(channel.getNumberOfFailedReceptions(), 0U);
    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 0U);
}

TEST_F(BusChannelTest, noBufferAvailableTest)
{
    outpost::swb::BusSubscription<uint16_t> sub1(0x1234);
    outpost::swb::BufferedBusChannelWithMemory<1, uint16_t, SubscriptionFilter<uint16_t>> channel;
    channel.getFilter().registerSubscription(sub1);

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
    outpost::swb::Message<uint16_t> m = {0x1234, p};
    EXPECT_EQ(channel.sendMessage(m), outpost::swb::OperationResult::success);
    EXPECT_EQ(channel.sendMessage(m), outpost::swb::OperationResult::noBufferAvailable);
    EXPECT_EQ(channel.getNumberOfIncomingMessages(), 2U);
    EXPECT_EQ(channel.getNumberOfAppendedMessages(), 1U);
    EXPECT_EQ(channel.getNumberOfFailedReceptions(), 1U);
    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 0U);
}

TEST_F(BusChannelTest, retrievalTest)
{
    outpost::swb::BusSubscription<uint16_t> sub(0x1234);
    outpost::swb::BufferedBusChannelWithMemory<5, uint16_t, SubscriptionFilter<uint16_t>> channel;
    channel.getFilter().registerSubscription(sub);

    outpost::utils::SharedBufferPointer p1;
    ASSERT_TRUE(mPool.allocate(p1));

    fillBuffer(p1);
    uint16_t id = 0x1234;
    {
        outpost::swb::Message<uint16_t> m1 = {id, p1};

        EXPECT_EQ(channel.sendMessage(m1), outpost::swb::OperationResult::success);
    }

    outpost::swb::Message<uint16_t> m2;
    EXPECT_EQ(channel.receiveMessage(m2, outpost::time::Duration::zero()),
              outpost::swb::OperationResult::success);

    EXPECT_EQ(channel.getCurrentNumberOfMessages(), 0U);
    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 1U);

    EXPECT_EQ(id, m2.id);
    ASSERT_EQ(10U, m2.buffer.getLength());

    for (size_t i = 0; i < m2.buffer.getLength(); i++)
    {
        EXPECT_EQ(i, m2.buffer[i]);
    }

    EXPECT_EQ(channel.receiveMessage(m2, outpost::time::Duration::zero()),
              outpost::swb::OperationResult::noMessageAvailable);
}

TEST_F(BusChannelTest, doubleSubscriptionTest)
{
    outpost::swb::BusSubscription<uint16_t> sub1(0x1234);
    outpost::swb::BusSubscription<uint16_t> sub2(0x1200, 0xFF00);
    outpost::swb::BufferedBusChannelWithMemory<5, uint16_t, SubscriptionFilter<uint16_t>> channel;
    channel.getFilter().registerSubscription(sub1);
    channel.getFilter().registerSubscription(sub2);

    outpost::utils::SharedBufferPointer p1;
    ASSERT_TRUE(mPool.allocate(p1));
    fillBuffer(p1);
    outpost::swb::Message<uint16_t> m1 = {0x1234, p1};

    EXPECT_EQ(channel.sendMessage(m1), outpost::swb::OperationResult::success);

    outpost::swb::Message<uint16_t> m2;
    EXPECT_EQ(channel.receiveMessage(m2, outpost::time::Duration::zero()),
              outpost::swb::OperationResult::success);

    EXPECT_EQ(channel.getNumberOfRetrievedMessages(), 1U);

    EXPECT_EQ(m1.id, m2.id);
    ASSERT_EQ(m1.buffer.getLength(), m2.buffer.getLength());
    EXPECT_ARRAY_EQ(uint8_t, &m1.buffer[0], &m2.buffer[0], m1.buffer.getLength());

    EXPECT_EQ(channel.receiveMessage(m2, outpost::time::Duration::zero()),
              outpost::swb::OperationResult::noMessageAvailable);
}

TEST_F(BusChannelTest, memoryExhaustTest)
{
    outpost::swb::BusSubscription<uint16_t> sub(0x1234);
    outpost::swb::BufferedBusChannelWithMemory<11, uint16_t, SubscriptionFilter<uint16_t>> channel;
    channel.getFilter().registerSubscription(sub);

    for (size_t n = 0; n < mPool.numberOfElements() + 1; ++n)
    {
        EXPECT_EQ(mPool.numberOfFreeElements(), mPool.numberOfElements());

        outpost::utils::SharedBufferPointer p1;
        ASSERT_TRUE(mPool.allocate(p1));

        EXPECT_EQ(mPool.numberOfFreeElements(), mPool.numberOfElements() - 1);
        fillBuffer(p1);
        outpost::swb::Message<uint16_t> m1 = {0x1234, p1};

        EXPECT_EQ(channel.sendMessage(m1), outpost::swb::OperationResult::success);

        EXPECT_EQ(mPool.numberOfFreeElements(), mPool.numberOfElements() - 1);

        outpost::swb::Message<uint16_t> m2;
        EXPECT_EQ(channel.receiveMessage(m2, outpost::time::Duration::zero()),
                  outpost::swb::OperationResult::success);

        EXPECT_EQ(channel.getNumberOfRetrievedMessages(), n + 1);
        EXPECT_EQ(mPool.numberOfFreeElements(), mPool.numberOfElements() - 1);

        EXPECT_EQ(m1.id, m2.id);
        ASSERT_EQ(m1.buffer.getLength(), m2.buffer.getLength());
        EXPECT_ARRAY_EQ(uint8_t, &m1.buffer[0], &m2.buffer[0], m1.buffer.getLength());

        EXPECT_EQ(channel.receiveMessage(m2, outpost::time::Duration::zero()),
                  outpost::swb::OperationResult::noMessageAvailable);

        EXPECT_EQ(mPool.numberOfFreeElements(), mPool.numberOfElements() - 1);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), mPool.numberOfElements());
}
