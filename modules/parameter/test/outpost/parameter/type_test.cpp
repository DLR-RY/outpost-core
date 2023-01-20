/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include <outpost/parameter/type.h>
#include <outpost/time.h>

#include <unittest/harness.h>

#include <stdint.h>

using namespace outpost::parameter;

TEST(TypeTest, comparing)
{
    EXPECT_EQ(Type::getType<int>(), Type::getType<int>());
    int i = 0;
    float f = 0.0f;
    // base comparison
    EXPECT_EQ(Type::getType(i), Type::getType(i));
    EXPECT_EQ(Type::getType<int>(), Type::getType<int>());
    EXPECT_NE(Type::getType<float>(), Type::getType<int>());
    EXPECT_NE(Type::getType(i), Type::getType(f));

    const int i2 = 10;
    int& i3 = i;
    const int& i4 = i2;
    // const ref does not matter
    EXPECT_EQ(Type::getType(i), Type::getType<int>());
    EXPECT_EQ(Type::getType(i2), Type::getType<int>());
    EXPECT_EQ(Type::getType(i3), Type::getType<int>());
    EXPECT_EQ(Type::getType(i4), Type::getType<int>());

    EXPECT_EQ(Type::getType(i), Type::getType<const int>());
    EXPECT_EQ(Type::getType(i2), Type::getType<const int>());
    EXPECT_EQ(Type::getType(i3), Type::getType<const int>());
    EXPECT_EQ(Type::getType(i4), Type::getType<const int>());

    EXPECT_EQ(Type::getType(i), Type::getType<int&>());
    EXPECT_EQ(Type::getType(i2), Type::getType<int&>());
    EXPECT_EQ(Type::getType(i3), Type::getType<int&>());
    EXPECT_EQ(Type::getType(i4), Type::getType<int&>());

    EXPECT_EQ(Type::getType(i), Type::getType<const int&>());
    EXPECT_EQ(Type::getType(i2), Type::getType<const int&>());
    EXPECT_EQ(Type::getType(i3), Type::getType<const int&>());
    EXPECT_EQ(Type::getType(i4), Type::getType<const int&>());
}
