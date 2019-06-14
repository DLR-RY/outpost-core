/*
 * Copyright (c) 2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#include <outpost/base/enum_range_trait.h>

#include <unittest/harness.h>

enum class TestEnum : uint8_t
{
    value1 = 1,
    value2 = 2,
    value3 = 3,
    value4 = 4
};

template <>
struct outpost::EnumRangeTrait<TestEnum>
{
    static constexpr TestEnum
    min()
    {
        return TestEnum::value1;
    }

    static constexpr TestEnum
    max()
    {
        return TestEnum::value4;
    }

    static constexpr bool
    isValid(const TestEnum& t)
    {
        return t <= outpost::EnumRangeTrait<TestEnum>::max()
               && t >= outpost::EnumRangeTrait<TestEnum>::min();
    }
};

TEST(EnumRangeTest, testMin)
{
    TestEnum e = TestEnum::value1;
    EXPECT_EQ(e, outpost::EnumRangeTrait<TestEnum>::min());
}

TEST(EnumRangeTest, testMax)
{
    TestEnum e = TestEnum::value4;
    EXPECT_EQ(e, outpost::EnumRangeTrait<TestEnum>::max());
}

TEST(EnumRangeTest, testValid)
{
    TestEnum e = TestEnum::value2;
    EXPECT_TRUE(outpost::EnumRangeTrait<TestEnum>::isValid(e));

    e = static_cast<TestEnum>(5U);
    EXPECT_FALSE(outpost::EnumRangeTrait<TestEnum>::isValid(e));
}
