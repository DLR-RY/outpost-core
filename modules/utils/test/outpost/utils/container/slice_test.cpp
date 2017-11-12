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

#include <array>

#include <unittest/harness.h>
#include <outpost/utils/container/slice.h>

using outpost::Slice;


TEST(SliceTest, createFromCStyleArray)
{
    uint8_t data[7];

    Slice<uint8_t> array(data);

    EXPECT_EQ(7U, array.getNumberOfElements());
}

TEST(SliceTest, dataAccessWithCStyleArray)
{
    uint8_t data[6] = { 6, 5, 4, 3, 2, 1 };

    Slice<uint8_t> array(data);

    EXPECT_EQ(6U, array.getNumberOfElements());
    EXPECT_EQ(6, array[0]);
    EXPECT_EQ(5, array[1]);
    EXPECT_EQ(2, array[4]);
}

TEST(SliceTest, createWithExplicitSize)
{
    uint8_t data[6];

    auto array = Slice<uint8_t>::unsafe(data, 4);

    EXPECT_EQ(4U, array.getNumberOfElements());
}

TEST(SliceTest, createArrayOfPointers)
{
    class TestClass
    {
    };

    TestClass testClass1;
    TestClass testClass2;
    TestClass testClass3;

    TestClass* data[3] = {
        &testClass1,
        &testClass2,
        &testClass3
    };

    Slice<TestClass*> array(data);

    EXPECT_EQ(3U, array.getNumberOfElements());

    EXPECT_EQ(&testClass1, array[0]);
    EXPECT_EQ(&testClass2, array[1]);
}

TEST(SliceTest, shouldBeConstructableFromStdArray)
{
    std::array<uint8_t, 10> array;
    auto slice = Slice<uint8_t>(array);
    EXPECT_EQ(array.size(), slice.getNumberOfElements());
}

TEST(SliceTest, shouldBeConstructableFromStdVector)
{
    std::vector<uint8_t> vector(5);
    auto slice = Slice<uint8_t>(vector);
    EXPECT_EQ(vector.size(), slice.getNumberOfElements());
}

TEST(SliceTest, shouldCreateSubSlicesFromBeginning)
{
    std::array<uint8_t, 10> array = {{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }};
    auto slice = outpost::asSlice(array);

    auto subslice = slice.first(3);

    ASSERT_EQ(3U, subslice.getNumberOfElements());

    EXPECT_EQ(0U, subslice[0]);
    EXPECT_EQ(1U, subslice[1]);
    EXPECT_EQ(2U, subslice[2]);
}

TEST(SliceTest, shouldCreateSubSlicesFromEnd)
{
    std::array<uint8_t, 10> array = {{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }};
    auto slice = outpost::asSlice(array);

    auto subslice = slice.last(3);

    ASSERT_EQ(3U, subslice.getNumberOfElements());

    EXPECT_EQ(7U, subslice[0]);
    EXPECT_EQ(8U, subslice[1]);
    EXPECT_EQ(9U, subslice[2]);
}

TEST(SliceTest, shouldCreateNestedSubSlices)
{
    std::array<uint8_t, 10> array = {{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }};
    auto slice = outpost::asSlice(array);

    auto subslice = slice.first(6).last(3);

    ASSERT_EQ(3U, subslice.getNumberOfElements());

    EXPECT_EQ(3U, subslice[0]);
    EXPECT_EQ(4U, subslice[1]);
    EXPECT_EQ(5U, subslice[2]);
}

TEST(SliceTest, shouldBeCompatibleWithGslSpan)
{
    std::array<uint8_t, 10> array;
    auto span = gsl::make_span(array);

    // Create from gsl::span
    auto slice = outpost::asSlice(span);
    EXPECT_EQ(array.size(), slice.getNumberOfElements());

    // Convert back to gsl::span
    auto span2 = slice.asSpan();
    EXPECT_EQ(array.size(), static_cast<std::size_t>(span2.size()));
}

TEST(SliceTest, shouldConvertToConstSlice)
{
    std::array<uint8_t, 10> array;
    outpost::Slice<uint8_t> slice(array);

    outpost::Slice<const uint8_t> constSlice(slice);

    EXPECT_EQ(array.size(), constSlice.getNumberOfElements());
}

TEST(SliceTest, shouldProvideIteratorInterface)
{
    std::array<uint8_t, 4> array = {{ 1, 4, 5, 7 }};
    outpost::Slice<uint8_t> slice(array);

    size_t index = 0;
    for (uint8_t data : slice)
    {
        EXPECT_EQ(array[index], data);
        index++;
    }
    EXPECT_EQ(array.size(), index);
}

TEST(SliceTest, shouldProvideConstIteratorInterface)
{
    std::array<uint8_t, 4> array = {{ 1, 4, 5, 7 }};
    const outpost::Slice<uint8_t> slice(array);

    size_t index = 0;
    for (uint8_t data : slice)
    {
        EXPECT_EQ(array[index], data);
        index++;
    }
    EXPECT_EQ(array.size(), index);
}

TEST(SliceTest, shouldProvideReverseIterator)
{
    std::array<uint8_t, 4> array = {{ 1, 4, 5, 7 }};
    outpost::Slice<uint8_t> slice(array);

    size_t index = 0;
    for (auto it = slice.rbegin(); it != slice.rend(); ++it)
    {
        EXPECT_EQ(array[array.size() - index - 1], *it);
        index++;
    }
    EXPECT_EQ(array.size(), index);
}

TEST(SliceTest, shouldCreateSliceFromIteratorPair)
{
    std::array<uint8_t, 4> array = {{ 1, 4, 5, 7 }};

    auto slice = outpost::Slice<uint8_t>(array.begin(), array.end());

    EXPECT_EQ(array.size(), slice.getNumberOfElements());
}
