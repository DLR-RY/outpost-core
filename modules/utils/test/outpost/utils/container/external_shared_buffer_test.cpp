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
 * - 2017-2018, Jan-Gerd Mess (DLR RY-AVS)
 * - 2018, Fabian Greif (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/rtos/thread.h>
#include <outpost/utils/container/reference_queue.h>
#include <outpost/utils/container/shared_buffer.h>
#include <outpost/utils/container/shared_object_pool.h>
#include <outpost/utils/storage/serialize.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <array>

using namespace testing;

static constexpr size_t poolSize = 1500;
static constexpr size_t objectSize = 160;
static std::array<uint8_t, poolSize * objectSize> DataBuffer;

class ExternalSharedBufferTest : public testing::Test
{
public:
    outpost::utils::ExternalSharedBufferPool<objectSize, poolSize> mPool{DataBuffer.data()};
    outpost::utils::ReferenceQueue<outpost::utils::SharedBufferPointer, 10> mQueue;

    ExternalSharedBufferTest()
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

    void
    passByRef(outpost::utils::SharedBufferPointer& p);

    void
    passByValue(outpost::utils::SharedBufferPointer p);
};

TEST_F(ExternalSharedBufferTest, isInitialized)
{
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
    EXPECT_EQ(mPool.numberOfElements(), poolSize);
    EXPECT_EQ(mPool.getElementSize(), objectSize);
}

TEST_F(ExternalSharedBufferTest, allocateBuffer)
{
    outpost::utils::SharedBufferPointer p1;
    outpost::utils::SharedBufferPointer p2;
    EXPECT_TRUE(mPool.allocate(p1));
    EXPECT_TRUE(p1.isValid());
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    EXPECT_EQ(mPool.numberOfElements(), poolSize);

    EXPECT_TRUE(mPool.allocate(p2));
    EXPECT_TRUE(p2.isValid());
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
    EXPECT_TRUE(p2 != p1);
}

TEST_F(ExternalSharedBufferTest, constructorTest)
{
    outpost::utils::SharedBufferPointer p1;
    EXPECT_EQ(p1.asSlice().getNumberOfElements(), 0U);

    outpost::Slice<uint8_t> slice = static_cast<outpost::Slice<uint8_t>>(p1);
    EXPECT_EQ(slice.getNumberOfElements(), 0U);

    outpost::Slice<const uint8_t> constSlice = static_cast<outpost::Slice<const uint8_t>>(p1);
    EXPECT_EQ(constSlice.getNumberOfElements(), 0U);

    uint8_t* ptr = static_cast<uint8_t*>(p1);
    EXPECT_EQ(ptr, nullptr);

    EXPECT_TRUE(mPool.allocate(p1));
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    outpost::utils::SharedBufferPointer p2;
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    p2 = p1;
    EXPECT_EQ(p1->getReferenceCount(), 2U);

    outpost::utils::SharedBufferPointer p3(p1);
    EXPECT_EQ(p1->getReferenceCount(), 3U);

    {
        outpost::utils::SharedBufferPointer p4 = outpost::utils::SharedBufferPointer(p3);
        EXPECT_EQ(p1->getReferenceCount(), 4U);
        outpost::utils::SharedChildPointer ch2;
        {
            outpost::utils::SharedChildPointer ch1;
            EXPECT_EQ(p1->getReferenceCount(), 4U);
            p1.getChild(ch1, 0, 0, 1);
            EXPECT_EQ(p1->getReferenceCount(), 6U);

            {
                EXPECT_EQ(p1->getReferenceCount(), 6U);
                ch1.getChild(ch2, 0, 0, 1);
                EXPECT_EQ(p1->getReferenceCount(), 8U);
            }
        }
        EXPECT_EQ(p1->getReferenceCount(), 6U);

        outpost::utils::SharedChildPointer ch3 = outpost::utils::SharedChildPointer(ch2);
    }

    EXPECT_EQ(p1->getReferenceCount(), 3U);
}

TEST_F(ExternalSharedBufferTest, constConstructorTest)
{
    outpost::utils::ConstSharedBufferPointer p1;

    EXPECT_EQ(p1.asSlice().getNumberOfElements(), 0U);

    outpost::Slice<const uint8_t> slice = static_cast<outpost::Slice<const uint8_t>>(p1);
    EXPECT_EQ(slice.getNumberOfElements(), 0U);

    const uint8_t* ptr = static_cast<const uint8_t*>(p1);
    EXPECT_EQ(ptr, nullptr);

    EXPECT_TRUE(mPool.allocate(p1));
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    outpost::utils::ConstSharedBufferPointer p2;
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    p2 = p1;
    EXPECT_EQ(p1->getReferenceCount(), 2U);

    outpost::utils::ConstSharedBufferPointer p3(p1);
    EXPECT_EQ(p1->getReferenceCount(), 3U);

    {
        outpost::utils::ConstSharedBufferPointer p4 = outpost::utils::ConstSharedBufferPointer(p3);
        EXPECT_EQ(p1->getReferenceCount(), 4U);
        outpost::utils::ConstSharedChildPointer ch2;
        {
            outpost::utils::ConstSharedChildPointer ch1;
            EXPECT_EQ(p1->getReferenceCount(), 4U);
            p1.getChild(ch1, 0, 0, 1);
            EXPECT_EQ(p1->getReferenceCount(), 6U);

            {
                EXPECT_EQ(p1->getReferenceCount(), 6U);
                ch1.getChild(ch2, 0, 0, 1);
                EXPECT_EQ(p1->getReferenceCount(), 8U);
            }
        }
        EXPECT_EQ(p1->getReferenceCount(), 6U);

        outpost::utils::ConstSharedChildPointer ch3 = outpost::utils::ConstSharedChildPointer(ch2);
    }

    EXPECT_EQ(p1->getReferenceCount(), 3U);
}

TEST_F(ExternalSharedBufferTest, deleteParentFirst)
{
    {
        outpost::utils::SharedChildPointer ch1;
        {
            outpost::utils::SharedBufferPointer p1;
            mPool.allocate(p1);
            EXPECT_EQ(p1->getReferenceCount(), 1U);
            p1.getChild(ch1, 0, 0, 1);
            EXPECT_EQ(p1->getReferenceCount(), 3U);
            EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
            passByRef(p1);
            EXPECT_EQ(p1->getReferenceCount(), 3U);
            EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
            passByValue(p1);
            EXPECT_EQ(p1->getReferenceCount(), 3U);
            EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        }
        EXPECT_EQ(ch1->getReferenceCount(), 2U);
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    }
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

void
ExternalSharedBufferTest::passByRef(outpost::utils::SharedBufferPointer& p)
{
    EXPECT_EQ(p->getReferenceCount(), 3U);

    {
        outpost::utils::SharedBufferPointer p_temp = p;
        EXPECT_EQ(p->getReferenceCount(), 4U);
    }

    EXPECT_EQ(p->getReferenceCount(), 3U);
}

void
ExternalSharedBufferTest::passByValue(outpost::utils::SharedBufferPointer p)
{
    EXPECT_EQ(p->getReferenceCount(), 4U);

    {
        outpost::utils::SharedBufferPointer p_temp = p;
        EXPECT_EQ(p->getReferenceCount(), 5U);
    }

    EXPECT_EQ(p->getReferenceCount(), 4U);
}

TEST_F(ExternalSharedBufferTest, deallocateBuffer)
{
    {
        outpost::utils::SharedBufferPointer p1;
        EXPECT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(ExternalSharedBufferTest, allocateFullPool)
{
    outpost::utils::SharedBufferPointer p[poolSize];
    for (size_t i = 0; i < poolSize; i++)
    {
        EXPECT_TRUE(mPool.allocate(p[i]));
        EXPECT_TRUE(p[i].isValid());
    }

    outpost::utils::SharedBufferPointer p_false;
    EXPECT_FALSE(mPool.allocate(p_false));
    EXPECT_FALSE(p_false.isValid());
}

TEST_F(ExternalSharedBufferTest, queueBuffer)
{
    outpost::utils::ReferenceQueue<outpost::utils::SharedBufferPointer, 2> q;
    EXPECT_TRUE(q.isEmpty());
    EXPECT_FALSE(q.isFull());

    {
        outpost::utils::SharedBufferPointer p1;
        ASSERT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            p1[i] = 0x01;
        }
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        EXPECT_TRUE(q.send(p1));
        EXPECT_FALSE(q.isEmpty());
        EXPECT_FALSE(q.isFull());
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
        EXPECT_TRUE(q.send(p1));
        EXPECT_FALSE(q.isEmpty());
        EXPECT_TRUE(q.isFull());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);

    {
        outpost::utils::SharedBufferPointer p2;
        EXPECT_TRUE(q.receive(p2, outpost::time::Duration::zero()));
        EXPECT_TRUE(p2.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            EXPECT_EQ(p2[i], 0x01);
        }
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
        EXPECT_FALSE(q.isEmpty());
        EXPECT_FALSE(q.isFull());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

    {
        outpost::utils::SharedBufferPointer p2;

        EXPECT_TRUE(q.receive(p2, outpost::time::Duration::zero()));

        ASSERT_TRUE(p2.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            EXPECT_EQ(p2[i], 0x02);
        }
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        EXPECT_TRUE(q.isEmpty());
        EXPECT_FALSE(q.isFull());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(ExternalSharedBufferTest, queueConstBuffer)
{
    outpost::utils::ReferenceQueue<outpost::utils::ConstSharedBufferPointer, 2> q;
    EXPECT_TRUE(q.isEmpty());
    EXPECT_FALSE(q.isFull());

    {
        outpost::utils::SharedBufferPointer p1;
        ASSERT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            p1[i] = 0x03;
        }
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        outpost::utils::ConstSharedBufferPointer constP1 = p1;
        EXPECT_TRUE(q.send(constP1));
        EXPECT_FALSE(q.isEmpty());
        EXPECT_FALSE(q.isFull());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    {
        outpost::utils::SharedBufferPointer p1;
        ASSERT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());

        for (size_t i = 0; i < objectSize; i++)
        {
            p1[i] = 0x04;
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
        outpost::utils::ConstSharedBufferPointer constP1 = p1;
        EXPECT_TRUE(q.send(constP1));
        EXPECT_FALSE(q.isEmpty());
        EXPECT_TRUE(q.isFull());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);

    {
        outpost::utils::ConstSharedBufferPointer p2;
        EXPECT_TRUE(q.receive(p2, outpost::time::Duration::zero()));
        EXPECT_TRUE(p2.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            EXPECT_EQ(p2[i], 0x03);
        }
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
        EXPECT_FALSE(q.isEmpty());
        EXPECT_FALSE(q.isFull());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

    {
        outpost::utils::ConstSharedBufferPointer p2;

        EXPECT_TRUE(q.receive(p2, outpost::time::Duration::zero()));

        ASSERT_TRUE(p2.isValid());
        for (size_t i = 0; i < objectSize; i++)
        {
            EXPECT_EQ(p2[i], 0x04);
        }
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        EXPECT_TRUE(q.isEmpty());
        EXPECT_FALSE(q.isFull());
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(ExternalSharedBufferTest, allocateChildBuffer)
{
    {
        outpost::utils::SharedChildPointer child;

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);

        {
            outpost::utils::SharedBufferPointer mPointer;
            EXPECT_TRUE(mPool.allocate(mPointer));
            EXPECT_TRUE(mPointer.isValid());
            for (size_t i = 0; i < objectSize; i++)
            {
                mPointer[i] = i;
            }

            EXPECT_TRUE(mPointer.getChild(child, 1, 5, 5));
            EXPECT_TRUE(child.isValid());

            EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

        {
            outpost::utils::SharedBufferPointer mPointer = child.getParent();

            outpost::utils::SharedChildPointer newChild;

            EXPECT_TRUE(mPointer.getChild(newChild, 3, 2, 3));
            EXPECT_TRUE(newChild.isValid());

            outpost::Slice<const uint8_t> mArray(
                    static_cast<outpost::Slice<const uint8_t>>(newChild));
            EXPECT_EQ(mArray.getNumberOfElements(), newChild.getLength());
            for (size_t i = 0; i < newChild.getLength(); i++)
            {
                EXPECT_EQ(mArray[i], i + 2);
            }
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(ExternalSharedBufferTest, twoElementPool)
{
    outpost::utils::SharedBufferPool<1, 2> pool;
    outpost::utils::SharedBufferPointer p1, p2;
    EXPECT_TRUE(pool.allocate(p1));
    EXPECT_TRUE(pool.allocate(p2));
}

TEST_F(ExternalSharedBufferTest, threeElementPool)
{
    outpost::utils::SharedBufferPool<1, 3> pool;
    outpost::utils::SharedBufferPointer p1, p2, p3;
    EXPECT_TRUE(pool.allocate(p1));
    EXPECT_TRUE(pool.allocate(p2));
    EXPECT_TRUE(pool.allocate(p3));
}

TEST_F(ExternalSharedBufferTest, allocateChildChildBuffer)
{
    {
        outpost::utils::SharedChildPointer child;
        outpost::utils::SharedChildPointer childChild;

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);

        {
            outpost::utils::SharedBufferPointer mPointer;
            EXPECT_TRUE(mPool.allocate(mPointer));
            EXPECT_TRUE(mPointer.isValid());
            for (size_t i = 0; i < objectSize; i++)
            {
                mPointer[i] = i + 1;  // other value then previous tests
            }

            EXPECT_TRUE(mPointer.getChild(child, 1, 5, 5));
            EXPECT_TRUE(child.isValid());

            EXPECT_TRUE(child.getChild(childChild, 2, 0, 5));
            EXPECT_TRUE(childChild.isValid());
            EXPECT_TRUE(childChild.getOrigin() == mPointer);
            EXPECT_TRUE(childChild.getParent() == child);
            EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

            outpost::utils::SharedChildPointer childChildChild;
            EXPECT_TRUE(childChild.getChild(childChildChild, 3, 0, 5));
            EXPECT_TRUE(childChildChild.getOrigin() == mPointer);
            EXPECT_TRUE(childChildChild == mPointer);
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

        {
            outpost::utils::SharedBufferPointer mPointer = child.getParent();

            outpost::utils::SharedChildPointer newChild;

            EXPECT_TRUE(mPointer.getChild(newChild, 3, 2, 3));
            EXPECT_TRUE(newChild.isValid());

            EXPECT_TRUE(newChild.isChild());

            EXPECT_FALSE(mPointer.isChild());

            outpost::Slice<const uint8_t> mArray(
                    static_cast<outpost::Slice<const uint8_t>>(newChild));
            EXPECT_EQ(mArray.getNumberOfElements(), newChild.getLength());
            for (size_t i = 0; i < newChild.getLength(); i++)
            {
                EXPECT_EQ(mArray[i], i + 1 + 2);
            }
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(ExternalSharedBufferTest, allocateConstChildChildBuffer)
{
    {
        outpost::utils::ConstSharedChildPointer child;
        outpost::utils::ConstSharedChildPointer childChild;

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);

        {
            outpost::utils::SharedBufferPointer mPointerWriteable;
            EXPECT_TRUE(mPool.allocate(mPointerWriteable));
            EXPECT_TRUE(mPointerWriteable.isValid());
            for (size_t i = 0; i < objectSize; i++)
            {
                mPointerWriteable[i] = i + 2;  // other value then the previous tests
            }

            outpost::utils::ConstSharedBufferPointer mPointer = mPointerWriteable;
            mPointerWriteable = outpost::utils::SharedBufferPointer();
            EXPECT_TRUE(mPointer.isValid());

            EXPECT_TRUE(mPointer.getChild(child, 1, 5, 5));
            EXPECT_TRUE(child.isValid());

            EXPECT_TRUE(child.getChild(childChild, 2, 0, 5));
            EXPECT_TRUE(childChild.isValid());
            EXPECT_TRUE(childChild.getOrigin() == mPointer);
            EXPECT_TRUE(childChild.getParent() == child);
            EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

            outpost::utils::ConstSharedChildPointer childChildChild;
            EXPECT_TRUE(childChild.getChild(childChildChild, 3, 0, 5));
            EXPECT_TRUE(childChildChild.getOrigin() == mPointer);
            EXPECT_TRUE(childChildChild == mPointer);
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

        {
            outpost::utils::ConstSharedBufferPointer mPointer = child.getParent();

            outpost::utils::ConstSharedChildPointer newChild;

            EXPECT_TRUE(mPointer.getChild(newChild, 3, 2, 3));
            EXPECT_TRUE(newChild.isValid());

            EXPECT_TRUE(newChild.isChild());

            EXPECT_FALSE(mPointer.isChild());

            outpost::Slice<const uint8_t> mArray(
                    static_cast<outpost::Slice<const uint8_t>>(newChild));
            EXPECT_EQ(mArray.getNumberOfElements(), newChild.getLength());
            for (size_t i = 0; i < newChild.getLength(); i++)
            {
                EXPECT_EQ(mArray[i], i + 2 + 2);
            }
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(ExternalSharedBufferTest, constPointerKeepsAllocated)
{
    outpost::utils::ConstSharedBufferPointer c1;
    outpost::utils::SharedBufferPointer p1;

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);

    EXPECT_TRUE(mPool.allocate(c1));
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    EXPECT_TRUE(mPool.allocate(p1));
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
    c1 = p1;
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    p1 = outpost::utils::SharedBufferPointer();
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    c1 = outpost::utils::ConstSharedBufferPointer();  // cppcheck-suppress redundantAssignment
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}
