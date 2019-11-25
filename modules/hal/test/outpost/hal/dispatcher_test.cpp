/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/base/slice.h>
#include <outpost/hal/protocol_dispatcher.h>

#include <unittest/harness.h>

#include <string.h>

#include <array>

class Receiver : public outpost::hal::ReceiverInterface
{
public:
    Receiver() : package(0){};

    outpost::Slice<uint8_t>* package;

    /**
     * receives a data package
     *
     * @param buffer 	buffer to write the data to
     * @param timeout	max timeout to wait for data
     *
     * @return 0	If no package received in the time or failure in underlying receiver
     * 		   >0   Number of bytes received in package, if larger then
     * buffer.getNumberOfElements(), data has been lost
     */
    uint32_t
    receive(outpost::Slice<uint8_t>& buffer, outpost::time::Duration)
    {
        if (package != nullptr)
        {
            memcpy(&buffer[0],
                   &(*package)[0],
                   buffer.getNumberOfElements() < package->getNumberOfElements()
                           ? buffer.getNumberOfElements()
                           : package->getNumberOfElements());
            return package->getNumberOfElements();
        }
        else
        {
            return 0;
        }
    }
};

class ProtocolDispatcherTest : public testing::Test
{
public:
    std::array<uint8_t, 8> buffer;
    Receiver recv;
    outpost::hal::ProtocolDispatcher<uint8_t, 2>* dispatcher;

    void
    handle()
    {
        dispatcher->handlePackage();
    }

    ProtocolDispatcherTest() : dispatcher(0){};

    virtual void
    SetUp() override
    {
        dispatcher = new outpost::hal::ProtocolDispatcher<uint8_t, 2>(
                recv,
                outpost::asSlice(buffer),
                1,
                10,
                1024,
                const_cast<char*>("test"),
                outpost::support::parameter::HeartbeatSource::default1);
        recv.package = 0;
    }

    virtual void
    TearDown() override
    {
        delete dispatcher;
        dispatcher = nullptr;
    }
};

TEST_F(ProtocolDispatcherTest, noQueue)
{
    // no package
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, dropPackage)
{
    std::array<uint8_t, 7> buffer;
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, equalSizePackageIsNotPartialPackage)
{
    std::array<uint8_t, 8> buffer;
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, cutAndDroppedPackage)
{
    std::array<uint8_t, 10> buffer;
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, queuePackage)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());

    EXPECT_TRUE(!queue.isEmpty());
}

TEST_F(ProtocolDispatcherTest, queueOverfull)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 2> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    ProtocolDispatcherTest::handle();
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());

    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&queue));

    EXPECT_TRUE(!queue.isEmpty());
}

TEST_F(ProtocolDispatcherTest, poolOverfull)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 1> pool;
    outpost::utils::SharedBufferQueue<2> queue;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    ProtocolDispatcherTest::handle();
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());

    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&queue));

    EXPECT_TRUE(!queue.isEmpty());
}

TEST_F(ProtocolDispatcherTest, defaultQueue)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 4> pool;
    outpost::utils::SharedBufferQueue<2> queue;
    outpost::utils::SharedBufferQueue<2> defQueue;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID + 1);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_TRUE(dispatcher->setDefaultQueue(&pool, &defQueue));
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_FALSE(defQueue.isEmpty());

    EXPECT_FALSE(dispatcher->setDefaultQueue(&pool, &defQueue));
}

TEST_F(ProtocolDispatcherTest, correctQueue)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 4> pool;
    outpost::utils::SharedBufferQueue<2> queue1;
    outpost::utils::SharedBufferQueue<2> queue2;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue1));
    EXPECT_TRUE(dispatcher->addQueue(ID + 1, &pool, &queue2));
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfUnmatchedPackages());

    EXPECT_FALSE(queue1.isEmpty());
    EXPECT_TRUE(queue2.isEmpty());
}

TEST_F(ProtocolDispatcherTest, twoQueuesOneID)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 4> pool;
    outpost::utils::SharedBufferQueue<2> queue1;
    outpost::utils::SharedBufferQueue<2> queue2;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue1));
    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue2));
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfUnmatchedPackages());

    EXPECT_FALSE(queue1.isEmpty());
    EXPECT_FALSE(queue2.isEmpty());
}

TEST_F(ProtocolDispatcherTest, oneQueuesTwoID)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 4> pool;
    outpost::utils::SharedBufferQueue<2> queue1;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue1));
    EXPECT_TRUE(dispatcher->addQueue(ID + 1, &pool, &queue1));
    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfUnmatchedPackages());

    EXPECT_FALSE(queue1.isEmpty());

    buffer.fill(ID + 1);
    ProtocolDispatcherTest::handle();

    // Packet 1
    EXPECT_FALSE(queue1.isEmpty());
    outpost::utils::SharedBufferPointer data;
    queue1.receive(data);
    EXPECT_EQ(data[0], ID);
    EXPECT_FALSE(queue1.isEmpty());

    // Packet 2
    queue1.receive(data);
    EXPECT_EQ(data[0], ID + 1);
    EXPECT_TRUE(queue1.isEmpty());
}

TEST_F(ProtocolDispatcherTest, cutPackage)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<6, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());

    EXPECT_TRUE(!queue.isEmpty());

    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&queue));
}

TEST_F(ProtocolDispatcherTest, cutPackage2)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    std::array<uint8_t, 10> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages());

    EXPECT_TRUE(!queue.isEmpty());

    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&queue));
}

TEST_F(ProtocolDispatcherTest, cutPackage3)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<6, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    std::array<uint8_t, 10> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages());

    EXPECT_TRUE(!queue.isEmpty());

    EXPECT_EQ(4u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&queue));
}

TEST_F(ProtocolDispatcherTest, tooManyQueues)
{
    outpost::utils::SharedBufferPool<6, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;

    EXPECT_TRUE(dispatcher->addQueue(0, &pool, &queue));
    EXPECT_TRUE(dispatcher->addQueue(0, &pool, &queue));
    EXPECT_FALSE(dispatcher->addQueue(0, &pool, &queue));
}

TEST_F(ProtocolDispatcherTest, dropPartial)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<6, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    std::array<uint8_t, 8> buffer;
    buffer.fill(ID);
    outpost::Slice<uint8_t> p = outpost::asSlice(buffer);
    ProtocolDispatcherTest::recv.package = &p;

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue, true));

    ProtocolDispatcherTest::handle();
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());

    EXPECT_TRUE(queue.isEmpty());

    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&queue));
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages(&queue));
}
