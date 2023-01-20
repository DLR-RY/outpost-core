/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Felix Passenberg (DLR RY-AVS)
 */

#include <outpost/utils/coding/hdlc.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ElementsAreArray;

using outpost::utils::HdlcStuffing;

class HdlcTest : public ::testing::Test
{
public:
    HdlcTest() :
        input_mem{},
        input(input_mem),
        output_mem{},
        output(output_mem),
        decoded_mem{},
        decoded(decoded_mem)
    {
    }

    virtual void
    SetUp() override
    {
    }
    std::array<uint8_t, 128> input_mem;
    outpost::Slice<uint8_t> input;  //(input_mem);
    std::array<uint8_t, 128> output_mem;
    outpost::Slice<uint8_t> output;  //(output_mem);
    std::array<uint8_t, 128> decoded_mem;
    outpost::Slice<uint8_t> decoded;  //(output_mem);
};

// ----------------------------------------------------------------------------
TEST_F(HdlcTest, Empty)
{
    // empty frame is start marker followed by end marker
    const std::array<const uint8_t, 2> expected{{0x7E, 0x7E}};

    // encode
    size_t encodedLength = HdlcStuffing::encode(outpost::Slice<uint8_t>::empty(), output);

    EXPECT_EQ(expected.size(), encodedLength);
    EXPECT_EQ(output.getNumberOfElements(), expected.size());
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));

    // decode
    encodedLength = HdlcStuffing::decode(output, decoded);

    EXPECT_EQ(expected.size() - 1, encodedLength);
    EXPECT_EQ(decoded.getNumberOfElements(), 0u);
}

TEST_F(HdlcTest, encodingSingleOrdinaryByte)
{
    input[0] = {0x42};
    input = input.first(1);

    const std::array<const uint8_t, 3> expected = {{0x7E, 0x42, 0x7E}};

    // encode
    size_t encodedLength = HdlcStuffing::encode(input, output);

    EXPECT_EQ(expected.size(), encodedLength);
    EXPECT_EQ(output.getNumberOfElements(), expected.size());
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));

    // decode
    encodedLength = HdlcStuffing::decode(output, decoded);

    EXPECT_EQ(expected.size() - 1, encodedLength);
    EXPECT_EQ(decoded.getNumberOfElements(), input.getNumberOfElements());
    ASSERT_THAT(decoded, ElementsAreArray(input.begin(), input.end()));
}

TEST_F(HdlcTest, encodeEndMarker)
{
    input[0] = {0x7E};
    input = input.first(1);
    const std::array<const uint8_t, 4> expected = {{0x7E, 0x7D, 0x5E, 0x7E}};

    // encode
    size_t encodedLength = HdlcStuffing::encode(input, output);

    EXPECT_EQ(expected.size(), encodedLength);
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));

    // decode
    encodedLength = HdlcStuffing::decode(output, decoded);

    EXPECT_EQ(expected.size() - 1, encodedLength);
    EXPECT_EQ(decoded.getNumberOfElements(), input.getNumberOfElements());
    ASSERT_THAT(decoded, ElementsAreArray(input.begin(), input.end()));
}

TEST_F(HdlcTest, encodeEscapeMarker)
{
    input[0] = {0x7D};
    input = input.first(1);
    const std::array<const uint8_t, 4> expected = {{0x7E, 0x7D, 0x5D, 0x7E}};

    // encode
    size_t encodedLength = HdlcStuffing::encode(input, output);

    EXPECT_EQ(expected.size(), encodedLength);
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));

    // decode
    encodedLength = HdlcStuffing::decode(output, decoded);

    EXPECT_EQ(expected.size() - 1, encodedLength);
    EXPECT_EQ(decoded.getNumberOfElements(), input.getNumberOfElements());
    ASSERT_THAT(decoded, ElementsAreArray(input.begin(), input.end()));
}

TEST_F(HdlcTest, encodeMultipleMarkers)
{
    std::array<uint8_t, 8> values({{1, 2, 0x7E, 0x7E, 0x7D, 0x7D, 5, 6}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(8);

    const std::array<const uint8_t, 14> expected = {
            {0x7E, 1, 2, 0x7D, 0x5E, 0x7D, 0x5E, 0x7D, 0x5D, 0x7D, 0x5D, 5, 6, 0x7E}};

    // encode
    size_t encodedLength = HdlcStuffing::encode(input, output);

    EXPECT_EQ(expected.size(), encodedLength);
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));

    // decode
    encodedLength = HdlcStuffing::decode(output, decoded);

    EXPECT_EQ(expected.size() - 1, encodedLength);
    EXPECT_EQ(decoded.getNumberOfElements(), input.getNumberOfElements());
    ASSERT_THAT(decoded, ElementsAreArray(input.begin(), input.end()));
}

TEST_F(HdlcTest, encodeTooSmallOutput)
{
    std::array<uint8_t, 3> values({{1, 2, 3}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(3);

    // const std::array<const uint8_t, 5> expected = {0x7E, 1, 2, 3, 0x7E}; // not used, encode MUST
    // fail

    output = output.first(4);  // too small

    // encode
    size_t encodedLength = HdlcStuffing::encode(input, output);

    ASSERT_EQ(0u, encodedLength);

    // decode
    // nothing to decode after failure
}

TEST_F(HdlcTest, encodeToInput)
{
    std::array<uint8_t, 3> values({{1, 2, 3}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(3);

    // const std::array<const uint8_t, 5> expected = {0x7E, 1, 2, 3, 0x7E}; // not used, encode MUST
    // fail

    output = input;

    // encode
    size_t encodedLength = HdlcStuffing::encode(input, output);

    ASSERT_EQ(0u, encodedLength);

    // decode
    // nothing to decode after failure
}

TEST_F(HdlcTest, decodeLeadingGarbage)
{
    std::array<uint8_t, 8> values({{0x42, 0, 0x12, 0x7E, 1, 2, 3, 0x7E}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(8);

    const std::array<const uint8_t, 3> expected = {{1, 2, 3}};

    // decode
    size_t encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(input.getNumberOfElements() - 1, encodedLength);
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));

    // decode
    // nothing to decode after failure
}

TEST_F(HdlcTest, decodeTrailingGarbage)
{
    std::array<uint8_t, 8> values({{0x7E, 1, 2, 3, 0x7E, 42, 0x42, 0}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(8);

    const std::array<const uint8_t, 3> expected = {{1, 2, 3}};

    // decode
    size_t encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(4u, encodedLength);  // 4 is position of the second 0x7E
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));
}

TEST_F(HdlcTest, decodeTrailingAndLeadingGarbage)
{
    std::array<uint8_t, 11> values({{0, 9, 3, 0x7E, 1, 2, 3, 0x7E, 42, 0x42, 0}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(11);

    const std::array<const uint8_t, 3> expected = {{1, 2, 3}};

    // decode
    size_t encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(7u, encodedLength);  // 7 is position of the second 0x7E
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));
}

TEST_F(HdlcTest, decodeMultipleFrames)
{
    std::array<uint8_t, 16> values(
            {{0x7E, 1, 2, 3, 0x7E, 'e', 'a', 's', 't', 'e', 'r', 0x7E, 42, 0x42, 0, 0x7E}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(16);
    auto const output_orig = output;

    const std::array<const uint8_t, 3> expected1 = {{1, 2, 3}};
    const std::array<const uint8_t, 6> expected2 = {{'e', 'a', 's', 't', 'e', 'r'}};
    const std::array<const uint8_t, 3> expected3 = {{42, 0x42, 0}};

    // decode 1
    size_t encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(4u, encodedLength);  // 4 is position of the second 0x7E
    ASSERT_THAT(expected1, ElementsAreArray(output.begin(), output.end()));

    // strip decoded data
    input = input.skipFirst(encodedLength);
    ASSERT_EQ(input[0], 0x7E);
    ASSERT_EQ(input[1], 'e');  // checking truncation works
    output = output_orig;      // resetting output to full length

    // decode 2
    encodedLength = HdlcStuffing::decode(input, output);
    ASSERT_EQ(7u, encodedLength);  // 7 is position of the second 0x7E int the truncated input
    ASSERT_THAT(expected2, ElementsAreArray(output.begin(), output.end()));
    input = input.skipFirst(encodedLength);
    output = output_orig;  // resetting output to full length

    // decode 3
    encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(input.getNumberOfElements() - 1, encodedLength);
    ASSERT_THAT(expected3, ElementsAreArray(output.begin(), output.end()));
    input = input.skipFirst(encodedLength);

    ASSERT_EQ(input.getNumberOfElements(), 1u);
    ASSERT_EQ(input[0], 0x7E);
}

TEST_F(HdlcTest, decodeStreamToSmallerOutput)
{
    std::array<uint8_t, 16> values(
            {{0x7E, 1, 2, 3, 0x7E, 'e', 'a', 's', 't', 'e', 'r', 0x7E, 42, 0x42, 0, 0x7E}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(16);
    output = output.first(6);  // length of longest frame
    auto const output_orig = output;

    const std::array<const uint8_t, 3> expected1 = {{1, 2, 3}};
    const std::array<const uint8_t, 6> expected2 = {{'e', 'a', 's', 't', 'e', 'r'}};
    const std::array<const uint8_t, 3> expected3 = {{42, 0x42, 0}};

    // decode 1
    size_t encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(4u, encodedLength);  // 4 is position of the second 0x7E
    ASSERT_THAT(expected1, ElementsAreArray(output.begin(), output.end()));

    // strip decoded data
    input = input.skipFirst(encodedLength);
    ASSERT_EQ(input[0], 0x7E);
    ASSERT_EQ(input[1], 'e');  // checking truncation works
    output = output_orig;      // resetting output to full length

    // decode 2
    encodedLength = HdlcStuffing::decode(input, output);
    ASSERT_EQ(7u, encodedLength);  // 7 is position of the second 0x7E int the truncated input
    ASSERT_THAT(expected2, ElementsAreArray(output.begin(), output.end()));
    input = input.skipFirst(encodedLength);
    output = output_orig;  // resetting output to full length

    // decode 3
    encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(input.getNumberOfElements() - 1, encodedLength);
    ASSERT_THAT(expected3, ElementsAreArray(output.begin(), output.end()));
    input = input.skipFirst(encodedLength);

    ASSERT_EQ(input.getNumberOfElements(), 1u);
    ASSERT_EQ(input[0], 0x7E);
}
TEST_F(HdlcTest, decodeAbort)
{
    std::array<uint8_t, 16> values(
            {{0x7E, 1, 2, 3, 0x7D, 0x7E, 'a', 's', 't', 'e', 'r', 0x7E, 42, 0x42, 0, 0x7E}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(16);
    auto const output_orig = output;

    const std::array<const uint8_t, 5> expected2 = {{'a', 's', 't', 'e', 'r'}};
    const std::array<const uint8_t, 3> expected3 = {{42, 0x42, 0}};

    // decode 1
    size_t encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(5u, encodedLength);  // 4 is position of the second 0x7E
    ASSERT_THAT(outpost::Slice<uint8_t>::empty(), ElementsAreArray(output.begin(), output.end()));

    // strip decoded data
    input = input.skipFirst(encodedLength);
    ASSERT_EQ(input[0], 0x7E);
    ASSERT_EQ(input[1], 'a');  // checking truncation works
    output = output_orig;      // resetting output to full length

    // decode 2
    encodedLength = HdlcStuffing::decode(input, output);
    ASSERT_EQ(6u, encodedLength);  // 6 is position of the second 0x7E in the truncated input
    ASSERT_THAT(expected2, ElementsAreArray(output.begin(), output.end()));
    input = input.skipFirst(encodedLength);
    output = output_orig;  // resetting output to full length

    // decode 3
    encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(input.getNumberOfElements() - 1, encodedLength);
    ASSERT_THAT(expected3, ElementsAreArray(output.begin(), output.end()));
    input = input.skipFirst(encodedLength);

    ASSERT_EQ(input.getNumberOfElements(), 1u);
    ASSERT_EQ(input[0], 0x7E);
}

TEST_F(HdlcTest, decodeTwoEscapes)
{
    std::array<uint8_t, 14> values(
            {{0x7E, 'e', 'r', 'r', 0x7D, 0x7D, 4, 0x7E, 'v', 'a', 'l', 'i', 'd', 0x7E}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(14);
    auto const output_orig = output;

    const std::array<const uint8_t, 5> expected1 = {{'v', 'a', 'l', 'i', 'd'}};

    // decode 1
    size_t encodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(5u, encodedLength);  // 5 is position of the escaped escape which is invalid, thus can
                                   // be assumed to be garbage
    ASSERT_THAT(outpost::Slice<uint8_t>::empty(), ElementsAreArray(output.begin(), output.end()));

    // strip garbage data
    input = input.skipFirst(encodedLength);
    ASSERT_EQ(input[0], 0x7D);
    ASSERT_EQ(input[1], 4);  // checking truncation works
    output = output_orig;    // resetting output to full length

    // decode 2
    encodedLength = HdlcStuffing::decode(input, output);
    ASSERT_EQ(8u, encodedLength);
    ASSERT_THAT(expected1, ElementsAreArray(output.begin(), output.end()));
    input = input.skipFirst(encodedLength);
    output = output_orig;  // resetting output to full length
}

TEST_F(HdlcTest, decodeToInput)
{
    std::array<uint8_t, 5> values({{0x7E, 1, 2, 3, 0x7E}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(5);

    const std::array<const uint8_t, 3> expected = {{1, 2, 3}};

    output = input;

    // decode
    size_t decodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(4u, decodedLength);
    ASSERT_THAT(expected, ElementsAreArray(output.begin(), output.end()));
}

TEST_F(HdlcTest, decodeOneMarker)
{
    std::array<uint8_t, 5> values({{0, 1, 0x7E, 2, 3}});
    input = outpost::Slice<uint8_t>(values);
    input = input.first(5);

    // decode
    size_t decodedLength = HdlcStuffing::decode(input, output);

    ASSERT_EQ(2u, decodedLength);
    ASSERT_THAT(outpost::Slice<uint8_t>::empty(), ElementsAreArray(output.begin(), output.end()));

    input = input.skipFirst(decodedLength);
    ASSERT_EQ(input[0], 0x7E);
    ASSERT_EQ(input[1], 2);  // checking truncation works
}
