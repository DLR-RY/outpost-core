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
	SpacecraftElapsedTime time1 = SpacecraftElapsedTime::afterEpoch(Milliseconds(0));
	SpacecraftElapsedTime time2 = SpacecraftElapsedTime::afterEpoch(Milliseconds(100));

	Duration d = time2 - time1;

	EXPECT_EQ(100, d.milliseconds());
}

TEST_F(TimePointTest, shouldConvertEpoch)
{
	TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::setOffset(Microseconds(200));

	SpacecraftElapsedTime timeScet = SpacecraftElapsedTime::afterEpoch(Milliseconds(0));
	GpsTime timeGps = timeScet.convertTo<GpsTime>();

	EXPECT_EQ(200, timeGps.timeSinceEpoch().microseconds());
}
