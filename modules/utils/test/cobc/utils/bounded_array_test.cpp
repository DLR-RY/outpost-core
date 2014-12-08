/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include <unittest/harness.h>
#include <cobc/utils/bounded_array.h>

using cobc::BoundedArray;

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
