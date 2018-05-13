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
 * - 2015, Annika Ofenloch (DLR RY-AVS)
 */

#include <outpost/time/duration.h>

#include <unittest/harness.h>

using namespace outpost::time;

TEST(DurationTest, shouldAddTwoDurations)
{
    Duration d1 = Milliseconds(100);
    Duration d2 = Microseconds(23456);

    Duration d3 = d1 + d2;

    EXPECT_EQ(Microseconds(123456), d3);

    EXPECT_EQ(Milliseconds(100), d1);
    EXPECT_EQ(Microseconds(23456), d2);
}

TEST(DurationTest, derivedTypes)
{
    ASSERT_EQ(Seconds(5), Milliseconds(5000));
    ASSERT_EQ(Microseconds(1000), Milliseconds(1));

    Duration duration = Seconds(5);

    ASSERT_EQ(5, duration.seconds());
    ASSERT_EQ(5000, duration.milliseconds());
    ASSERT_EQ(5000000, duration.microseconds());
}

TEST(DurationTest, shouldConvertToMinutes)
{
    Duration duration = Seconds(3600);

    ASSERT_EQ(60, duration.minutes());
}

TEST(DurationTest, shouldConvertToHours)
{
    Duration duration = Seconds(3600);

    ASSERT_EQ(1, duration.hours());
}

TEST(DurationTest, convertBigNumbersMicrosecondsToDuration)
{
    Duration out = Microseconds(3147483643);

    EXPECT_EQ(3147483643, out.microseconds());
}
