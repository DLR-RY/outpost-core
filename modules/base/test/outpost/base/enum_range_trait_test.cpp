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
    VALUE_1 = 0,
    VALUE_2 = 1,
    VALUE_3 = 2,
    VALUE_4 = 3
};

template <>
struct outpost::EnumRangeTrait<TestEnum>
{
    static constexpr TestEnum
    min()
    {
        return TestEnum::VALUE_1;
    }

    static constexpr TestEnum
    max()
    {
        return TestEnum::VALUE_4;
    }

    static bool
    isValid(const TestEnum& t)
    {
        return t <= outpost::EnumRangeTrait<TestEnum>::max();
    }
};

TEST(EnumRangeTest, testMin)
{
    TestEnum e = TestEnum::VALUE_1;
    EXPECT_EQ(e, outpost::EnumRangeTrait<TestEnum>::min());
}

TEST(EnumRangeTest, testMax)
{
    TestEnum e = TestEnum::VALUE_4;
    EXPECT_EQ(e, outpost::EnumRangeTrait<TestEnum>::max());
}

TEST(EnumRangeTest, testValid)
{
    TestEnum e = TestEnum::VALUE_2;
    EXPECT_TRUE(outpost::EnumRangeTrait<TestEnum>::isValid(e));

    e = static_cast<TestEnum>(5U);
    EXPECT_FALSE(outpost::EnumRangeTrait<TestEnum>::isValid(e));
}
