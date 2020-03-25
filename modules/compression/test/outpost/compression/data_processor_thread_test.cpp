/*
 *
 * Copyright (c) 2020, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <outpost/base/fixpoint.h>
#include <outpost/compression/data_block.h>
#include <outpost/compression/data_processor_thread.h>
#include <outpost/utils/container/reference_queue.h>
#include <outpost/utils/container/shared_object_pool.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unittest/time/testing_clock.h>

using namespace testing;
using namespace outpost;
using namespace outpost::compression;

namespace data_aggregation_test
{
class DataProcessorThreadTest : public ::testing::Test
{
public:
    DataProcessorThreadTest()
    {
    }

    virtual ~DataProcessorThreadTest()
    {
    }

    virtual void
    SetUp()
    {
        mClock.setTime(
                outpost::time::SpacecraftElapsedTime::afterEpoch(outpost::time::Duration::zero()));
    }

    virtual void
    TearDown()
    {
    }

    outpost::utils::SharedBufferPool<16384, 20> mPool;
    outpost::utils::ReferenceQueue<DataBlock, 8> mInputQueue;
    outpost::utils::ReferenceQueue<DataBlock, 8> mOutputQueue;

    unittest::time::TestingClock mClock;
};

TEST_F(DataProcessorThreadTest, Constructor)
{
    DataProcessorThread thread(123U, mPool, mInputQueue, mOutputQueue);

    EXPECT_EQ(thread.getNumberOfReceivedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfProcessedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfForwardedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfLostBlocks(), 0U);

    EXPECT_FALSE(thread.isEnabled());
}

TEST_F(DataProcessorThreadTest, EnableDisable)
{
    DataProcessorThread thread(123U, mPool, mInputQueue, mOutputQueue);

    thread.enable();
    EXPECT_TRUE(thread.isEnabled());

    thread.disable();
    EXPECT_FALSE(thread.isEnabled());
}

TEST_F(DataProcessorThreadTest, emptyQueue)
{
    DataProcessorThread thread(123U, mPool, mInputQueue, mOutputQueue);

    thread.processSingleBlock(outpost::time::Duration::zero());

    EXPECT_EQ(thread.getNumberOfReceivedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfProcessedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfForwardedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfLostBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfLostBlocks(), 0U);
}

TEST_F(DataProcessorThreadTest, processSingleInvalidBlock)
{
    DataProcessorThread thread(123U, mPool, mInputQueue, mOutputQueue);

    outpost::compression::DataBlock block;
    mInputQueue.send(block);

    thread.processSingleBlock(outpost::time::Duration::zero());

    EXPECT_EQ(thread.getNumberOfReceivedBlocks(), 1U);
    EXPECT_EQ(thread.getNumberOfProcessedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfForwardedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfLostBlocks(), 0U);

    EXPECT_FALSE(mOutputQueue.receive(block, outpost::time::Duration::zero()));
}

TEST_F(DataProcessorThreadTest, processSingleBlock)
{
    DataProcessorThread thread(123U, mPool, mInputQueue, mOutputQueue, 2U, outpost::time::Duration::zero());

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));

    outpost::compression::DataBlock block(
            p,
            123U,
            outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)),
            outpost::compression::SamplingRate::hz05,
            outpost::compression::Blocksize::bs16);

    for (int32_t i = 0; i < 16; i++)
    {
        block.push(outpost::Fixpoint(i));
    }
    mInputQueue.send(block);

    thread.processSingleBlock(outpost::time::Duration::zero());

    EXPECT_EQ(thread.getNumberOfReceivedBlocks(), 1U);
    EXPECT_EQ(thread.getNumberOfProcessedBlocks(), 1U);
    EXPECT_EQ(thread.getNumberOfForwardedBlocks(), 1U);
    EXPECT_EQ(thread.getNumberOfLostBlocks(), 0U);

    EXPECT_TRUE(mOutputQueue.receive(block, outpost::time::Duration::zero()));

    EXPECT_EQ(block.getSamplingRate(), outpost::compression::SamplingRate::hz05);
    EXPECT_EQ(block.getBlocksize(), outpost::compression::Blocksize::bs16);
    EXPECT_TRUE(block.isEncoded());
    outpost::Slice<uint8_t> encodedData = block.getEncodedData();
    EXPECT_EQ(encodedData.getNumberOfElements(),
              DataBlock::headerSize + Bitstream::headerSize + 8U);
}

TEST_F(DataProcessorThreadTest, processMultipleBlocks)
{
    DataProcessorThread thread(123U, mPool, mInputQueue, mOutputQueue, 2U, outpost::time::Duration::zero());

    for (size_t blocks = 0; blocks < 10; blocks++)
    {
        outpost::utils::SharedBufferPointer p;
        ASSERT_TRUE(mPool.allocate(p));

        outpost::compression::DataBlock block(
                p,
                123U,
                outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)),
                outpost::compression::SamplingRate::hz05,
                outpost::compression::Blocksize::bs16);

        for (int32_t i = 0; i < 16; i++)
        {
            block.push(outpost::Fixpoint(i));
        }
        mInputQueue.send(block);

        thread.processSingleBlock(outpost::time::Duration::zero());
    }

    EXPECT_EQ(thread.getNumberOfReceivedBlocks(), 10U);
    EXPECT_EQ(thread.getNumberOfProcessedBlocks(), 10U);
    EXPECT_EQ(thread.getNumberOfForwardedBlocks(), 8U);
    EXPECT_EQ(thread.getNumberOfLostBlocks(), 2U);

    DataBlock b;
    for (size_t blocks = 0; blocks < 8; blocks++)
    {
        EXPECT_TRUE(mOutputQueue.receive(b));
    }
    EXPECT_FALSE(mOutputQueue.receive(b, outpost::time::Duration::zero()));

    thread.resetCounters();

    EXPECT_EQ(thread.getNumberOfReceivedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfProcessedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfForwardedBlocks(), 0U);
    EXPECT_EQ(thread.getNumberOfLostBlocks(), 0U);
}

}  // namespace data_aggregation_test
