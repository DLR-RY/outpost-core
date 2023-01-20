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
        EXPECT_FALSE(block.isValid());
        EXPECT_FALSE(block.isTransformed());
        EXPECT_FALSE(block.isEncoded());
        EXPECT_FALSE(block.isComplete());
    }

    outpost::utils::SharedBufferPointer p;
    ASSERT_TRUE(mPool.allocate(p));
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
        EXPECT_TRUE(block.isValid());
        EXPECT_FALSE(block.isComplete());

        EXPECT_EQ(block.getEncodedData().getNumberOfElements(), 0U);

        EXPECT_FALSE(block.isTransformed());
        EXPECT_FALSE(block.isEncoded());
    }
}

TEST_F(DataBlockTest, Push)
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
        EXPECT_FALSE(block.isComplete());
        EXPECT_TRUE(block.push(Fixpoint(i)));
        EXPECT_EQ(block.getSampleCount(), i + 1);
        EXPECT_TRUE(block.isValid());
    }
    EXPECT_TRUE(block.isComplete());
    EXPECT_FALSE(block.push(Fixpoint(123)));
    EXPECT_EQ(block.getSampleCount(), 16U);
    EXPECT_TRUE(block.isValid());

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

    outpost::time::GpsTime time = outpost::time::GpsTime::afterEpoch(outpost::time::Hours(3U));
    time = time + outpost::time::Seconds(56U) + outpost::time::Milliseconds(123U);

    outpost::compression::DataBlock block(p,
                                          123U,
                                          time,
                                          outpost::compression::SamplingRate::hz05,
                                          outpost::compression::Blocksize::bs16);

    for (int16_t i = 0; i < 16; i++)
    {
        block.push(Fixpoint(i));
    }

    ASSERT_TRUE(block.applyWaveletTransform());

    outpost::utils::SharedBufferPointer p_out;
    ASSERT_TRUE(mPool.allocate(p_out));
    outpost::compression::DataBlock block_out(p_out,
                                              block.getParameterId(),
                                              block.getStartTime(),
                                              block.getSamplingRate(),
                                              block.getBlocksize());

    ASSERT_TRUE(block.encode(block_out, encoder));
    EXPECT_TRUE(block_out.isEncoded());

    outpost::Slice<uint8_t> enc = block_out.getEncodedData();
    EXPECT_EQ(enc.getNumberOfElements(), 21U);
    EXPECT_EQ(enc[0], 1U);
    EXPECT_EQ(enc[1], 0U);
    EXPECT_EQ(enc[2], 123U);
    uint8_t encoded_sr_bs = static_cast<uint8_t>(outpost::compression::SamplingRate::hz05) << 4;
    encoded_sr_bs |= static_cast<uint8_t>(outpost::compression::Blocksize::bs16);
    uint32_t seconds = (enc[3] << 24U) + (enc[4] << 16U) + (enc[5] << 8U) + enc[6];
    EXPECT_EQ(seconds, 10856U);
    uint16_t milliseconds = (enc[7] << 8U) + enc[8];
    EXPECT_EQ(milliseconds, 123U);
    EXPECT_EQ(enc[9], encoded_sr_bs);
}

}  // namespace data_block_test
