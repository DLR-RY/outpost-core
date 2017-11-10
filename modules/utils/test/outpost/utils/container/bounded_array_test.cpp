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

#include <unittest/harness.h>
#include <outpost/utils/container/bounded_array.h>

using outpost::BoundedArray;

class BoundedArrayTest : public testing::Test
{
};

TEST_F(BoundedArrayTest, createFromCStyleArray)
{
    uint8_t data[7];

    BoundedArray<uint8_t> array(data);

    EXPECT_EQ(7U, array.getNumberOfElements());
}

TEST_F(BoundedArrayTest, dataAccessWithCStyleArray)
{
    uint8_t data[6] = { 6, 5, 4, 3, 2, 1 };

    BoundedArray<uint8_t> array(data);

    EXPECT_EQ(6U, array.getNumberOfElements());
    EXPECT_EQ(6, array[0]);
    EXPECT_EQ(5, array[1]);
    EXPECT_EQ(2, array[4]);
}

TEST_F(BoundedArrayTest, createWithExplicitSize)
{
    uint8_t data[6];

    BoundedArray<uint8_t> array(data, 4);

    EXPECT_EQ(4U, array.getNumberOfElements());
}

TEST_F(BoundedArrayTest, createArrayOfPointers)
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

    BoundedArray<TestClass*> array(data);

    EXPECT_EQ(3U, array.getNumberOfElements());

    EXPECT_EQ(&testClass1, array[0]);
    EXPECT_EQ(&testClass2, array[1]);
}
