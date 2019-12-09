/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/utils/coding/nand_bch_compiletime.h>
#include <outpost/utils/coding/nand_bch_interface.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include <array>

using namespace outpost::utils;

constexpr uint32_t dataSize = 512;
constexpr uint32_t spareSize = 16;

template <uint32_t mNandDataSize, uint32_t mNandSpareSize>
using BCH = NandBCHCTime<NandBCHInterface::DEF_GALIOS_DEMISIONS,
                         NandBCHInterface::DEF_ERROR_CORRECTION,
                         mNandDataSize,
                         mNandSpareSize>;

static BCH<dataSize, spareSize> bch;

class BCHCTest : public ::testing::Test
{
public:
    virtual void
    SetUp()
    {
    }
};

TEST(BCHCTest, defaultValueAreValid)
{
    EXPECT_TRUE(bch.isTemplateParameterValid());
}

RC_GTEST_FIXTURE_PROP(BCHCTest, simpleEncodeDecode, ())
{
    std::array<uint8_t, dataSize> input;
    std::array<uint8_t, dataSize> output;
    std::array<uint8_t, dataSize> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();

    for (size_t i = 0; i < input.size(); i++)
    {
        input[i] = *rand256;
    }

    std::array<uint8_t, dataSize + spareSize> encoded;

    expected = input;

    EXPECT_TRUE(bch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));
    EXPECT_TRUE(bch.decode(outpost::asSlice(encoded), outpost::asSlice(output)));

    EXPECT_EQ(output, expected);
}

RC_GTEST_FIXTURE_PROP(BCHCTest, simpleEncodeDecodeSeveralParts, ())
{
    constexpr uint8_t multi = 8;
    BCH<dataSize * multi, spareSize * multi> lbch;

    std::array<uint8_t, dataSize * multi> input;
    std::array<uint8_t, dataSize * multi> output;
    std::array<uint8_t, dataSize * multi> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();

    for (size_t i = 0; i < input.size(); i++)
    {
        input[i] = *rand256;
    }

    std::array<uint8_t, (dataSize + spareSize) * multi> encoded;

    expected = input;

    EXPECT_TRUE(lbch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));
    EXPECT_TRUE(lbch.decode(outpost::asSlice(encoded), outpost::asSlice(output)));

    EXPECT_EQ(output, expected);
}

RC_GTEST_FIXTURE_PROP(BCHCTest, simpleSeveralPartsPartialCopy, ())
{
    constexpr uint8_t multi = 8;
    constexpr uint32_t partial = dataSize * 6 + dataSize / 2;
    BCH<dataSize * multi, spareSize * multi> lbch;

    std::array<uint8_t, dataSize * multi> input;
    std::array<uint8_t, partial> output;
    std::array<uint8_t, dataSize * multi> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();

    for (size_t i = 0; i < input.size(); i++)
    {
        input[i] = *rand256;
    }

    std::array<uint8_t, (dataSize + spareSize) * multi> encoded;

    expected = input;

    EXPECT_TRUE(lbch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));
    EXPECT_TRUE(lbch.decode(outpost::asSlice(encoded), outpost::asSlice(output)));

    for (size_t i = 0; i < output.size(); i++)
    {
        EXPECT_EQ(output[i], expected[i]);
    }
}

RC_GTEST_FIXTURE_PROP(BCHCTest, failsTooSmallEncoded, ())
{
    std::array<uint8_t, dataSize> input;
    std::array<uint8_t, dataSize> output;
    std::array<uint8_t, dataSize> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();

    for (size_t i = 0; i < input.size(); i++)
    {
        input[i] = *rand256;
    }

    std::array<uint8_t, dataSize + spareSize> encoded;

    expected = input;

    EXPECT_FALSE(bch.encode(outpost::asSlice(input),
                            outpost::asSlice(encoded).first(dataSize + spareSize - 1)));

    // to get valid data
    EXPECT_TRUE(bch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));

    EXPECT_FALSE(bch.decode(outpost::asSlice(encoded).first(dataSize + spareSize - 1),
                            outpost::asSlice(output)));
}

RC_GTEST_FIXTURE_PROP(BCHCTest, partialCopy, ())
{
    std::array<uint8_t, dataSize> input;
    std::array<uint8_t, dataSize - 1> output;
    std::array<uint8_t, dataSize> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();

    for (size_t i = 0; i < input.size(); i++)
    {
        input[i] = *rand256;
    }

    std::array<uint8_t, dataSize + spareSize> encoded;

    expected = input;

    EXPECT_TRUE(bch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));
    EXPECT_TRUE(bch.decode(outpost::asSlice(encoded), outpost::asSlice(output)));

    for (size_t i = 0; i < output.size(); i++)
    {
        EXPECT_EQ(output[i], expected[i]);
    }
}

RC_GTEST_FIXTURE_PROP(BCHCTest, partialCopyKeepsRange, ())
{
    std::array<uint8_t, dataSize> input;
    std::array<uint8_t, dataSize> output;
    std::array<uint8_t, dataSize> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();

    for (size_t i = 0; i < input.size() - 1; i++)
    {
        input[i] = *rand256;
    }

    expected = input;

    // any two different values work
    input[input.size() - 1] = 0xff;
    output[input.size() - 1] = 0x00;
    expected[input.size() - 1] = output[input.size() - 1];

    std::array<uint8_t, dataSize + spareSize> encoded;

    EXPECT_TRUE(bch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));
    EXPECT_EQ(DecodeStatus::noError,
              bch.decode(outpost::asSlice(encoded),
                         outpost::asSlice(output).first(output.size() - 1)));

    EXPECT_EQ(output, expected);
}

RC_GTEST_FIXTURE_PROP(BCHCTest, correctBitFlip, ())
{
    std::array<uint8_t, dataSize> input;
    std::array<uint8_t, dataSize> output;
    std::array<uint8_t, dataSize> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();
    auto rand512 = rc::gen::inRange<uint32_t>(0, dataSize);
    auto rand8 = rc::gen::inRange<uint32_t>(0, 8);

    for (size_t i = 0; i < input.size(); i++)
    {
        input[i] = *rand256;
    }

    std::array<uint8_t, dataSize + spareSize> encoded;

    expected = input;

    EXPECT_TRUE(bch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));

    uint32_t byte = *rand512;
    uint32_t bit = *rand8;

    uint8_t value = encoded[byte];
    value = (value & (~(1u << bit))) | (~value & ((1u << bit)));
    encoded[byte] = value;

    EXPECT_TRUE(bch.decode(outpost::asSlice(encoded), outpost::asSlice(output)));

    EXPECT_EQ(output, expected);
}

RC_GTEST_FIXTURE_PROP(BCHCTest, singleBitflipsShouldNotMoveFromCorrectableToWronglyCorrected, ())
{
    std::array<uint8_t, dataSize> input;
    std::array<uint8_t, dataSize> output;
    std::array<uint8_t, dataSize> expected;

    auto rand256 = rc::gen::arbitrary<uint8_t>();
    auto rand512 = rc::gen::inRange<uint32_t>(0, dataSize);
    auto rand8 = rc::gen::inRange<uint32_t>(0, 8);

    for (size_t i = 0; i < input.size(); i++)
    {
        input[i] = *rand256;
    }

    std::array<uint8_t, dataSize + spareSize> encoded;

    expected = input;

    bool correct = true;
    EXPECT_TRUE(bch.encode(outpost::asSlice(input), outpost::asSlice(encoded)));
    uint32_t itcount = 0;
    while (correct)
    {
        uint32_t byte = *rand512;
        uint32_t bit = *rand8;

        uint8_t value = encoded[byte];
        value = (value & (~(1u << bit))) | (~value & ((1u << bit)));
        encoded[byte] = value;

        correct = bch.decode(outpost::asSlice(encoded), outpost::asSlice(output));

        if (correct)
        {
            EXPECT_EQ(output, expected);
        }
        itcount++;

        if (itcount > NandBCHInterface::DEF_ERROR_CORRECTION * 2)
        {
            break;  // just to be save from the very unlikely case of having randomness that end in
                    // infinite loop
        }
    }

    EXPECT_GT(itcount, NandBCHInterface::DEF_ERROR_CORRECTION);
}
