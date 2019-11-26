/*
 *
 * Copyright (c) 2017, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "regression_data.h"

#include <outpost/base/fixpoint.h>
#include <outpost/base/slice.h>
#include <outpost/utils/compression/nls_encoder.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <inttypes.h>

using namespace testing;
using namespace outpost;

static constexpr size_t bufferLength = 4096U;

static int16_t outputBuffer[bufferLength];

static int16_t inputBuffer[bufferLength];

static uint8_t bitStreamBuffer[2 * bufferLength];
static outpost::Slice<uint8_t> bitStreamSlice(bitStreamBuffer);

class NLSRegressionTest : public ::testing::Test
{
public:
    NLSRegressionTest()
    {
    }

    virtual ~NLSRegressionTest()
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

    outpost::compression::NLSEncoder encoder;
};

TEST_F(NLSRegressionTest, Test_Dataset1)
{
    memcpy(inputBuffer, regression_input1, 4096 * sizeof(int16_t));
    outpost::Bitstream bitstream(bitStreamSlice);

    encoder.encode(inputBuffer, bufferLength, bitstream);
    outpost::Serialize s_stream(bitStreamSlice);
    bitstream.serialize(s_stream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);

    outpost::Bitstream bitstream_out(bitStreamSlice);
    outpost::Deserialize stream(bitStreamSlice);
    bitstream_out.deserialize(stream);

    size_t outBufferLength;
    encoder.decode(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    for (size_t i = 0; i < outBufferLength; i++)
    {
        EXPECT_EQ(outputBuffer[i], regression_input1[i]);
    }
}

TEST_F(NLSRegressionTest, Test_Dataset2)
{
    memcpy(inputBuffer, regression_input2, 4096 * sizeof(int16_t));
    outpost::Bitstream bitstream(bitStreamSlice);

    encoder.encode(inputBuffer, bufferLength, bitstream);
    outpost::Serialize s_stream(bitStreamSlice);
    bitstream.serialize(s_stream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);

    outpost::Bitstream bitstream_out(bitStreamSlice);
    outpost::Deserialize stream(bitStreamSlice);
    bitstream_out.deserialize(stream);

    size_t outBufferLength;
    encoder.decode(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    for (size_t i = 0; i < outBufferLength; i++)
    {
        EXPECT_EQ(outputBuffer[i], regression_input2[i]);
    }
}
