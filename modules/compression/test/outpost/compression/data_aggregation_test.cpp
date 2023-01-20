/*
 *
 * Copyright (c) 2017, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <outpost/base/fixpoint.h>
#include <outpost/compression/data_aggregator.h>
#include <outpost/compression/data_block.h>
#include <outpost/compression/data_block_sender.h>
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
class DataAggregationTest : public ::testing::Test
{
public:
    DataAggregationTest()
    {
    }

    virtual ~DataAggregationTest()
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

    outpost::utils::SharedBufferPool<16500, 10> mPool;
    outpost::utils::ReferenceQueue<DataBlock, 8> mQueue;

    outpost::time::GpsTime now =
            outpost::time::GpsTime::afterEpoch(outpost::time::Duration::zero());

    unittest::time::TestingClock mClock;
};

TEST_F(DataAggregationTest, TrivialConstructor)
{
    OneTimeQueueSender ots(mQueue);

    DataAggregator aggregator;
    EXPECT_FALSE(aggregator.isEnabled());
    EXPECT_FALSE(aggregator.enable(SamplingRate::hz1, Blocksize::bs16));
}

TEST_F(DataAggregationTest, AggregatorArray)
{
    OneTimeQueueSender ots(mQueue);

    DataAggregator aggregators[3];

    for (size_t i = 0; i < 3; i++)
    {
        EXPECT_FALSE(aggregators[i].enable(SamplingRate::hz1, Blocksize::bs16));
    }

    EXPECT_TRUE(aggregators[0].initialize(123U, mPool, ots));
    EXPECT_TRUE(aggregators[1].initialize(124U, mPool, ots));
    EXPECT_FALSE(aggregators[2].initialize(124U, mPool, ots));

    EXPECT_TRUE(aggregators[0].enable(SamplingRate::hz1, Blocksize::bs16));
    EXPECT_TRUE(aggregators[1].enable(SamplingRate::hz1, Blocksize::bs16));
    EXPECT_FALSE(aggregators[2].enable(SamplingRate::hz1, Blocksize::bs16));
}

TEST_F(DataAggregationTest, Initialize)
{
    OneTimeQueueSender ots(mQueue);
    Fixpoint f = 1.0f;

    DataAggregator aggregator;
    EXPECT_FALSE(aggregator.isEnabled());
    EXPECT_FALSE(aggregator.enable(SamplingRate::hz1, Blocksize::bs16));

    EXPECT_FALSE(aggregator.initialize(0, mPool, ots));
    EXPECT_FALSE(aggregator.enable(SamplingRate::hz1, Blocksize::bs16));
    EXPECT_FALSE(aggregator.push(f, now));

    EXPECT_TRUE(aggregator.initialize(123U, mPool, ots));
    EXPECT_TRUE(aggregator.enable(SamplingRate::hz1, Blocksize::bs16));
    EXPECT_TRUE(aggregator.push(f, now));
}

TEST_F(DataAggregationTest, Constructor)
{
    OneTimeQueueSender ots(mQueue);
    EXPECT_EQ(DataAggregator::numberOfAggregators(), 0U);

    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_EQ(DataAggregator::numberOfAggregators(), 1U);

    EXPECT_TRUE(&aggregator == DataAggregator::findDataAggregator(123U));

    EXPECT_EQ(aggregator.getBlocksize(), Blocksize::disabled);
    EXPECT_EQ(aggregator.getSamplingRate(), SamplingRate::disabled);
    EXPECT_EQ(aggregator.getParameterId(), 123U);

    EXPECT_FALSE(aggregator.isEnabled());

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());
    EXPECT_FALSE(aggregator.hasSamplingRateChanged());
    EXPECT_FALSE(aggregator.isAtStartOfNewBlock());
}

TEST_F(DataAggregationTest, PushOnce)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    Fixpoint f = 1.5f;
    EXPECT_FALSE(aggregator.push(f, now));

    aggregator.enable(SamplingRate::hz05, Blocksize::bs16);
    ASSERT_TRUE(aggregator.isEnabled());
    EXPECT_TRUE(aggregator.push(f, now));

    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 1U);

    DataBlock b;
    EXPECT_FALSE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_FALSE(b.isValid());
    EXPECT_FALSE(b.isComplete());
}

TEST_F(DataAggregationTest, PushBlock)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz05, Blocksize::bs16));

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 16U - 1; i++)
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }
    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 1U);
    DataBlock b;
    EXPECT_FALSE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_FALSE(b.isValid());
    EXPECT_FALSE(b.isComplete());

    mClock.incrementBy(outpost::time::Seconds(30U));

    EXPECT_TRUE(aggregator.push(f, now));
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));

    EXPECT_TRUE(b.isValid());
    EXPECT_TRUE(b.isComplete());
    EXPECT_EQ(b.getBlocksize(), Blocksize::bs16);
    EXPECT_EQ(b.getSampleCount(), 16U);
    EXPECT_EQ(b.getParameterId(), 123U);

    EXPECT_EQ(b.getSamplingRate(), SamplingRate::hz05);
    EXPECT_EQ(b.getStartTime(),
              outpost::time::GpsTime::afterEpoch(outpost::time::Duration::zero()));

    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 1U);

    EXPECT_TRUE(aggregator.push(f, now));

    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 2U);
}

TEST_F(DataAggregationTest, PushBlock4096)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz05, Blocksize::bs4096));

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 4096U - 1; i++)
    {
        Fixpoint fadd(static_cast<int>(i));
        fadd = f + fadd;
        EXPECT_TRUE(aggregator.push(fadd, now));
    }
    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 1U);
    DataBlock b;
    EXPECT_FALSE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_FALSE(b.isValid());
    EXPECT_FALSE(b.isComplete());

    mClock.incrementBy(outpost::time::Seconds(30U));

    EXPECT_TRUE(aggregator.push(f, now));
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));

    EXPECT_TRUE(b.isValid());
    EXPECT_TRUE(b.isComplete());
    EXPECT_EQ(b.getBlocksize(), Blocksize::bs4096);
    EXPECT_EQ(b.getSampleCount(), 4096U);
    EXPECT_EQ(b.getParameterId(), 123U);

    EXPECT_EQ(b.getSamplingRate(), SamplingRate::hz05);
    EXPECT_EQ(b.getStartTime(),
              outpost::time::GpsTime::afterEpoch(outpost::time::Duration::zero()));

    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 1U);

    EXPECT_TRUE(aggregator.push(f, now));

    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 2U);
}

TEST_F(DataAggregationTest, DisableAfterBlock)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz05, Blocksize::bs16));

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 16U - 1; i++)
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }
    aggregator.disableAfterCurrentBlock();
    EXPECT_TRUE(aggregator.isEnabled());

    EXPECT_TRUE(aggregator.push(f, now));

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));

    EXPECT_FALSE(aggregator.push(f, now));
    EXPECT_FALSE(aggregator.isEnabled());
}

TEST_F(DataAggregationTest, EnableForOneBlock)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_TRUE(aggregator.enableForOneBlock(SamplingRate::hz05, Blocksize::bs16));

    EXPECT_TRUE(aggregator.isEnabled());

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < toUInt(Blocksize::bs16); i++)
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));

    EXPECT_FALSE(aggregator.isEnabled());
    EXPECT_FALSE(aggregator.push(f, now));
}

TEST_F(DataAggregationTest, Disable)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz05, Blocksize::bs16));

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 2U; i++)
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    aggregator.disable();

    EXPECT_FALSE(aggregator.push(f, now));
    EXPECT_FALSE(aggregator.isEnabled());
}

TEST_F(DataAggregationTest, ChangeBlocksize)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz05, Blocksize::bs16));

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 2U; i++)
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());

    aggregator.setNextBlocksize(Blocksize::bs128);
    EXPECT_TRUE(aggregator.hasBlocksizeChanged());

    aggregator.setNextBlocksize(Blocksize::bs16);
    EXPECT_FALSE(aggregator.hasBlocksizeChanged());

    aggregator.setNextBlocksize(Blocksize::bs128);

    EXPECT_EQ(aggregator.getBlocksize(), Blocksize::bs16);
    EXPECT_EQ(aggregator.getNextBlocksize(), Blocksize::bs128);

    while (aggregator.isAtStartOfNewBlock())
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    EXPECT_TRUE(aggregator.hasBlocksizeChanged());

    EXPECT_TRUE(aggregator.push(f, now));

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());
    EXPECT_EQ(aggregator.getBlocksize(), Blocksize::bs128);
    EXPECT_EQ(aggregator.getNextBlocksize(), Blocksize::bs128);

    while (aggregator.isAtStartOfNewBlock())
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getBlocksize(), Blocksize::bs16);

    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getBlocksize(), Blocksize::bs128);
}

TEST_F(DataAggregationTest, ChangeSamplingRate)
{
    OneTimeQueueSender ots(mQueue);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_FALSE(aggregator.hasSamplingRateChanged());

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz05, Blocksize::bs16));

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 2U; i++)
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    EXPECT_FALSE(aggregator.hasSamplingRateChanged());

    aggregator.setNextSamplingRate(SamplingRate::hz10);

    EXPECT_TRUE(aggregator.hasSamplingRateChanged());

    aggregator.setNextSamplingRate(SamplingRate::hz05);
    EXPECT_FALSE(aggregator.hasSamplingRateChanged());

    aggregator.setNextSamplingRate(SamplingRate::hz10);

    EXPECT_EQ(aggregator.getSamplingRate(), SamplingRate::hz05);
    EXPECT_EQ(aggregator.getNextSamplingRate(), SamplingRate::hz10);

    while (aggregator.isAtStartOfNewBlock())
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    EXPECT_TRUE(aggregator.hasSamplingRateChanged());

    EXPECT_TRUE(aggregator.push(f, now));

    EXPECT_FALSE(aggregator.hasSamplingRateChanged());
    EXPECT_EQ(aggregator.getSamplingRate(), SamplingRate::hz10);
    EXPECT_EQ(aggregator.getNextSamplingRate(), SamplingRate::hz10);

    while (aggregator.isAtStartOfNewBlock())
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getSamplingRate(), SamplingRate::hz05);

    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getSamplingRate(), SamplingRate::hz10);
}

TEST_F(DataAggregationTest, EmptyPool)
{
    outpost::utils::ReferenceQueue<DataBlock, 12> q;
    OneTimeQueueSender ots(q);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz01, Blocksize::bs16));

    Fixpoint f = 1.0f;
    while (mPool.numberOfFreeElements() > 0 || aggregator.isAtStartOfNewBlock())
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }
    EXPECT_EQ(aggregator.getNumCompletedBlocks(), 10U);
    EXPECT_EQ(aggregator.getNumOverallSamples(), 160U);

    EXPECT_FALSE(aggregator.push(f, now));
    EXPECT_EQ(aggregator.getNumLostBlocks(), 0U);
    EXPECT_EQ(aggregator.getNumLostSamples(), 1U);

    {
        DataBlock b;
        EXPECT_TRUE(q.receive(b, outpost::time::Duration::zero()));
    }

    EXPECT_TRUE(aggregator.push(f, now));
}

TEST_F(DataAggregationTest, FullQueue)
{
    outpost::utils::ReferenceQueue<DataBlock, 1> q;
    OneTimeQueueSender ots(q);
    DataAggregator aggregator(123U, mPool, ots);

    EXPECT_TRUE(aggregator.enable(SamplingRate::hz01, Blocksize::bs16));

    Fixpoint f = 1.0f;
    for (size_t i = 0; i < 48U; i++)
    {
        EXPECT_TRUE(aggregator.push(f, now));
    }

    EXPECT_EQ(aggregator.getNumLostSamples(), 0U);
    EXPECT_EQ(aggregator.getNumLostBlocks(), 2U);
    EXPECT_EQ(aggregator.getNumCompletedBlocks(), 3U);
}

}  // namespace data_aggregation_test
