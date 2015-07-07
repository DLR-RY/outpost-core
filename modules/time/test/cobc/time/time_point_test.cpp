/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include <cobc/time/time_point.h>

using namespace cobc::time;

TEST(TimePointTest, shouldCreateFromDuration)
{
	SpacecraftElapsedTime time = SpacecraftElapsedTime::afterEpoch(Milliseconds(1456));

	EXPECT_EQ(Milliseconds(1456), time.timeSinceEpoch());
}

//  TimePoint - TimePoint --> Duration
TEST(TimePointTest, subtractionShouldGiveADuration)
{
	SpacecraftElapsedTime time1 = SpacecraftElapsedTime::afterEpoch(Milliseconds(100));
	SpacecraftElapsedTime time2 = SpacecraftElapsedTime::afterEpoch(Milliseconds(200));

	Duration d = time2 - time1;

	EXPECT_EQ(Milliseconds(100), d);
}

// TimePoint + Duration  --> TimePoint
TEST(TimePointTest, shouldAddDurationToTimePoint)
{
	SpacecraftElapsedTime time1 = SpacecraftElapsedTime::afterEpoch(Milliseconds(200));
	Duration d = Microseconds(12345);

	SpacecraftElapsedTime time2 = time1 + d;

	EXPECT_EQ(Microseconds(212345), time2.timeSinceEpoch());
}

// TimePoint - Duration  --> TimePoint
TEST(TimePointTest, shouldSubtractDurationFromTimePoint)
{
	SpacecraftElapsedTime time1 = SpacecraftElapsedTime::afterEpoch(Milliseconds(200));
	Duration d = Microseconds(12345);

	SpacecraftElapsedTime time2 = time1 - d;

	EXPECT_EQ(Microseconds(187655), time2.timeSinceEpoch());
}

TEST(TimePointTest, shouldConvertEpoch)
{
	TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::setOffset(Microseconds(200));

	SpacecraftElapsedTime timeScet = SpacecraftElapsedTime::afterEpoch(Milliseconds(0));
	GpsTime timeGps = timeScet.convertTo<GpsTime>();

	EXPECT_EQ(200, timeGps.timeSinceEpoch().microseconds());
}
