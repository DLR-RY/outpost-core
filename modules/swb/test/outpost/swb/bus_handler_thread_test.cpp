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
#include <outpost/swb/bus_subscription.h>
#include <outpost/swb/software_bus.h>
#include <outpost/utils/container/reference_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

#include <unittest/harness.h>
#include <unittest/swb/testing_software_bus.h>
#include <unittest/time/testing_clock.h>

#include <cstdlib>
#include <future>
#include <utility>

static unittest::time::TestingClock Clock;

class BusHandlerThreadTest : public ::testing::Test
{
public:
    BusHandlerThreadTest()
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
};

using namespace outpost::swb;

// ----------------------------------------------------------------------------
TEST_F(BusHandlerThreadTest, handlerTest)
{
    outpost::utils::SharedBufferPool<1024, 10> pool;
    outpost::utils::ReferenceQueue<outpost::swb::Message<uint16_t>, 10> queue;
    outpost::swb::SoftwareBusFiltered<uint16_t, RangeFilter<uint16_t>> bus(
            pool, queue, 1U, outpost::support::parameter::HeartbeatSource::default0);

    unittest::swb::TestingSoftwareBus testingBus(bus);
    bus.getFilter().setRange(1, 100);

    outpost::swb::BufferedBusChannelWithMemory<10, uint16_t, SubscriptionFilter<uint16_t>> channel;
    outpost::swb::BusSubscription<uint16_t> subscription(1U);
    channel.getFilter().registerSubscription(subscription);
    EXPECT_TRUE(bus.registerChannel(channel) == outpost::swb::OperationResult::success);

    testingBus.singleMessage();
    outpost::swb::Message<uint16_t> m;
    EXPECT_FALSE(channel.receiveMessage(m) == outpost::swb::OperationResult::success);

    outpost::utils::SharedBufferPointer p;
    pool.allocate(p);
    for (size_t i = 0; i < 16U; i++)
    {
        p[i] = i;
    }

    outpost::swb::Message<uint16_t> sendMessage = {2U, p};
    EXPECT_EQ(outpost::swb::OperationResult::success, bus.sendMessage(sendMessage));
    testingBus.singleMessage();

    EXPECT_FALSE(channel.receiveMessage(m) == outpost::swb::OperationResult::success);

    sendMessage = {1U, p};
    EXPECT_EQ(outpost::swb::OperationResult::success, bus.sendMessage(sendMessage));
    testingBus.singleMessage();

    EXPECT_TRUE(channel.receiveMessage(m) == outpost::swb::OperationResult::success);
    EXPECT_EQ(m.id, 1U);
    for (size_t i = 0; i < 16U; i++)
    {
        EXPECT_EQ(m.buffer[i], i);
    }

    sendMessage = {0U, p};
    EXPECT_EQ(outpost::swb::OperationResult::invalidMessage, bus.sendMessage(sendMessage));
    testingBus.singleMessage();

    EXPECT_TRUE(channel.receiveMessage(m) == outpost::swb::OperationResult::noMessageAvailable);

    sendMessage = {101U, p};
    EXPECT_EQ(outpost::swb::OperationResult::invalidMessage, bus.sendMessage(sendMessage));
    testingBus.singleMessage();

    EXPECT_TRUE(channel.receiveMessage(m) == outpost::swb::OperationResult::noMessageAvailable);
}
