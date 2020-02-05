/*
 *
 * Copyright (c) 2017, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <outpost/base/fixpoint.h>
#include <outpost/compression/data_aggregation_policy.h>
#include <outpost/compression/data_aggregator.h>

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

    outpost::utils::SharedBufferPool<16384, 10> mPool;
    outpost::utils::ReferenceQueue<DataBlock, 8> mQueue;

    unittest::time::TestingClock mClock;
};

TEST_F(DataAggregationTest, Constructor)
{
    EXPECT_EQ(DataAggregatorBase::numberOfAggregators(), 0U);

    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    EXPECT_EQ(DataAggregatorBase::numberOfAggregators(), 1U);

    EXPECT_TRUE(&aggregator == DataAggregatorBase::findDataAggregator(123U));

    EXPECT_EQ(aggregator.getBlocksize(), Blocksize::bs16);
    EXPECT_EQ(aggregator.getSamplingRate(), SamplingRate::hz05);
    EXPECT_EQ(aggregator.getParameterId(), 123U);

    EXPECT_FALSE(aggregator.isEnabled());

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());
    EXPECT_FALSE(aggregator.hasSamplingRateChanged());
    EXPECT_TRUE(aggregator.isBlockEmpty());
}

TEST_F(DataAggregationTest, PushOnce)
{
    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    Fixpoint f = 1.5f;
    EXPECT_FALSE(aggregator.push(f));

    aggregator.enable();
    ASSERT_TRUE(aggregator.isEnabled());
    EXPECT_TRUE(aggregator.push(f));

    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 1U);

    DataBlock b;
    EXPECT_FALSE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_FALSE(b.isValid());
    EXPECT_FALSE(b.isComplete());
}

TEST_F(DataAggregationTest, PushBlock)
{
    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    aggregator.enable();

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 16U - 1; i++)
    {
        EXPECT_TRUE(aggregator.push(f));
    }
    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 1U);
    DataBlock b;
    EXPECT_FALSE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_FALSE(b.isValid());
    EXPECT_FALSE(b.isComplete());

    mClock.incrementBy(outpost::time::Seconds(30U));

    EXPECT_TRUE(aggregator.push(f));
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

    EXPECT_TRUE(aggregator.push(f));

    EXPECT_EQ(mPool.numberOfElements(), mPool.numberOfFreeElements() + 2U);
}

TEST_F(DataAggregationTest, DisableAfterBlock)
{
    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    aggregator.enable();

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 16U - 1; i++)
    {
        EXPECT_TRUE(aggregator.push(f));
    }
    aggregator.disableAfterCurrentBlock();
    EXPECT_TRUE(aggregator.push(f));

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));

    EXPECT_FALSE(aggregator.push(f));
}

TEST_F(DataAggregationTest, EnableForOneBlock)
{
    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    aggregator.enableForOneBlock();

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < toUInt(Blocksize::bs16); i++)
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));

    EXPECT_FALSE(aggregator.push(f));
}

TEST_F(DataAggregationTest, Disable)
{
    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    aggregator.enable();

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 2U; i++)
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    aggregator.disable();

    EXPECT_FALSE(aggregator.push(f));
    EXPECT_FALSE(aggregator.isEnabled());
}

TEST_F(DataAggregationTest, ChangeBlocksize)
{
    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());

    aggregator.enable();

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 2U; i++)
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());

    aggregator.setBlocksize(Blocksize::bs128);
    EXPECT_TRUE(aggregator.hasBlocksizeChanged());

    aggregator.setBlocksize(Blocksize::bs16);
    EXPECT_FALSE(aggregator.hasBlocksizeChanged());

    aggregator.setBlocksize(Blocksize::bs128);

    EXPECT_EQ(aggregator.getBlocksize(), Blocksize::bs16);
    EXPECT_EQ(aggregator.getNextBlocksize(), Blocksize::bs128);

    while (!aggregator.isBlockEmpty())
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    EXPECT_TRUE(aggregator.hasBlocksizeChanged());

    EXPECT_TRUE(aggregator.push(f));

    EXPECT_FALSE(aggregator.hasBlocksizeChanged());
    EXPECT_EQ(aggregator.getBlocksize(), Blocksize::bs128);
    EXPECT_EQ(aggregator.getNextBlocksize(), Blocksize::bs128);

    while (!aggregator.isBlockEmpty())
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getBlocksize(), Blocksize::bs16);

    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getBlocksize(), Blocksize::bs128);
}

TEST_F(DataAggregationTest, ChangeSamplingRate)
{
    DataAggregator<outpost::compression::data_aggregation_policy::TryOnce> aggregator(
            123U, SamplingRate::hz05, Blocksize::bs16, mPool, mQueue, mClock);

    EXPECT_FALSE(aggregator.hasSamplingRateChanged());

    aggregator.enable();

    Fixpoint f = 1.5f;
    for (size_t i = 0; i < 2U; i++)
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    EXPECT_FALSE(aggregator.hasSamplingRateChanged());

    aggregator.setSamplingRate(SamplingRate::hz10);

    EXPECT_TRUE(aggregator.hasSamplingRateChanged());

    aggregator.setSamplingRate(SamplingRate::hz05);
    EXPECT_FALSE(aggregator.hasSamplingRateChanged());

    aggregator.setSamplingRate(SamplingRate::hz10);

    EXPECT_EQ(aggregator.getSamplingRate(), SamplingRate::hz05);
    EXPECT_EQ(aggregator.getNextSamplingRate(), SamplingRate::hz10);

    while (!aggregator.isBlockEmpty())
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    EXPECT_TRUE(aggregator.hasSamplingRateChanged());

    EXPECT_TRUE(aggregator.push(f));

    EXPECT_FALSE(aggregator.hasSamplingRateChanged());
    EXPECT_EQ(aggregator.getSamplingRate(), SamplingRate::hz10);
    EXPECT_EQ(aggregator.getNextSamplingRate(), SamplingRate::hz10);

    while (!aggregator.isBlockEmpty())
    {
        EXPECT_TRUE(aggregator.push(f));
    }

    DataBlock b;
    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getSamplingRate(), SamplingRate::hz05);

    ASSERT_TRUE(mQueue.receive(b, outpost::time::Duration::zero()));
    EXPECT_EQ(b.getSamplingRate(), SamplingRate::hz10);
}

}  // namespace data_aggregation_test
