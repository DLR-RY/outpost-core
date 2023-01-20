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

using namespace testing;

static constexpr size_t poolSize = 1500;
static constexpr size_t objectSize = 160;

class SharedBufferTest : public testing::Test
{
public:
    outpost::utils::SharedBufferPool<objectSize, poolSize> mPool;
    outpost::utils::ReferenceQueue<outpost::utils::SharedBufferPointer, 10> mQueue;

    SharedBufferTest()
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

TEST_F(SharedBufferTest, isInitialized)
{
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
    EXPECT_EQ(mPool.numberOfElements(), poolSize);
    EXPECT_EQ(mPool.getElementSize(), objectSize);
}

TEST_F(SharedBufferTest, allocateBuffer)
{
    outpost::utils::SharedBufferPointer p1;
    outpost::utils::SharedBufferPointer p2;
    EXPECT_TRUE(mPool.allocate(p1));
    EXPECT_TRUE(p1.isValid());
    EXPECT_TRUE(p1.isUsed());
    EXPECT_FALSE(p2.isValid());
    EXPECT_FALSE(p2.isUsed());
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    EXPECT_EQ(mPool.numberOfElements(), poolSize);

    EXPECT_TRUE(mPool.allocate(p2));
    EXPECT_TRUE(p2.isValid());
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 2);
    EXPECT_TRUE(p2 != p1);
}

TEST_F(SharedBufferTest, constructorTest)
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

TEST_F(SharedBufferTest, constConstructorTest)
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

TEST_F(SharedBufferTest, deleteParentFirst)
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
SharedBufferTest::passByRef(outpost::utils::SharedBufferPointer& p)
{
    EXPECT_EQ(p->getReferenceCount(), 3U);

    {
        outpost::utils::SharedBufferPointer p_temp = p;
        EXPECT_EQ(p->getReferenceCount(), 4U);
    }

    EXPECT_EQ(p->getReferenceCount(), 3U);
}

void
SharedBufferTest::passByValue(outpost::utils::SharedBufferPointer p)
{
    EXPECT_EQ(p->getReferenceCount(), 4U);

    {
        outpost::utils::SharedBufferPointer p_temp = p;
        EXPECT_EQ(p->getReferenceCount(), 5U);
    }

    EXPECT_EQ(p->getReferenceCount(), 4U);
}

TEST_F(SharedBufferTest, deallocateBuffer)
{
    {
        outpost::utils::SharedBufferPointer p1;
        EXPECT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(SharedBufferTest, allocateFullPool)
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

TEST_F(SharedBufferTest, queueBuffer)
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

TEST_F(SharedBufferTest, queueConstBuffer)
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

TEST_F(SharedBufferTest, allocateChildBuffer)
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

TEST_F(SharedBufferTest, twoElementPool)
{
    outpost::utils::SharedBufferPool<1, 2> pool;
    outpost::utils::SharedBufferPointer p1, p2;
    EXPECT_TRUE(pool.allocate(p1));
    EXPECT_TRUE(pool.allocate(p2));
}

TEST_F(SharedBufferTest, threeElementPool)
{
    outpost::utils::SharedBufferPool<1, 3> pool;
    outpost::utils::SharedBufferPointer p1, p2, p3;
    EXPECT_TRUE(pool.allocate(p1));
    EXPECT_TRUE(pool.allocate(p2));
    EXPECT_TRUE(pool.allocate(p3));
}

TEST_F(SharedBufferTest, allocateChildChildBuffer)
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
            ASSERT_TRUE(child.isValid());

            EXPECT_TRUE(child.getChild(childChild, 2, 0, 5));
            ASSERT_TRUE(childChild.isValid());
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
            ASSERT_TRUE(newChild.isValid());

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

TEST_F(SharedBufferTest, allocateConstChildChildBuffer)
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
            ASSERT_TRUE(mPointer.isValid());

            EXPECT_TRUE(mPointer.getChild(child, 1, 5, 5));
            ASSERT_TRUE(child.isValid());

            EXPECT_TRUE(child.getChild(childChild, 2, 0, 5));
            ASSERT_TRUE(childChild.isValid());
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
            ASSERT_TRUE(newChild.isValid());

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

TEST_F(SharedBufferTest, constPointerKeepsAllocated)
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

TEST_F(SharedBufferTest, aligmentTest)
{
    outpost::utils::SharedBufferPool<objectSize, poolSize, 1> Pool1;
    outpost::utils::SharedBufferPool<objectSize, poolSize, alignof(uint32_t)> Pool2;
    outpost::utils::SharedBufferPool<objectSize, poolSize, 4> Pool3;
    outpost::utils::SharedBufferPool<objectSize, poolSize, 32> Pool4;  // 32 is the largest power of
                                                                       // two dividing
                                                                       // objectSize(160)
    outpost::utils::SharedBufferPool<128, poolSize, 128> Pool5;

    outpost::utils::ConstSharedBufferPointer c1;
    outpost::utils::ConstSharedBufferPointer c2;
    outpost::utils::ConstSharedBufferPointer c3;
    EXPECT_TRUE(Pool2.allocate(c1));
    EXPECT_TRUE(Pool2.allocate(c2));
    EXPECT_TRUE(Pool2.allocate(c3));
    EXPECT_TRUE(reinterpret_cast<size_t>(&c1.asSlice()[0]) % alignof(uint32_t) == 0);
    EXPECT_TRUE(reinterpret_cast<size_t>(&c2.asSlice()[0]) % alignof(uint32_t) == 0);
    EXPECT_TRUE(reinterpret_cast<size_t>(&c3.asSlice()[0]) % alignof(uint32_t) == 0);

    EXPECT_TRUE(Pool3.allocate(c1));
    EXPECT_TRUE(Pool3.allocate(c2));
    EXPECT_TRUE(Pool3.allocate(c3));
    EXPECT_TRUE(reinterpret_cast<size_t>(&c1.asSlice()[0]) % 4 == 0);
    EXPECT_TRUE(reinterpret_cast<size_t>(&c2.asSlice()[0]) % 4 == 0);
    EXPECT_TRUE(reinterpret_cast<size_t>(&c3.asSlice()[0]) % 4 == 0);

    EXPECT_TRUE(Pool4.allocate(c1));
    EXPECT_TRUE(Pool4.allocate(c2));
    EXPECT_TRUE(Pool4.allocate(c3));
    EXPECT_TRUE(reinterpret_cast<size_t>(&c1.asSlice()[0]) % 32 == 0);
    EXPECT_TRUE(reinterpret_cast<size_t>(&c2.asSlice()[0]) % 32 == 0);
    EXPECT_TRUE(reinterpret_cast<size_t>(&c3.asSlice()[0]) % 32 == 0);
}

TEST_F(SharedBufferTest, RawSharedBuffers)
{
    outpost::utils::SharedBuffer b1;

    std::array<uint8_t, 100> memory1;
    outpost::Slice<uint8_t> slice1(memory1);
    std::array<uint8_t, 100> memory2;
    outpost::Slice<uint8_t> slice2(memory2);

    EXPECT_FALSE(b1.isUsed());
    EXPECT_FALSE(b1.isValid());
    EXPECT_EQ(b1.getPointer().getDataPointer(), nullptr);
    EXPECT_EQ(b1.getPointer().getNumberOfElements(), 0u);
    EXPECT_EQ(b1.getReferenceCount(), 0u);

    b1.setPointer(slice1);

    EXPECT_FALSE(b1.isUsed());
    EXPECT_TRUE(b1.isValid());
    EXPECT_EQ(b1.getPointer().getDataPointer(), slice1.getDataPointer());
    EXPECT_EQ(b1.getPointer().getNumberOfElements(), slice1.getNumberOfElements());
    EXPECT_EQ(b1.getReferenceCount(), 0u);

    b1.setPointer(slice2);

    EXPECT_FALSE(b1.isUsed());
    EXPECT_TRUE(b1.isValid());
    EXPECT_EQ(b1.getPointer().getDataPointer(), slice2.getDataPointer());
    EXPECT_EQ(b1.getPointer().getNumberOfElements(), slice2.getNumberOfElements());
    EXPECT_EQ(b1.getReferenceCount(), 0u);

    b1.setPointer(outpost::Slice<uint8_t>::empty());

    EXPECT_FALSE(b1.isUsed());
    EXPECT_FALSE(b1.isValid());
    EXPECT_EQ(b1.getPointer().getDataPointer(), nullptr);
    EXPECT_EQ(b1.getPointer().getNumberOfElements(), 0u);
    EXPECT_EQ(b1.getReferenceCount(), 0u);
}

TEST_F(SharedBufferTest, RawSharedBufferPointer)
{
    outpost::utils::SharedBuffer b1;
    outpost::utils::SharedBufferPointer p1;
    outpost::utils::SharedBufferPointer p2;
    outpost::utils::SharedChildPointer ch1;

    std::array<uint8_t, 100> memory1;
    outpost::Slice<uint8_t> slice1(memory1);
    std::array<uint8_t, 100> memory2;
    outpost::Slice<uint8_t> slice2(memory2);

    EXPECT_FALSE(p1.isUsed());
    EXPECT_FALSE(p1.isValid());
    EXPECT_FALSE(ch1.isUsed());
    EXPECT_FALSE(ch1.isValid());
    EXPECT_EQ(p1.getLength(), 0u);
    EXPECT_EQ(ch1.getLength(), 0u);
    EXPECT_EQ(p1.getType(), 0u);
    EXPECT_EQ(ch1.getType(), 0u);
    EXPECT_EQ(p1.asSlice().getDataPointer(), nullptr);
    EXPECT_EQ(ch1.asSlice().getDataPointer(), nullptr);
    EXPECT_EQ(p1.asSlice().getNumberOfElements(), 0u);
    EXPECT_EQ(ch1.asSlice().getNumberOfElements(), 0u);
    EXPECT_EQ(p1, p2);

    p1.setType(42u);
    EXPECT_EQ(p1.getType(), 42u);

    EXPECT_EQ(p1, p2);
    EXPECT_EQ(p2.getType(), 0u);
    p2 = p1;
    EXPECT_EQ(p2.getType(), 42u);

    EXPECT_FALSE(p1.getChild(ch1, 0, 0, 0));
    EXPECT_FALSE(p1.getChild(ch1, 0, 0, 1));
    EXPECT_FALSE(p1.isChild());
    EXPECT_FALSE(ch1.isChild());

    // start with valid memory
    b1.setPointer(slice1);
    p1 = outpost::utils::SharedBufferPointer(&b1);

    EXPECT_TRUE(b1.isUsed());
    EXPECT_TRUE(p1.isUsed());
    EXPECT_TRUE(b1.isValid());
    EXPECT_TRUE(p1.isValid());
    EXPECT_EQ(p1.getLength(), slice1.getNumberOfElements());
    EXPECT_EQ(p1.getType(), 0u);
    EXPECT_EQ(p1.asSlice().getDataPointer(), slice1.getDataPointer());
    EXPECT_EQ(p1.asSlice().getNumberOfElements(), slice1.getNumberOfElements());
    EXPECT_NE(p1, p2);

    // try to change the memory
    b1.setPointer(slice2);
    EXPECT_EQ(b1.getPointer().getDataPointer(), slice1.getDataPointer());
    EXPECT_NE(b1.getPointer().getDataPointer(), slice2.getDataPointer());
}

TEST_F(SharedBufferTest, RawSharedChildPointer)
{
    outpost::utils::SharedBuffer b1;
    outpost::utils::SharedBufferPointer p1;
    outpost::utils::SharedBufferPointer p2;
    outpost::utils::SharedChildPointer ch1;
    outpost::utils::SharedChildPointer ch2;

    std::array<uint8_t, 100> memory1;
    outpost::Slice<uint8_t> slice1(memory1);
    for (size_t i = 0; i < slice1.getNumberOfElements(); i++)
    {
        slice1[i] = i;
    }

    // start with valid memory
    b1.setPointer(slice1);
    p1 = outpost::utils::SharedBufferPointer(&b1);

    // get children
    p1.setType(42u);
    EXPECT_TRUE(p1.getChild(ch1, 0, 0, 10));
    EXPECT_TRUE(p1.getChild(ch2, 0, 0, 0));

    ASSERT_TRUE(ch1.isUsed());
    EXPECT_TRUE(ch2.isUsed());
    ASSERT_TRUE(ch1.isValid());
    EXPECT_TRUE(ch2.isValid());
    EXPECT_EQ(ch1.getLength(), 10u);
    EXPECT_EQ(ch1.getType(), 0u);
    EXPECT_EQ(ch1.asSlice().getDataPointer(), slice1.getDataPointer());
    EXPECT_EQ(ch1.asSlice().getNumberOfElements(), 10u);
    EXPECT_EQ(ch1.asSlice()[0], 0u);
    EXPECT_EQ(ch1.asSlice()[9], 9u);

    EXPECT_EQ(ch1, ch2);

    EXPECT_EQ(ch2.getLength(), 0u);
    EXPECT_EQ(ch2.getType(), 0u);
    EXPECT_EQ(ch2.asSlice().getDataPointer(), slice1.getDataPointer());
    EXPECT_EQ(ch2.asSlice().getNumberOfElements(), 0u);

    // test child child
    outpost::utils::SharedChildPointer ch1ch1;
    outpost::utils::SharedChildPointer ch1ch2;
    outpost::utils::SharedChildPointer ch2ch1;
    outpost::utils::SharedChildPointer ch2ch2;

    EXPECT_TRUE(ch1.getChild(ch1ch1, 0, 2, 1));  // shall be OK to get a subchild
    EXPECT_TRUE(ch1.getChild(ch1ch1, 0, 3, 1));  // shall be OK to update a child
    EXPECT_TRUE(ch1.getChild(ch1ch2, 0, 3, 0));  // shall be OK to get a zero length sub child
    EXPECT_FALSE(
            ch2.getChild(ch2ch1, 0, 2, 1));  // shall be BAD to get a child from a zero length child
    EXPECT_TRUE(ch2.getChild(ch2ch1, 0, 0, 0));  // shall be OK to get a zero length child from a
                                                 // zero length child
    EXPECT_TRUE(ch2.getChild(ch2ch2, 0, 0, 0));

    ASSERT_TRUE(ch1ch1.isUsed());
    ASSERT_TRUE(ch1ch2.isUsed());
    ASSERT_TRUE(ch2ch1.isUsed());
    ASSERT_TRUE(ch2ch2.isUsed());

    EXPECT_EQ(ch1ch1, ch1ch2);
    EXPECT_EQ(ch1ch2, ch2ch1);
    EXPECT_EQ(ch2ch1, ch2ch2);
}

TEST_F(SharedBufferTest, PreventOutOfChilChilds)
{
    outpost::utils::SharedBuffer b1;
    outpost::utils::SharedBufferPointer p1;
    outpost::utils::SharedChildPointer ch1;
    outpost::utils::SharedChildPointer ch1ch1;

    std::array<uint8_t, 100> memory1;
    outpost::Slice<uint8_t> slice1(memory1);
    for (size_t i = 0; i < slice1.getNumberOfElements(); i++)
    {
        slice1[i] = i;
    }

    // start with valid memory
    b1.setPointer(slice1);
    p1 = outpost::utils::SharedBufferPointer(&b1);

    // get children
    p1.setType(42u);
    EXPECT_TRUE(p1.getChild(ch1, 0, 0, 10));

    EXPECT_TRUE(ch1.isUsed());
    ASSERT_TRUE(ch1.isValid());
    EXPECT_EQ(ch1.getLength(), 10u);
    EXPECT_EQ(ch1.getType(), 0u);
    EXPECT_EQ(ch1.asSlice().getDataPointer(), slice1.getDataPointer());
    EXPECT_EQ(ch1.asSlice().getNumberOfElements(), 10u);
    EXPECT_EQ(ch1.asSlice()[0], 0u);
    EXPECT_EQ(ch1.asSlice()[9], 9u);

    // shall be BAD to get a child with an offset and length out of the range of the parent
    EXPECT_FALSE(ch1.getChild(ch1ch1, 0, 20, 50));
    EXPECT_FALSE(ch1.getChild(ch1ch1, 0, 20, 0));
    EXPECT_FALSE(ch1.getChild(ch1ch1, 0, 0, 50));

    // the parent should be GE its child (Offset + length of child should be within offset + length
    // of parent)
    EXPECT_GE(ch1.getLength(), ch1ch1.getLength());

    EXPECT_EQ(ch1.asSlice()[0], 0u);
    EXPECT_EQ(ch1.getLength(), 10u);
    ASSERT_FALSE(ch1ch1.isValid());
    EXPECT_FALSE(ch1ch1.isUsed());
}
