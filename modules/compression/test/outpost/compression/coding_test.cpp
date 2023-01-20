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
#include <outpost/compression/legall_wavelet.h>
#include <outpost/compression/nls_encoder.h>
#include <outpost/utils/storage/bitstream.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;
using namespace outpost;

namespace coding_test
{
constexpr size_t bufferLength = 4096U;

outpost::FP<16> waveletInputBuffer[bufferLength];
outpost::Slice<FP<16>> waveletData(waveletInputBuffer);

int16_t inputBuffer[bufferLength];
int16_t inputReference[bufferLength];
uint8_t bitStreamBuffer[2 * bufferLength];
int16_t outputBuffer[bufferLength];
double outBuffer[bufferLength];
outpost::Slice<double> outSlice(outBuffer);

outpost::Slice<int16_t> inputReferenceData(inputReference);
outpost::Slice<int16_t> inputData(inputBuffer);
outpost::Slice<uint8_t> bitStreamData(bitStreamBuffer);
outpost::Slice<int16_t> outputData(outputBuffer);

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

    outpost::compression::NLSEncoder encoder;
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

    encoder.encode(inputData, bitstream);
    outpost::Serialize s_stream(bitStreamData);
    bitstream.serialize(s_stream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSize()], 0, 2 * bufferLength - bitstream.getSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    outpost::Slice<int16_t> res = encoder.decode(bitstream_out, outputData);

    ASSERT_EQ(res.getNumberOfElements(), bufferLength);
    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(res[i], inputReference[i]);
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

    encoder.encode(inputData, bitstream_in);
    outpost::Serialize s_stream(bitStreamData);
    bitstream_in.serialize(s_stream);

    EXPECT_LE(bitstream_in.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream_in.getSize()], 0, 2 * bufferLength - bitstream_in.getSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    outpost::Slice<int16_t> res = encoder.decode(bitstream_out, outputData);

    ASSERT_EQ(res.getNumberOfElements(), bufferLength);
    for (size_t i = 0; i < bufferLength; i++)
    {
        uint16_t dif = std::abs(res[i] - inputReference[i]);
        EXPECT_LE(dif, 16U);
    }
}

TEST_F(CodingTest, LinearTest)
{
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = static_cast<int16_t>(i * 2);
        inputReference[i] = static_cast<int16_t>(i * 2);
    }

    outpost::Bitstream bitstream(bitStreamData);

    encoder.encode(inputData, bitstream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSerializedSize()],
           0,
           2 * bufferLength - bitstream.getSerializedSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    outpost::Slice<int16_t> res = encoder.decode(bitstream_out, outputData);

    ASSERT_EQ(res.getNumberOfElements(), bufferLength);
    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_LE(std::abs(res[i] - inputReference[i]), 1);
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

    encoder.encode(inputData, bitstream_in);

    EXPECT_EQ(bitstream_in.getSerializedSize(), 7154U);
    outpost::Serialize s_stream(bitStreamData);
    bitstream_in.serialize(s_stream, 6000U);

    EXPECT_LE(bitstream_in.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream_in.getSerializedSize()],
           0,
           2 * bufferLength - bitstream_in.getSerializedSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    outpost::Slice<int16_t> res = encoder.decode(bitstream_out, outputData);

    ASSERT_EQ(res.getNumberOfElements(), bufferLength);
    double mse = 0.0f;
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        mse += std::abs(res[i] - inputReference[i]) * std::abs(res[i] - inputReference[i]);
        uint16_t dif = std::abs(res[i] - inputReference[i]);
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

    outpost::compression::LeGall53Wavelet::forwardTransformInPlace(waveletData);
    outpost::Slice<int16_t> inBuffer = outpost::compression::LeGall53Wavelet::reorder(waveletData);

    outpost::Bitstream bitstream(bitStreamData);

    encoder.encode(inBuffer, bitstream);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSerializedSize()],
           0,
           2 * bufferLength - bitstream.getSerializedSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize stream(bitStreamData);
    bitstream_out.deserialize(stream);

    outpost::Slice<int16_t> res = encoder.decode(bitstream_out, outputData);

    ASSERT_EQ(res.getNumberOfElements(), bufferLength);
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(std::abs(res[i]), std::abs(inBuffer[i]));
    }

    double doubleInBuffer[bufferLength];
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        doubleInBuffer[i] = static_cast<double>(res[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(outpost::Slice<double>(doubleInBuffer),
                                                             outSlice);

    double mse = 0.0f;
    for (uint32_t i = 0; i < bufferLength; i++)
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
        inputReference[i] = static_cast<int16_t>(3U * (i / 2) + 1024U);
        waveletInputBuffer[i] = static_cast<int16_t>(3U * (i / 2) + 1024U);
    }

    outpost::compression::LeGall53Wavelet::forwardTransformInPlace(waveletData);
    outpost::Slice<int16_t> inBuffer = outpost::compression::LeGall53Wavelet::reorder(waveletData);

    outpost::Bitstream bitstream(bitStreamData);

    encoder.encode(inBuffer, bitstream);
    EXPECT_EQ(bitstream.getSize(), 1711U);
    outpost::Serialize s_stream(bitStreamData);
    bitstream.serialize(s_stream, 1500U);

    EXPECT_LE(bitstream.getSize(), 2 * bufferLength);
    memset(&bitStreamBuffer[bitstream.getSize()], 0, 2 * bufferLength - bitstream.getSize());

    outpost::Bitstream bitstream_out(bitStreamData);
    outpost::Deserialize d_stream(bitStreamData);
    bitstream_out.deserialize(d_stream);

    outpost::Slice<int16_t> res = encoder.decode(bitstream_out, outputData);

    ASSERT_EQ(res.getNumberOfElements(), bufferLength);
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        EXPECT_LE(std::abs(std::abs(outputBuffer[i]) - std::abs(inBuffer[i])), 2);
    }

    double doubleInBuffer[bufferLength];
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        doubleInBuffer[i] = static_cast<double>(outputBuffer[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(outpost::Slice<double>(doubleInBuffer),
                                                             outSlice);

    double mse = 0.0f;
    for (uint32_t i = 0; i < bufferLength; i++)
    {
        mse += (std::abs(outBuffer[i]) - std::abs(inputReference[i]))
               * (std::abs(outBuffer[i]) - std::abs(inputReference[i]));
        EXPECT_LE(std::abs(std::abs(outBuffer[i]) - std::abs(inputReference[i])), 1.25f);
    }
    mse /= bufferLength;
    EXPECT_LE(mse, 0.4f);
}

}  // namespace coding_test
