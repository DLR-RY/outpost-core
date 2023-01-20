/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Jan-Gerd Mess (DLR RY-AVS)
 */

#include <outpost/swb/default_message_filter.h>
#include <outpost/swb/software_bus.h>

#include <unittest/harness.h>
#include <unittest/swb/testing_software_bus.h>

using namespace outpost::swb;

class SoftwareBusTest : public ::testing::Test
{
public:
    SoftwareBusTest()
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

    outpost::utils::SharedBufferPool<1024, 20> mPool;
    outpost::utils::ReferenceQueue<Message<uint16_t>, 10U> mQueue;
};
using MessageId = uint16_t;

TEST_F(SoftwareBusTest, constructorTest)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendMessageWithoutChannelTest)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    outpost::utils::ConstSharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));

    Message<MessageId> message = {0, p};
    EXPECT_EQ(OperationResult::success, bus.sendMessage(message));

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendEmptyMessageNoChannel)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    outpost::utils::ConstSharedBufferPointer p;
    Message<MessageId> message = {0, p};
    EXPECT_EQ(OperationResult::success, bus.sendMessage(message));

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendMessagesNoChannel)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    for (size_t i = 0; i < 10; i++)
    {
        outpost::utils::ConstSharedBufferPointer p;
        mPool.allocate(p);
        ASSERT_TRUE(p.isValid());
        Message<uint16_t> message = {0, p};
        EXPECT_EQ(OperationResult::success, bus.sendMessage(message));
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 10U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendEmptyBufferZeroCopy)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    outpost::utils::ConstSharedBufferPointer p;
    EXPECT_EQ(mPool.numberOfFreeElements(), 20U);
    EXPECT_EQ(OperationResult::success, bus.sendMessage(0, p, CopyMode::zero_copy));
    EXPECT_EQ(mPool.numberOfFreeElements(), 20U);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendEmptyBufferCopyOnce)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    outpost::utils::ConstSharedBufferPointer p;
    EXPECT_EQ(mPool.numberOfFreeElements(), 20U);
    EXPECT_EQ(OperationResult::success, bus.sendMessage(0, p, CopyMode::copy_once));
    EXPECT_EQ(mPool.numberOfFreeElements(), 20U);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendValidBufferNoChannelZeroCopy)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    outpost::utils::ConstSharedBufferPointer p;
    mPool.allocate(p);
    ASSERT_TRUE(p.isValid());
    EXPECT_EQ(mPool.numberOfFreeElements(), 19U);
    EXPECT_EQ(OperationResult::success, bus.sendMessage(0, p, CopyMode::zero_copy));
    EXPECT_EQ(mPool.numberOfFreeElements(), 19U);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendValidBufferCopyOnce)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    outpost::utils::ConstSharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
    EXPECT_EQ(mPool.numberOfFreeElements(), 19U);
    EXPECT_EQ(OperationResult::success, bus.sendMessage(0, p, CopyMode::copy_once));
    EXPECT_EQ(mPool.numberOfFreeElements(), 18U);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendEmptySlice)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    outpost::Slice<uint8_t> emptySlice = outpost::Slice<uint8_t>::empty();
    EXPECT_EQ(OperationResult::success, bus.sendMessage(0, emptySlice));
    EXPECT_EQ(mPool.numberOfFreeElements(), 20U);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendOverlongSlice)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    uint8_t buffer[1025];
    outpost::Slice<uint8_t> overlongSlice(buffer);
    EXPECT_EQ(OperationResult::messageTooLong, bus.sendMessage(0, overlongSlice));
    EXPECT_EQ(mPool.numberOfFreeElements(), 20U);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 1U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, sendValidSlice)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    uint8_t buffer[128];
    outpost::Slice<uint8_t> slice(buffer);
    EXPECT_EQ(OperationResult::success, bus.sendMessage(0, slice));
    EXPECT_EQ(mPool.numberOfFreeElements(), 19U);

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfChannels(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDeclinedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);

    unittest::swb::TestingSoftwareBus testingBus(bus);
    testingBus.singleMessage();

    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(mPool.numberOfFreeElements(), 20U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, fullQueueNoChannel)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    for (size_t i = 0; i < 10; i++)
    {
        outpost::utils::ConstSharedBufferPointer p;
        mPool.allocate(p);
        ASSERT_TRUE(p.isValid());
        Message<MessageId> message = {0, p};
        EXPECT_EQ(OperationResult::success, bus.sendMessage(message));
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 10U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);

    outpost::utils::ConstSharedBufferPointer p;
    mPool.allocate(p);
    ASSERT_TRUE(p.isValid());
    Message<MessageId> message = {0, p};
    EXPECT_EQ(OperationResult::sendFailed, bus.sendMessage(message));

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 10U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 1U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, fullQueue)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    BufferedBusChannelWithMemory<10, MessageId> channel;
    bus.registerChannel(channel);

    for (size_t i = 0; i < 10; i++)
    {
        outpost::utils::ConstSharedBufferPointer p;
        mPool.allocate(p);
        ASSERT_TRUE(p.isValid());
        Message<MessageId> message = {0, p};
        EXPECT_EQ(OperationResult::success, bus.sendMessage(message));
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 10U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);

    outpost::utils::ConstSharedBufferPointer p;
    mPool.allocate(p);
    ASSERT_TRUE(p.isValid());
    Message<MessageId> message = {0, p};
    EXPECT_EQ(OperationResult::sendFailed, bus.sendMessage(message));

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 10U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 1U);

    unittest::swb::TestingSoftwareBus testing(bus);

    for (size_t i = 0; i < 10; i++)
    {
        testing.singleMessage();

        EXPECT_EQ(bus.getNumberOfHandledMessages(), i + 1U);
        EXPECT_EQ(bus.getNumberOfForwardedMessages(), i + 1U);
    }

    testing.singleMessage();

    EXPECT_EQ(bus.getNumberOfHandledMessages(), 10U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 10U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, noFreeBuffers)
{
    outpost::utils::SharedBufferPool<1024, 5> pool;
    SoftwareBus<MessageId> bus(
            pool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    for (size_t i = 0; i < 5; i++)
    {
        outpost::utils::ConstSharedBufferPointer p;
        mPool.allocate(p);
        ASSERT_TRUE(p.isValid());
        // cppcheck-suppress unreadVariable
        Message<MessageId> message = {0, p};
        EXPECT_EQ(OperationResult::success, bus.sendMessage(0, p, CopyMode::copy_once));
        EXPECT_EQ(pool.numberOfFreeElements(), 4U - i);
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 5U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);

    outpost::utils::ConstSharedBufferPointer p;
    mPool.allocate(p);
    ASSERT_TRUE(p.isValid());
    // cppcheck-suppress unreadVariable
    Message<MessageId> message = {0, p};
    EXPECT_EQ(OperationResult::noBufferAvailable, bus.sendMessage(0, p, CopyMode::copy_once));

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 5U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 1U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);

    unittest::swb::TestingSoftwareBus testing(bus);

    for (size_t i = 0; i < 5; i++)
    {
        testing.singleMessage();

        EXPECT_EQ(bus.getNumberOfHandledMessages(), i + 1U);
        EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    }

    testing.singleMessage();

    EXPECT_EQ(bus.getNumberOfHandledMessages(), 5U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);
}

TEST_F(SoftwareBusTest, registerChannels)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    EXPECT_EQ(bus.getNumberOfChannels(), 0U);

    BufferedBusChannelWithMemory<10, MessageId> channel1;
    bus.registerChannel(channel1);

    EXPECT_EQ(bus.getNumberOfChannels(), 1U);

    BufferedBusChannelWithMemory<10, MessageId> channel2;
    bus.registerChannel(channel2);

    EXPECT_EQ(bus.getNumberOfChannels(), 2U);
}

TEST_F(SoftwareBusTest, forwardToChannels)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    EXPECT_EQ(bus.getNumberOfChannels(), 0U);

    BufferedBusChannelWithMemory<10, MessageId> channel1;
    bus.registerChannel(channel1);

    BufferedBusChannelWithMemory<10, MessageId> channel2;
    bus.registerChannel(channel2);

    uint8_t buffer[16];
    for (uint8_t i = 0U; i < 16U; i++)
    {
        buffer[i] = i;
    }
    {
        outpost::Slice<uint8_t> data(buffer);

        MessageId id = 123;

        bus.sendMessage(id, data);
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);

    unittest::swb::TestingSoftwareBus testingBus(bus);
    testingBus.singleMessage();

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 0U);

    Message<MessageId> m1;
    ASSERT_EQ(channel1.receiveMessage(m1, outpost::time::Duration::zero()),
              OperationResult::success);
    EXPECT_EQ(m1.id, 123U);
    EXPECT_ARRAY_EQ(uint8_t, buffer, &m1.buffer[0], 16U);

    Message<MessageId> m2;
    ASSERT_EQ(channel2.receiveMessage(m2, outpost::time::Duration::zero()),
              OperationResult::success);
    EXPECT_EQ(m2.id, 123U);
    EXPECT_ARRAY_EQ(uint8_t, buffer, &m2.buffer[0], 16U);
}

TEST_F(SoftwareBusTest, defaultChannel)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    EXPECT_EQ(bus.getNumberOfChannels(), 0U);

    constexpr MessageId min = 0;
    constexpr MessageId max = 100;
    BufferedBusChannelWithMemory<10, MessageId, RangeFilter<MessageId>> channel1;
    channel1.getFilter().setRange(min, max);
    bus.registerChannel(channel1);

    BufferedBusChannelWithMemory<10, MessageId> channelDefault;
    bus.setDefaultChannel(channelDefault);

    uint8_t buffer[16];
    for (uint8_t i = 0U; i < 16U; i++)
    {
        buffer[i] = i;
    }
    unittest::swb::TestingSoftwareBus testingBus(bus);

    // not matched -> to default
    constexpr MessageId id1 = max + 1;
    {
        outpost::Slice<uint8_t> data(buffer);

        bus.sendMessage(id1, data);
        testingBus.singleMessage();
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 1U);

    Message<MessageId> m1;
    ASSERT_EQ(channel1.receiveMessage(m1, outpost::time::Duration::zero()),
              OperationResult::noMessageAvailable);

    Message<MessageId> m2;
    ASSERT_EQ(channelDefault.receiveMessage(m2, outpost::time::Duration::zero()),
              OperationResult::success);
    EXPECT_EQ(m2.id, id1);
    EXPECT_ARRAY_EQ(uint8_t, buffer, &m2.buffer[0], 16U);

    // matched -> not to default
    constexpr MessageId id2 = max;
    {
        outpost::Slice<uint8_t> data(buffer);

        bus.sendMessage(id2, data);
        testingBus.singleMessage();
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 2U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 2U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 1U);

    ASSERT_EQ(channelDefault.receiveMessage(m1, outpost::time::Duration::zero()),
              OperationResult::noMessageAvailable);

    ASSERT_EQ(channel1.receiveMessage(m2, outpost::time::Duration::zero()),
              OperationResult::success);
    EXPECT_EQ(m2.id, id2);
    EXPECT_ARRAY_EQ(uint8_t, buffer, &m2.buffer[0], 16U);
}

TEST_F(SoftwareBusTest, defaultChannelCannotOverride)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    BufferedBusChannelWithMemory<10, MessageId> channel1;
    EXPECT_EQ(bus.setDefaultChannel(channel1), OperationResult::success);

    BufferedBusChannelWithMemory<10, MessageId> channel2;
    EXPECT_EQ(bus.setDefaultChannel(channel2), OperationResult::maxChannelsReached);
    // also readding the orginal on fails
    EXPECT_EQ(bus.setDefaultChannel(channel1), OperationResult::maxChannelsReached);

    // check against silent override
    uint8_t buffer[16];
    for (uint8_t i = 0U; i < 16U; i++)
    {
        buffer[i] = i;
    }
    unittest::swb::TestingSoftwareBus testingBus(bus);

    // not matched -> to default
    constexpr MessageId id1 = 11;
    {
        outpost::Slice<uint8_t> data(buffer);

        bus.sendMessage(id1, data);
        testingBus.singleMessage();
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 1U);

    Message<MessageId> m1;
    ASSERT_EQ(channel1.receiveMessage(m1, outpost::time::Duration::zero()),
              OperationResult::success);

    EXPECT_EQ(m1.id, id1);
    EXPECT_ARRAY_EQ(uint8_t, buffer, &m1.buffer[0], 16U);

    Message<MessageId> m2;
    ASSERT_EQ(channel2.receiveMessage(m1, outpost::time::Duration::zero()),
              OperationResult::noMessageAvailable);
}

TEST_F(SoftwareBusTest, defaultChannelFiltersAsWell)
{
    SoftwareBus<MessageId> bus(
            mPool, mQueue, 123U, outpost::support::parameter::HeartbeatSource::default0);

    EXPECT_EQ(bus.getNumberOfChannels(), 0U);

    constexpr MessageId min = 0;
    constexpr MessageId max = 100;
    BufferedBusChannelWithMemory<10, MessageId, RangeFilter<MessageId>> channel1;
    channel1.getFilter().setRange(min, max);
    bus.registerChannel(channel1);

    constexpr MessageId maxOfDefault = max * 2;
    BufferedBusChannelWithMemory<10, MessageId, RangeFilter<MessageId>> channelDefault;
    channelDefault.getFilter().setRange(min, maxOfDefault);
    bus.setDefaultChannel(channelDefault);

    uint8_t buffer[16];
    for (uint8_t i = 0U; i < 16U; i++)
    {
        buffer[i] = i;
    }
    unittest::swb::TestingSoftwareBus testingBus(bus);

    // not matched but matched by default -> to default
    constexpr MessageId id1 = maxOfDefault;
    {
        outpost::Slice<uint8_t> data(buffer);

        bus.sendMessage(id1, data);
        testingBus.singleMessage();
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 1U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 1U);

    Message<MessageId> m1;
    ASSERT_EQ(channel1.receiveMessage(m1, outpost::time::Duration::zero()),
              OperationResult::noMessageAvailable);

    Message<MessageId> m2;
    ASSERT_EQ(channelDefault.receiveMessage(m2, outpost::time::Duration::zero()),
              OperationResult::success);
    EXPECT_EQ(m2.id, id1);
    EXPECT_ARRAY_EQ(uint8_t, buffer, &m2.buffer[0], 16U);

    // not matched by any -> non receives
    constexpr MessageId id2 = maxOfDefault + 1;
    {
        outpost::Slice<uint8_t> data(buffer);

        bus.sendMessage(id2, data);
        testingBus.singleMessage();
    }

    EXPECT_EQ(bus.getNumberOfAcceptedMessages(), 2U);
    EXPECT_EQ(bus.getNumberOfFailedCopyOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfFailedSendOperations(), 0U);
    EXPECT_EQ(bus.getNumberOfHandledMessages(), 2U);
    EXPECT_EQ(bus.getNumberOfForwardedMessages(), 0U);
    EXPECT_EQ(bus.getNumberOfDefaultedMessages(), 1U);

    ASSERT_EQ(channelDefault.receiveMessage(m1, outpost::time::Duration::zero()),
              OperationResult::noMessageAvailable);

    ASSERT_EQ(channel1.receiveMessage(m2, outpost::time::Duration::zero()),
              OperationResult::noMessageAvailable);
}
