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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unittest/harness.h>

#include <math.h>

#include <chrono>

using ::testing::_;
using ::testing::ElementsAreArray;
using ::testing::InvokeWithoutArgs;
using ::testing::Return;
using namespace outpost;

namespace transform_test
{
static constexpr size_t maxBufferLength = 8192;
static FP<16> inputBuffer[maxBufferLength];
static FP<16> intermediateBuffer[maxBufferLength];
static FP<16> outputBuffer[maxBufferLength];

static double doubleBuffer[maxBufferLength];
static double outBuffer[maxBufferLength];

static FP<16> inputBufferReference[maxBufferLength];
static FP<16> intermediateBufferReference[maxBufferLength];
static FP<16> outputBufferReference[maxBufferLength];

static outpost::Slice<FP<16>> inputData(inputBuffer);
static outpost::Slice<FP<16>> intermediateData(intermediateBuffer);
static outpost::Slice<FP<16>> outputData(outputBuffer);

static outpost::Slice<FP<16>> inputReference(inputBufferReference);
static outpost::Slice<FP<16>> intermediateReference(intermediateBufferReference);
static outpost::Slice<FP<16>> outputReference(outputBufferReference);

class TransformTest : public ::testing::Test
{
public:
    TransformTest()
    {
    }

    virtual ~TransformTest()
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
};

TEST_F(TransformTest, LeGallTestConstant16)
{
    const FP<16> c = int16_t(1U);
    constexpr size_t bufferLength = 16;
    for (size_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = c;
        inputReference[i] = c;
    }
    intermediateReference[0] = 1;
    intermediateReference[1] = 1;

    outpost::compression::LeGall53Wavelet::forwardTransform(
            inputData.begin(), intermediateData.begin(), bufferLength);

    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(int16_t(intermediateData[i]), int16_t(intermediateReference[i]));
    }

    for (uint16_t i = 0; i < bufferLength; i++)
    {
        doubleBuffer[i] = static_cast<double>(intermediateData[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(doubleBuffer, outBuffer, bufferLength);

    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(static_cast<int16_t>(outBuffer[i]), static_cast<int16_t>(inputReference[i]));
    }
}

TEST_F(TransformTest, LeGallTestConstant32)
{
    const FP<16> c = int16_t(1U);
    constexpr size_t bufferLength = 32;
    for (size_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = c;
        inputReference[i] = c;
    }

    outpost::compression::LeGall53Wavelet::forwardTransform(
            inputData.begin(), intermediateData.begin(), bufferLength);

    for (uint16_t i = 0; i < bufferLength; i++)
    {
        doubleBuffer[i] = static_cast<double>(intermediateData[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(doubleBuffer, outBuffer, bufferLength);

    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(int16_t(inputReference[i]), int16_t(outBuffer[i]));
    }
}

TEST_F(TransformTest, LeGallTestLinear16)
{
    constexpr size_t bufferLength = 16;
    for (size_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = int16_t(i * 3);
        inputReference[i] = int16_t(i * 3);
    }

    outpost::compression::LeGall53Wavelet::forwardTransform(
            inputData.begin(), intermediateData.begin(), bufferLength);

    for (uint16_t i = 0; i < bufferLength; i++)
    {
        doubleBuffer[i] = static_cast<double>(intermediateData[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(doubleBuffer, outBuffer, bufferLength);

    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(int16_t(inputReference[i]), int16_t(outBuffer[i]));
    }
}

TEST_F(TransformTest, LeGallTestLinear8)
{
    constexpr size_t bufferLength = 8;
    for (size_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = int16_t(i * 3);
        inputReference[i] = int16_t(i * 3);
    }

    outpost::compression::LeGall53Wavelet::forwardTransform(
            inputData.begin(), intermediateData.begin(), bufferLength);

    for (uint16_t i = 0; i < bufferLength; i++)
    {
        doubleBuffer[i] = static_cast<double>(intermediateData[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(doubleBuffer, outBuffer, bufferLength);

    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(int16_t(inputReference[i]), int16_t(outBuffer[i]));
    }
}

TEST_F(TransformTest, LeGallTestLinear32)
{
    constexpr size_t bufferLength = 32;
    for (size_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = int16_t(i * 3);
        inputReference[i] = int16_t(i * 3);
    }

    outpost::compression::LeGall53Wavelet::forwardTransform(
            inputData.begin(), intermediateData.begin(), bufferLength);

    for (uint16_t i = 0; i < bufferLength; i++)
    {
        doubleBuffer[i] = static_cast<double>(intermediateData[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(doubleBuffer, outBuffer, bufferLength);

    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(int16_t(inputReference[i]), int16_t(outBuffer[i]));
    }
}

TEST_F(TransformTest, LeGallTestRandom4096)
{
    constexpr size_t bufferLength = 4096;
    for (size_t i = 0; i < bufferLength; i++)
    {
        inputBuffer[i] = 1024 + rand() % 2048;
        inputReference[i] = inputBuffer[i];
    }

    outpost::compression::LeGall53Wavelet::forwardTransform(
            inputData.begin(), intermediateData.begin(), bufferLength);

    for (uint16_t i = 0; i < bufferLength; i++)
    {
        doubleBuffer[i] = static_cast<double>(intermediateData[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(doubleBuffer, outBuffer, bufferLength);

    for (size_t i = 0; i < bufferLength; i++)
    {
        EXPECT_EQ(static_cast<int16_t>(inputReference[i]),
                  static_cast<int16_t>(round(outBuffer[i])));
    }
}

TEST_F(TransformTest, InPlaceTest)
{
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        inputBuffer[i] = static_cast<int16_t>(0.15f * i + 3) % 1024 + 512;
        inputReference[i] = static_cast<int16_t>(0.15f * i + 3) % 1024 + 512;
    }

    outpost::compression::LeGall53Wavelet::forwardTransformInPlace(inputBuffer, maxBufferLength);

    outpost::compression::LeGall53Wavelet::reorder(inputBuffer, maxBufferLength);
    int16_t* reorderd_buffer = reinterpret_cast<int16_t*>(inputBuffer);

    double double_reordered_buffer[maxBufferLength];
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        double_reordered_buffer[i] = static_cast<double>(reorderd_buffer[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(
            double_reordered_buffer, outBuffer, maxBufferLength);
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        EXPECT_LT(outBuffer[i] - static_cast<double>(inputReference[i]), 1.75f);
    }

    double mse = 0.0f;
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        mse += (static_cast<double>(inputReference[i]) - outBuffer[i])
               * (static_cast<double>(inputReference[i]) - outBuffer[i]);
    }
    mse /= maxBufferLength;
    EXPECT_LT(mse, 7.0f);
}

TEST_F(TransformTest, InPlaceConstantTest)
{
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        inputBuffer[i] = 2027;
        inputReference[i] = 2027;
    }

    outpost::compression::LeGall53Wavelet::forwardTransformInPlace(inputBuffer, maxBufferLength);

    outpost::compression::LeGall53Wavelet::reorder(inputBuffer, maxBufferLength);
    int16_t* reorderd_buffer = reinterpret_cast<int16_t*>(inputBuffer);

    double double_reordered_buffer[maxBufferLength];
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        double_reordered_buffer[i] = static_cast<double>(reorderd_buffer[i]);
    }

    outpost::compression::LeGall53Wavelet::backwardTransform(
            double_reordered_buffer, outBuffer, maxBufferLength);
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        EXPECT_LT(outBuffer[i] - static_cast<double>(inputReference[i]), 1.75f);
    }

    double mse = 0.0f;
    for (size_t i = 0; i < maxBufferLength; i++)
    {
        mse += (static_cast<double>(inputReference[i]) - outBuffer[i])
               * (static_cast<double>(inputReference[i]) - outBuffer[i]);
    }
    mse /= maxBufferLength;
    EXPECT_LT(mse, 7.0f);
}

TEST_F(TransformTest, ReorderTest)
{
    for (uint16_t i = 0; i < 64; i++)
    {
        inputBuffer[i] = i;
    }

    outpost::compression::LeGall53Wavelet::forwardTransform(inputBuffer, intermediateBuffer, 64);

    for (uint16_t i = 0; i < 64; i++)
    {
        inputBuffer[i] = i;
    }
    outpost::compression::LeGall53Wavelet::forwardTransformInPlace(inputBuffer, 64);

    for (uint16_t i = 0; i < 64; i++)
    {
        outputBuffer[i] = inputBuffer[i];
    }

    outpost::compression::LeGall53Wavelet::reorder(outputBuffer, 64);
    int16_t* p = reinterpret_cast<int16_t*>(outputBuffer);

    for (uint16_t i = 0; i < 64; i++)
    {
        EXPECT_EQ(static_cast<int16_t>(intermediateBuffer[i]), p[i]);
    }
}

}  // namespace transform_test
