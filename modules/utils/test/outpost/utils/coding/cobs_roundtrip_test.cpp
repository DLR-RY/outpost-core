/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include <outpost/utils/coding/cobs.h>

#include <string.h>     // for memset
#include <unittest/harness.h>

#include <rapidcheck/gtest.h>

using outpost::utils::Cobs;
using ::testing::ElementsAreArray;

class CobsRoundtriptTest : public ::testing::Test
{
public:
    virtual void
    SetUp()
    {
        memset(encoded, 0xAB, sizeof(encoded));
        memset(actual,  0xAB, sizeof(actual));
    }

    size_t
    roundtrip(outpost::Slice<uint8_t> input)
    {
        size_t encodedLength = Cobs::encode(input, outpost::asSlice(encoded));
        size_t decodedLength = Cobs::decode(outpost::asSlice(encoded).first(encodedLength), actual);

        return decodedLength;
    }

    uint8_t encoded[1024];
    uint8_t actual[1024];
};

// ----------------------------------------------------------------------------
TEST_F(CobsRoundtriptTest, singleZeroByte)
{
    uint8_t input[1] = { 0 };

    uint8_t expected[3] = { 0, 0xAB, 0xAB };

    size_t decodedLength = roundtrip(outpost::asSlice(input));

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(expected, ElementsAreArray(actual, 3));
}

TEST_F(CobsRoundtriptTest, zeroPrefixAndSuffix)
{
    uint8_t input[3] = { 0, 1, 0 };

    uint8_t expected[5] = { 0, 1, 0, 0xAB, 0xAB };

    size_t decodedLength = roundtrip(outpost::asSlice(input));

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(expected, ElementsAreArray(actual, 5));
}

TEST_F(CobsRoundtriptTest, blockOfDataWithoutZero)
{
    uint8_t input[512];
    for (size_t i = 0; i < 512; ++i)
    {
        input[i] = (i % 255) + 1;
    }

    size_t decodedLength = roundtrip(outpost::asSlice(input));

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(input, ElementsAreArray(actual, sizeof(input)));
}

// ----------------------------------------------------------------------------
/*
 * Decoded the COBS data in place by using the same array for input and
 * output of the encoder.
 */
TEST_F(CobsRoundtriptTest, inPlaceDecodingOfBlockOfDataWithoutZero)
{
    uint8_t input[512];
    for (size_t i = 0; i < 512; ++i)
    {
        input[i] = (i % 255) + 1;
    }

    size_t encodedLength = Cobs::encode(outpost::asSlice(input), outpost::asSlice(encoded));
    size_t decodedLength = Cobs::decode(outpost::asSlice(encoded).first(encodedLength), encoded);

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(input, ElementsAreArray(encoded, sizeof(input)));
}

TEST_F(CobsRoundtriptTest, inPlaceDecodingOfZeroPrefixAndSuffix)
{
    uint8_t input[3] = { 0, 1, 0 };

    size_t encodedLength = Cobs::encode(outpost::asSlice(input), outpost::asSlice(encoded));
    size_t decodedLength = Cobs::decode(outpost::asSlice(encoded).first(encodedLength), encoded);

    EXPECT_EQ(sizeof(input), decodedLength);
    EXPECT_THAT(input, ElementsAreArray(encoded, sizeof(input)));
}

RC_GTEST_FIXTURE_PROP(CobsRoundtriptTest, shouldPerformRoundTripWithRandomData, ())
{
	const auto input = *rc::gen::resize(200, rc::gen::arbitrary<std::vector<uint8_t>>());

	outpost::Slice<const uint8_t> inputArray(input);
	size_t encodedLength = outpost::utils::CobsBase<32>::encode(inputArray, outpost::asSlice(encoded));
	size_t decodedLength = outpost::utils::CobsBase<32>::decode(outpost::asSlice(encoded).first(encodedLength), encoded);

	RC_ASSERT(input.size() == decodedLength);
	RC_ASSERT(input == std::vector<uint8_t>(encoded, &encoded[decodedLength]));
}
