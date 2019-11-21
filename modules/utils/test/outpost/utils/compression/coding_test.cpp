/*
 *
 * Copyright (c) 2017, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include <outpost/base/fixpoint.h>
#include <outpost/base/slice.h>
#include <outpost/utils/compression/legall_wavelet.h>
#include <outpost/utils/compression/nls_transformer.h>
#include <outpost/utils/storage/bitstream.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unittest/utils/compression/reorder_stub.h>

using namespace testing;
using namespace outpost;

static constexpr size_t bufferLength = 64U;

static outpost::FP<16> waveletInputBuffer[bufferLength];
static int16_t inputBuffer[bufferLength];
static int16_t inputReference[bufferLength];
static uint8_t bitStreamBuffer[2 * bufferLength];
static int16_t outputBuffer[bufferLength];
static double outBuffer[bufferLength];

static outpost::Slice<int16_t> inputData(inputBuffer);
static outpost::Slice<uint8_t> bitStreamData(bitStreamBuffer);
static outpost::Slice<int16_t> outputData(outputBuffer);

class CodingTest : public ::testing::Test
{
public:
    CodingTest()
    {
    }

    virtual ~CodingTest()
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

    outpost::compression::NLSTransformer transformer;
};

TEST_F(CodingTest, ConstantTest)
{
    const int16_t c = int16_t(2024);

    for (uint32_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = c;
        inputReference[i] = c;
    }

    outpost::Bitstream bitstream(bitStreamData);

    transformer.forward(inputBuffer, bufferLength, bitstream);
    outpost::Serialize s_stream(bitStreamData);
    bitstream.serialize(s_stream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSize()], 0, 2 * bufferLength - bitstream.getSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    size_t outBufferLength;
    transformer.backward(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    for (size_t i = 0; i < outBufferLength; i++)
    {
        EXPECT_EQ(outputBuffer[i], inputReference[i]);
    }
}

TEST_F(CodingTest, ConstantTestWithCompression)
{
    const int16_t c = int16_t(2021);

    for (uint32_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = c;
        inputReference[i] = c;
    }

    outpost::Bitstream bitstream_in(bitStreamData);

    transformer.forward(inputBuffer, bufferLength, bitstream_in);
    outpost::Serialize s_stream(bitStreamData);
    bitstream_in.serialize(s_stream);

    EXPECT_LE(bitstream_in.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream_in.getSize()], 0, 2 * bufferLength - bitstream_in.getSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    size_t outBufferLength;
    transformer.backward(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    for (size_t i = 0; i < outBufferLength; i++)
    {
        uint16_t dif = std::abs(outputBuffer[i] - inputReference[i]);
        EXPECT_LE(dif, 16U);
    }
}

TEST_F(CodingTest, LinearTest)
{
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = static_cast<int16_t>(i * 7);
        inputReference[i] = static_cast<int16_t>(i * 7);
    }

    outpost::Bitstream bitstream(bitStreamData);

    transformer.forward(inputBuffer, bufferLength, bitstream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSerializedSize()],
           0,
           2 * bufferLength - bitstream.getSerializedSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    size_t outBufferLength;
    transformer.backward(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    for (size_t i = 0; i < outBufferLength; i++)
    {
        EXPECT_EQ(outputBuffer[i], inputReference[i]);
    }
}

TEST_F(CodingTest, LinearTestWithCompression)
{
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        if (i < 8)
        {
            inputBuffer[i] = static_cast<int16_t>(i + 1024U);
            inputReference[i] = static_cast<int16_t>(i + 1024U);
        }
        else if (i % 2)
        {
            inputBuffer[i] = static_cast<int16_t>(i + 432U);
            inputReference[i] = static_cast<int16_t>(i + 432U);
        }
        else
        {
            inputBuffer[i] = static_cast<int16_t>(i - 1467U);
            inputReference[i] = static_cast<int16_t>(i - 1467U);
        }
    }

    outpost::Bitstream bitstream_in(bitStreamData);

    transformer.forward(inputBuffer, bufferLength, bitstream_in);

    EXPECT_EQ(bitstream_in.getSerializedSize(), 107U);
    outpost::Serialize s_stream(bitStreamData);
    bitstream_in.serialize(s_stream, 83);

    EXPECT_LE(bitstream_in.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream_in.getSerializedSize()],
           0,
           2 * bufferLength - bitstream_in.getSerializedSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    size_t outBufferLength;
    transformer.backward(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    double mse = 0.0f;
    for (uint32_t i = 0; i < outBufferLength; i++)
    {
        mse += std::abs(outputBuffer[i] - inputReference[i])
               * std::abs(outputBuffer[i] - inputReference[i]);
        uint16_t dif = std::abs(outputBuffer[i] - inputReference[i]);
        EXPECT_LE(dif, 5U);
    }
    mse /= bufferLength;
    EXPECT_LE(mse, 4);
}

TEST_F(CodingTest, WaveletTest)
{
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        inputReference[i] = static_cast<int16_t>(i + 1024U);
        waveletInputBuffer[i] = static_cast<int16_t>(i + 1024U);
    }

    outpost::compression::LeGall53Wavelet::forwardTransformInPlace(waveletInputBuffer,
                                                                   bufferLength);
    unittest::compression::ReorderStub::reorder(waveletInputBuffer, bufferLength);
    int16_t* inBuffer = reinterpret_cast<int16_t*>(waveletInputBuffer);

    outpost::Bitstream bitstream(bitStreamData);

    transformer.forward(inBuffer, bufferLength, bitstream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSerializedSize()],
           0,
           2 * bufferLength - bitstream.getSerializedSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    size_t outBufferLength;
    transformer.backward(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    for (uint32_t i = 0; i < outBufferLength; i++)
    {
        EXPECT_EQ(std::abs(outputBuffer[i]), std::abs(inBuffer[i]));
    }

    double doubleInBuffer[bufferLength];
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        doubleInBuffer[i] = static_cast<double>(outputBuffer[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(
            doubleInBuffer, outBuffer, bufferLength);

    double mse = 0.0f;
    for (uint32_t i = 0; i < outBufferLength; i++)
    {
        mse += (std::abs(outBuffer[i]) - std::abs(inputReference[i]))
               * (std::abs(outBuffer[i]) - std::abs(inputReference[i]));
        EXPECT_LE(std::abs(outBuffer[i]) - std::abs(inputReference[i]), 2.0f);
    }
    mse /= bufferLength;
    EXPECT_LE(mse, 0.4f);
}

TEST_F(CodingTest, WaveletTestWithCompression)
{
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        inputReference[i] = static_cast<int16_t>(3U * i + 1024U);
        waveletInputBuffer[i] = static_cast<int16_t>(3U * i + 1024U);
    }

    outpost::compression::LeGall53Wavelet::forwardTransformInPlace(waveletInputBuffer,
                                                                   bufferLength);
    unittest::compression::ReorderStub::reorder(waveletInputBuffer, bufferLength);
    int16_t* inBuffer = reinterpret_cast<int16_t*>(waveletInputBuffer);

    outpost::Bitstream bitstream(bitStreamData);

    transformer.forward(inBuffer, bufferLength, bitstream);
    EXPECT_EQ(bitstream.getSize(), 25U);
    outpost::Serialize s_stream(bitStreamData);
    bitstream.serialize(s_stream, 23U);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSize()], 0, 2 * bufferLength - bitstream.getSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize d_stream(bitStreamData);
    bitstream_out.deserialize(d_stream);

    size_t outBufferLength;
    transformer.backward(bitstream_out, outputBuffer, outBufferLength);

    ASSERT_EQ(outBufferLength, bufferLength);
    for (uint32_t i = 0; i < outBufferLength; i++)
    {
        EXPECT_LE(std::abs(outputBuffer[i]) - std::abs(inBuffer[i]), 1.0f);
    }

    double doubleInBuffer[bufferLength];
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        doubleInBuffer[i] = static_cast<double>(outputBuffer[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(
            doubleInBuffer, outBuffer, bufferLength);

    double mse = 0.0f;
    for (uint32_t i = 0; i < outBufferLength; i++)
    {
        mse += (std::abs(outBuffer[i]) - std::abs(inputReference[i]))
               * (std::abs(outBuffer[i]) - std::abs(inputReference[i]));
        EXPECT_LE(std::abs(outBuffer[i]) - std::abs(inputReference[i]), 2.0f);
    }
    mse /= bufferLength;
    EXPECT_LE(mse, 0.75f);
}
