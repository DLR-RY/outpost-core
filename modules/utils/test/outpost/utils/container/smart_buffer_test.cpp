/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <outpost/rtos/thread.h>
#include <outpost/utils/container/smart_buffer.h>
#include <outpost/utils/container/smart_object_pool.h>
#include <outpost/utils/storage/serialize.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unittest/utils/container/smart_buffer_queue_stub.h>

using namespace testing;

static constexpr size_t poolSize = 1500;
static constexpr size_t objectSize = 160;

class SmartBufferTest : public testing::Test
{
public:
    outpost::utils::SmartBufferPool<objectSize, poolSize> mPool;
    unittest::utils::SmartBufferQueue<10> mQueue;

    SmartBufferTest()
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
    passByRef(outpost::utils::SmartBufferPointer& p);

    void
    passByValue(outpost::utils::SmartBufferPointer p);
};

TEST_F(SmartBufferTest, isInitialized)
{
    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
    EXPECT_EQ(mPool.numberOfElements(), poolSize);
}

TEST_F(SmartBufferTest, allocateBuffer)
{
    outpost::utils::SmartBufferPointer p1;
    outpost::utils::SmartBufferPointer p2;
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

TEST_F(SmartBufferTest, constructorTest)
{
    outpost::utils::SmartBufferPointer p1;
    EXPECT_TRUE(mPool.allocate(p1));
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    outpost::utils::SmartBufferPointer p2;
    EXPECT_EQ(p1->getReferenceCount(), 1U);
    p2 = p1;
    EXPECT_EQ(p1->getReferenceCount(), 2U);

    outpost::utils::SmartBufferPointer p3(p1);
    EXPECT_EQ(p1->getReferenceCount(), 3U);

    {
        outpost::utils::SmartBufferPointer p4 = outpost::utils::SmartBufferPointer(p3);
        EXPECT_EQ(p1->getReferenceCount(), 4U);
        outpost::utils::ChildSmartPointer ch2;
        {
            outpost::utils::ChildSmartPointer ch1;
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

        outpost::utils::ChildSmartPointer ch3 = outpost::utils::ChildSmartPointer(ch2);
    }

    EXPECT_EQ(p1->getReferenceCount(), 3U);
}

TEST_F(SmartBufferTest, deleteParentFirst)
{
    {
        outpost::utils::ChildSmartPointer ch1;
        {
            outpost::utils::SmartBufferPointer p1;
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
SmartBufferTest::passByRef(outpost::utils::SmartBufferPointer& p)
{
    EXPECT_EQ(p->getReferenceCount(), 3U);

    {
        outpost::utils::SmartBufferPointer p_temp = p;
        EXPECT_EQ(p->getReferenceCount(), 4U);
    }

    EXPECT_EQ(p->getReferenceCount(), 3U);
}

void
SmartBufferTest::passByValue(outpost::utils::SmartBufferPointer p)
{
    EXPECT_EQ(p->getReferenceCount(), 4U);

    {
        outpost::utils::SmartBufferPointer p_temp = p;
        EXPECT_EQ(p->getReferenceCount(), 5U);
    }

    EXPECT_EQ(p->getReferenceCount(), 4U);
}

TEST_F(SmartBufferTest, deallocateBuffer)
{
    {
        outpost::utils::SmartBufferPointer p1;
        EXPECT_TRUE(mPool.allocate(p1));
        EXPECT_TRUE(p1.isValid());
        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);
    }

    EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);
}

TEST_F(SmartBufferTest, allocateFullPool)
{
    outpost::utils::SmartBufferPointer p[poolSize];
    for (size_t i = 0; i < poolSize; i++)
    {
        EXPECT_TRUE(mPool.allocate(p[i]));
        EXPECT_TRUE(p[i].isValid());
    }

    outpost::utils::SmartBufferPointer p_false;
    EXPECT_FALSE(mPool.allocate(p_false));
    EXPECT_FALSE(p_false.isValid());
}

TEST_F(SmartBufferTest, queueBuffer)
{
    unittest::utils::SmartBufferQueue<2> q;
    EXPECT_TRUE(q.isEmpty());
    EXPECT_FALSE(q.isFull());

    {
        outpost::utils::SmartBufferPointer p1;
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
        outpost::utils::SmartBufferPointer p1;
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
        outpost::utils::SmartBufferPointer p2;
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
        outpost::utils::SmartBufferPointer p2;

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

TEST_F(SmartBufferTest, allocateChildBuffer)
{
    {
        outpost::utils::ChildSmartPointer child;

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);

        {
            outpost::utils::SmartBufferPointer mPointer;
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
            outpost::utils::SmartBufferPointer mPointer = child.getParent();

            outpost::utils::ChildSmartPointer newChild;

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

TEST_F(SmartBufferTest, allocateChildChildBuffer)
{
    {
        outpost::utils::ChildSmartPointer child;
        outpost::utils::ChildSmartPointer childChild;

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize);

        {
            outpost::utils::SmartBufferPointer mPointer;
            EXPECT_TRUE(mPool.allocate(mPointer));
            EXPECT_TRUE(mPointer.isValid());
            for (size_t i = 0; i < objectSize; i++)
            {
                mPointer[i] = i;
            }

            EXPECT_TRUE(mPointer.getChild(child, 1, 5, 5));
            EXPECT_TRUE(child.isValid());

            EXPECT_TRUE(child.getChild(childChild, 2, 5, 5));
            EXPECT_TRUE(childChild.isValid());
            EXPECT_TRUE(childChild.getOrigin() == mPointer);
            EXPECT_TRUE(childChild.getParent() == child);
            EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

            outpost::utils::ChildSmartPointer childChildChild;
            EXPECT_TRUE(childChild.getChild(childChildChild, 3, 0, 5));
            EXPECT_TRUE(childChildChild.getOrigin() == mPointer);
            EXPECT_TRUE(childChildChild == mPointer);
        }

        EXPECT_EQ(mPool.numberOfFreeElements(), poolSize - 1);

        {
            outpost::utils::SmartBufferPointer mPointer = child.getParent();

            outpost::utils::ChildSmartPointer newChild;

            EXPECT_TRUE(mPointer.getChild(newChild, 3, 2, 3));
            EXPECT_TRUE(newChild.isValid());

            EXPECT_TRUE(newChild.isChild());

            EXPECT_FALSE(mPointer.isChild());

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
