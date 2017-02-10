/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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
#include <cobc/time/duration.h>

using namespace cobc::time;

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
