/*
 * Copyright (c) 2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2022, Adrian Roeser (DLR RY-AVS)
 */

#include <outpost/utils/log2.h>

#include <unittest/harness.h>

#include <stdint.h>

using namespace outpost;

TEST(Log2, test)
{
    EXPECT_EQ(0U, outpost::Log2(0));
    EXPECT_EQ(0U, outpost::Log2(1));
    EXPECT_EQ(1U, outpost::Log2(2));
    EXPECT_EQ(4U, outpost::Log2(16));
    EXPECT_EQ(10U, outpost::Log2(1024));
    EXPECT_EQ(12U, outpost::Log2(4096));
    EXPECT_EQ(63U, outpost::Log2(std::numeric_limits<size_t>::max()));
}
