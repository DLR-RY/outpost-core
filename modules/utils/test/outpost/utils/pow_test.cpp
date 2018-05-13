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
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/utils/pow.h>

#include <unittest/harness.h>

#include <stdint.h>

using namespace outpost;

TEST(PowerOfTwoTest, test)
{
    EXPECT_EQ(8, outpost::PowerOfTwo<3>::value);
    EXPECT_EQ(256 * 256 * 256, outpost::PowerOfTwo<24>::value);
}

TEST(PowTest, test)
{
    int32_t value;

    value = outpost::Pow<2, 3>::value;
    EXPECT_EQ(8, value);

    value = outpost::Pow<2, 0>::value;
    EXPECT_EQ(1, value);

    value = outpost::Pow<2, -1>::value;
    EXPECT_EQ(0, value);

    value = outpost::Pow<3, 4>::value;
    EXPECT_EQ(81, value);
}
