/*
 *
 * Copyright (c) 2020, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <outpost/compression/data_block.h>
#include <outpost/compression/nls_encoder.h>
#include <outpost/utils/container/shared_buffer.h>
#include <outpost/utils/container/shared_object_pool.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unittest/harness.h>

using namespace testing;
using namespace outpost;
using namespace compression;

namespace data_block_test
{
class DataBlockTest : public ::testing::Test
{
public:
    DataBlockTest()
    {
    }

    virtual ~DataBlockTest()
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

    outpost::utils::SharedBufferPool<16384, 20> mPool;
};

TEST_F(DataBlockTest, ToUInt)
{
    EXPECT_EQ(toUInt(Blocksize::disabled), 0U);
    EXPECT_EQ(toUInt(Blocksize::bs16), 16U);
    EXPECT_EQ(toUInt(Blocksize::bs128), 128U);
    EXPECT_EQ(toUInt(Blocksize::bs256), 256U);
    EXPECT_EQ(toUInt(Blocksize::bs512), 512U);
    EXPECT_EQ(toUInt(Blocksize::bs1024), 1024U);
    EXPECT_EQ(toUInt(Blocksize::bs2048), 2048U);
    EXPECT_EQ(toUInt(Blocksize::bs4096), 4096U);
}

TEST_F(DataBlockTest, Constructor)
{
    {
        outpost::compression::DataBlock block;
        EXPECT_EQ(block.getBlocksize(), outpost::compression::Blocksize::disabled);
        EXPECT_EQ(block.getSamplingRate(), outpost::compression::SamplingRate::disabled);
        EXPECT_EQ(block.getParameterId(), 0U);
        EXPECT_EQ(block.getStartTime(),
                  outpost::time::GpsTime::afterEpoch(outpost::time::Duration::zero()));
        EXPECT_EQ(block.getSamples().getNumberOfElements(), 0U);
        EXPECT_EQ(block.getSampleCount(), 0);
    }

    outpost::utils::SharedBufferPointer p;
    mPool.allocate(p);
    {
        outpost::compression::DataBlock block(
                p,
                123U,
                outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)),
                outpost::compression::SamplingRate::hz05,
                outpost::compression::Blocksize::bs16);
        EXPECT_EQ(block.getBlocksize(), outpost::compression::Blocksize::bs16);
        EXPECT_EQ(block.getSamplingRate(), outpost::compression::SamplingRate::hz05);
        EXPECT_EQ(block.getParameterId(), 123U);
        EXPECT_EQ(block.getStartTime(),
                  outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)));
        EXPECT_EQ(block.getSamples().getNumberOfElements(), 0U);
        EXPECT_EQ(block.getSampleCount(), 0);

        EXPECT_EQ(block.getEncodedDataBlock().getNumberOfElements(), 0U);

        EXPECT_FALSE(block.isTransformed());
        EXPECT_FALSE(block.isEncoded());
    }
}

TEST_F(DataBlockTest, Push)
{
    outpost::utils::SharedBufferPointer p;
    mPool.allocate(p);
    outpost::compression::DataBlock block(
            p,
            123U,
            outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)),
            outpost::compression::SamplingRate::hz05,
            outpost::compression::Blocksize::bs16);

    for (int32_t i = 0; i < 16; i++)
    {
        EXPECT_FALSE(block.isComplete());
        EXPECT_TRUE(block.push(Fixpoint(i)));
        EXPECT_EQ(block.getSampleCount(), i + 1);
    }
    EXPECT_TRUE(block.isComplete());
    EXPECT_FALSE(block.push(Fixpoint(123)));
    EXPECT_EQ(block.getSampleCount(), 16U);

    outpost::Slice<Fixpoint> slice = block.getSamples();
    EXPECT_EQ(slice.getNumberOfElements(), 16U);

    for (int32_t i = 0; i < 16; i++)
    {
        EXPECT_EQ(slice[i], Fixpoint(i));
    }
}

TEST_F(DataBlockTest, GetSamples)
{
    outpost::utils::SharedBufferPointer p;
    mPool.allocate(p);
    outpost::compression::DataBlock block(
            p,
            123U,
            outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)),
            outpost::compression::SamplingRate::hz05,
            outpost::compression::Blocksize::bs16);

    for (int16_t i = 0; i < 16; i++)
    {
        block.push(Fixpoint(i));
    }

    outpost::Slice<Fixpoint> samples = block.getSamples();
    EXPECT_EQ(samples.getNumberOfElements(), 16U);

    for (int16_t i = 0; i < 16; i++)
    {
        EXPECT_EQ(samples[i], Fixpoint(i));
    }

    outpost::Slice<int16_t> coefficients = block.getCoefficients();
    EXPECT_EQ(coefficients.getNumberOfElements(), 0U);
}

TEST_F(DataBlockTest, GetCoefficients)
{
    outpost::utils::SharedBufferPointer p;
    mPool.allocate(p);
    outpost::compression::DataBlock block(
            p,
            123U,
            outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)),
            outpost::compression::SamplingRate::hz05,
            outpost::compression::Blocksize::bs16);

    for (int16_t i = 0; i < 16; i++)
    {
        block.push(Fixpoint(i));
    }
    EXPECT_FALSE(block.isTransformed());
    block.applyWaveletTransform();
    EXPECT_TRUE(block.isTransformed());
    EXPECT_FALSE(block.isEncoded());

    outpost::Slice<Fixpoint> samples = block.getSamples();
    EXPECT_EQ(samples.getNumberOfElements(), 0U);

    outpost::Slice<int16_t> coefficients = block.getCoefficients();
    EXPECT_EQ(coefficients.getNumberOfElements(), 16U);
}

TEST_F(DataBlockTest, Encode)
{
    outpost::compression::NLSEncoder encoder;

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
    outpost::compression::DataBlock block(
            p,
            123U,
            outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U)),
            outpost::compression::SamplingRate::hz05,
            outpost::compression::Blocksize::bs16);

    for (int16_t i = 0; i < 16; i++)
    {
        block.push(Fixpoint(i));
    }

    block.applyWaveletTransform();

    outpost::utils::SharedBufferPointer p_out;
    ASSERT_TRUE(mPool.allocate(p_out));
    outpost::compression::DataBlock block_out(p_out,
                                              block.getParameterId(),
                                              block.getStartTime(),
                                              block.getSamplingRate(),
                                              block.getBlocksize());

    EXPECT_TRUE(block.encode(block_out, encoder));
    EXPECT_TRUE(block_out.isEncoded());
}

}  // namespace data_block_test
