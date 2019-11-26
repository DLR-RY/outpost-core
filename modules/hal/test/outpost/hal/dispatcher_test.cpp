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

class ProtocolDispatcherTest : public testing::Test
{
public:
    static constexpr uint32_t offset = 1;
    outpost::hal::ProtocolDispatcher<uint8_t, 2>* dispatcher;
    std::array<uint8_t, 8> buffer;

    ProtocolDispatcherTest() : dispatcher(0){};

    virtual void
    SetUp() override
    {
        dispatcher = new outpost::hal::ProtocolDispatcher<uint8_t, 2>(offset);
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
    dispatcher->handlePackage(outpost::asSlice(buffer), 0);
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, dropPackage)
{
    dispatcher->handlePackage(outpost::asSlice(buffer), 7);
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, equalSizePackageIsNotPartialPackage)
{
    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, cutAndDroppedPackage)
{
    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages());
}

TEST_F(ProtocolDispatcherTest, queuePackage)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
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
    buffer.fill(ID + 1);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_TRUE(dispatcher->setDefaultQueue(&pool, &defQueue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_FALSE(defQueue.isEmpty());

    EXPECT_FALSE(dispatcher->setDefaultQueue(&pool, &defQueue));
}

TEST_F(ProtocolDispatcherTest, resetCounters)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 4> pool;
    outpost::utils::SharedBufferQueue<2> queue;
    outpost::utils::SharedBufferQueue<2> defQueue;
    buffer.fill(ID + 1);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_TRUE(dispatcher->setDefaultQueue(&pool, &defQueue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());

    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes(&defQueue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&defQueue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
    EXPECT_EQ(2u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(4u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(2u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes(&defQueue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&defQueue));
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&defQueue));

    buffer.fill(ID);
    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
    EXPECT_EQ(2u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(6u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(3u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes(&defQueue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&defQueue));
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&defQueue));
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
    EXPECT_EQ(3u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(10u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(5u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(1u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_EQ(2u, dispatcher->getNumberOfOverflowedBytes(&defQueue));
    EXPECT_EQ(1u, dispatcher->getNumberOfPartialPackages(&defQueue));
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&defQueue));
    EXPECT_EQ(4u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(2u, dispatcher->getNumberOfPartialPackages(&queue));
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&queue));

    dispatcher->resetErrorCounters();
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfUnmatchedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes(&defQueue));
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages(&defQueue));
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages(&defQueue));
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages(&queue));
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages(&queue));
}

TEST_F(ProtocolDispatcherTest, correctQueue)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 4> pool;
    outpost::utils::SharedBufferQueue<2> queue1;
    outpost::utils::SharedBufferQueue<2> queue2;
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue1));
    EXPECT_TRUE(dispatcher->addQueue(ID + 1, &pool, &queue2));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue1));
    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue2));
    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue1));
    EXPECT_TRUE(dispatcher->addQueue(ID + 1, &pool, &queue1));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    EXPECT_EQ(0u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfUnmatchedPackages());

    EXPECT_FALSE(queue1.isEmpty());

    buffer.fill(ID + 1);
    dispatcher->handlePackage(outpost::asSlice(buffer), 8);

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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 10);
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
    buffer.fill(ID);

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue, true));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);
    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages());
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes());
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages());

    EXPECT_TRUE(queue.isEmpty());

    EXPECT_EQ(1u, dispatcher->getNumberOfDroppedPackages(&queue));
    EXPECT_EQ(0u, dispatcher->getNumberOfOverflowedBytes(&queue));
    EXPECT_EQ(0u, dispatcher->getNumberOfPartialPackages(&queue));
}

TEST_F(ProtocolDispatcherTest, testCopy)
{
    const uint8_t ID = 1;
    outpost::utils::SharedBufferPool<8, 1> pool;
    outpost::utils::SharedBufferQueue<1> queue;
    for (unsigned int i = 0; i < 8; i++)
    {
        buffer[i] = i;
    }

    EXPECT_TRUE(dispatcher->addQueue(ID, &pool, &queue));

    dispatcher->handlePackage(outpost::asSlice(buffer), 8);

    EXPECT_FALSE(queue.isEmpty());

    outpost::utils::SharedBufferPointer data;
    queue.receive(data);

    EXPECT_ARRAY_EQ(uint8_t, &buffer[0], &data[0], 8);

    for (unsigned int i = 0; i < 8; i++)
    {
        buffer[i] = i + 8;
    }
    buffer[offset] = ID;
    data = outpost::utils::SharedBufferPointer();

    dispatcher->handlePackage(outpost::asSlice(buffer), 6);
    EXPECT_FALSE(queue.isEmpty());
    queue.receive(data);
    EXPECT_ARRAY_EQ(uint8_t, &buffer[0], &data[0], 6);
    EXPECT_EQ(data.getLength(), 6u);
}
