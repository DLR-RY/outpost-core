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

    Slice<uint8_t> array(data, 4);

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
    EXPECT_EQ(10U, slice.getNumberOfElements());
}

TEST(SliceTest, shouldBeConstructableFromStdVector)
{
    std::vector<uint8_t> vector(5);
    auto slice = Slice<uint8_t>(vector);
    EXPECT_EQ(5U, slice.getNumberOfElements());
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
