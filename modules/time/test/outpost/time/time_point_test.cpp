/*
 * Copyright (c) 2015-2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 * - 2022, Tobias Pfeffer (DLR RY-AVS)
 */

#include <outpost/time/time_point.h>

#include <unittest/harness.h>

using namespace outpost::time;

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

    SpacecraftElapsedTime timeScet = SpacecraftElapsedTime::startOfEpoch();
    GpsTime timeGps = timeScet.convertTo<GpsTime>();

    EXPECT_EQ(200, timeGps.timeSinceEpoch().microseconds());

    // Reset offset
    TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::setOffset(Duration::zero());
}

TEST(TimePointTest, shouldBeComparable)
{
    SpacecraftElapsedTime time1 = SpacecraftElapsedTime::afterEpoch(Milliseconds(100));
    SpacecraftElapsedTime time2 = SpacecraftElapsedTime::afterEpoch(Milliseconds(200));

    EXPECT_TRUE(time1 == time1);
    EXPECT_FALSE(time1 == time2);
    EXPECT_FALSE(time1 != time1);
    EXPECT_TRUE(time1 != time2);

    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time1 <= time2);
    EXPECT_FALSE(time1 > time2);
    EXPECT_FALSE(time1 >= time2);

    EXPECT_FALSE(time2 < time1);
    EXPECT_FALSE(time2 <= time1);
    EXPECT_TRUE(time2 > time1);
    EXPECT_TRUE(time2 >= time1);

    EXPECT_FALSE(time1 < time1);
    EXPECT_TRUE(time1 <= time1);
    EXPECT_FALSE(time1 > time1);
    EXPECT_TRUE(time1 >= time1);
}

TEST(TimePointTest, comparisonShouldNotUnderflowWithMin)
{
    SpacecraftElapsedTime time1 = SpacecraftElapsedTime::afterEpoch(-Duration::maximum());
    SpacecraftElapsedTime time2 = SpacecraftElapsedTime::afterEpoch(Milliseconds(200));

    EXPECT_TRUE(time1 == time1);
    EXPECT_FALSE(time1 == time2);
    EXPECT_FALSE(time1 != time1);
    EXPECT_TRUE(time1 != time2);

    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time1 <= time2);
    EXPECT_FALSE(time1 > time2);
    EXPECT_FALSE(time1 >= time2);

    EXPECT_FALSE(time2 < time1);
    EXPECT_FALSE(time2 <= time1);
    EXPECT_TRUE(time2 > time1);
    EXPECT_TRUE(time2 >= time1);

    EXPECT_FALSE(time1 < time1);
    EXPECT_TRUE(time1 <= time1);
    EXPECT_FALSE(time1 > time1);
    EXPECT_TRUE(time1 >= time1);
}

TEST(TimePointTest, comparisonShouldNotOverflowWithMax)
{
    SpacecraftElapsedTime time1 = SpacecraftElapsedTime::afterEpoch(Milliseconds(100));
    SpacecraftElapsedTime time2 = SpacecraftElapsedTime::afterEpoch(Duration::maximum());

    EXPECT_TRUE(time1 == time1);
    EXPECT_FALSE(time1 == time2);
    EXPECT_FALSE(time1 != time1);
    EXPECT_TRUE(time1 != time2);

    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time1 <= time2);
    EXPECT_FALSE(time1 > time2);
    EXPECT_FALSE(time1 >= time2);

    EXPECT_FALSE(time2 < time1);
    EXPECT_FALSE(time2 <= time1);
    EXPECT_TRUE(time2 > time1);
    EXPECT_TRUE(time2 >= time1);

    EXPECT_FALSE(time2 < time2);
    EXPECT_TRUE(time2 <= time2);
    EXPECT_FALSE(time2 > time2);
    EXPECT_TRUE(time2 >= time2);
}