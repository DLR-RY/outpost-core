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

class TimePointTest : public testing::Test
{
public:

};


TEST_F(TimePointTest, subtractionShouldGiveADuration)
{
	SpacecraftElapsedTimePoint time1 = SpacecraftElapsedTimePoint::afterEpoch(Milliseconds(0));
	SpacecraftElapsedTimePoint time2 = SpacecraftElapsedTimePoint::afterEpoch(Milliseconds(100));

	Duration d = time2 - time1;

	EXPECT_EQ(100, d.milliseconds());
}

TEST_F(TimePointTest, shouldConvertEpoch)
{
	SpacecraftElapsedTimePoint::Epoch::offsetToGpsTime = Microseconds(200);

	SpacecraftElapsedTimePoint timeScet = SpacecraftElapsedTimePoint::afterEpoch(Milliseconds(0));
	GpsTimePoint timeGps = timeScet.convertTo<epoch::GpsTime>();

	EXPECT_EQ(200, timeGps.timeSinceEpoch().microseconds());
}
