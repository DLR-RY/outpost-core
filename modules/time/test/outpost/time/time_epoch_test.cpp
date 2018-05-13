/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/time/date.h>
#include <outpost/time/time_epoch.h>

#include <unittest/harness.h>

using namespace outpost::time;

TEST(TimeEpochTest, convertGpsTimeToUnixTime)
{
    // Calculated reference values from
    // http://www.andrews.edu/~tzs/timeconv/timeconvert.php
    std::vector<std::vector<int64_t>> referencePoints{
            // Start of epoch
            {0, 315964800},

            // Test random dates
            {4665599, 320630399},
            {378303139, 694267932},
            {630720013, 946684800},
            {643939211, 959903998},

            // Test leap years
            {4665599, 320630399},
            {4665600, 320630400},
            {4752000, 320716800},

            // Test leap seconds
            {315187204, 631151999},
            {315187205, 631151999},
            {315187206, 631152000},

            {820108812, 1136073599},
            {820108813, 1136073599},
            {820108814, 1136073600},

            {1119744015, 1435708799},
            {1119744016, 1435708799},
            {1119744017, 1435708800},
    };

    for (auto&& reference : referencePoints)
    {
        GpsTime gpsTime = GpsTime::afterEpoch(Seconds(reference[0]));
        UnixTime unixTime = gpsTime.convertTo<UnixTime>();

        EXPECT_EQ(Seconds(reference[1]), unixTime.timeSinceEpoch());
    }
}

TEST(TimeEpochTest, convertUnixTimeToGpsTime)
{
    // Calculated reference values from
    // http://www.andrews.edu/~tzs/timeconv/timeconvert.php
    std::vector<std::vector<int64_t>> referencePoints{
            // Start of epoch
            {0, 315964800},

            // Test random dates
            {4665599, 320630399},
            {378303139, 694267932},
            {630720013, 946684800},
            {643939211, 959903998},

            // Test leap years
            {4665599, 320630399},
            {4665600, 320630400},
            {4752000, 320716800},

            // Test leap seconds
            {315187204, 631151999},
            {315187206, 631152000},

            {820108812, 1136073599},
            {820108814, 1136073600},

            {1119744015, 1435708799},
            {1119744017, 1435708800},
    };

    for (auto&& reference : referencePoints)
    {
        UnixTime unixTime = UnixTime::afterEpoch(Seconds(reference[1]));
        GpsTime gpsTime = unixTime.convertTo<GpsTime>();

        EXPECT_EQ(Seconds(reference[0]), gpsTime.timeSinceEpoch());
    }
}

TEST(TimeEpochTest, shouldConvertBetweenDatesAndTai)
{
    Date date = Date{2002, 02, 02, 12, 0, 0};
    UnixTime unixTime = date.toUnixTime(date);
    AtomicTime taiTime = unixTime.convertTo<AtomicTime>();

    EXPECT_EQ(1391342432, taiTime.timeSinceEpoch().seconds());
}

TEST(TestEpochTest, shouldSetOffsetBetweenScetAndGps)
{
    SpacecraftElapsedTime scet = SpacecraftElapsedTime::afterEpoch(Milliseconds(1234567));
    GpsTime gps = GpsTime::afterEpoch(Seconds(4752000));

    setOffsetBetweenScetAndGps(scet, gps);

    EXPECT_EQ(scet, gps.convertTo<SpacecraftElapsedTime>());
    EXPECT_EQ(gps, scet.convertTo<GpsTime>());
}

// See next leap second table in time_epoch.cpp
TEST(TestEpochTest, shouldCalculateNextLeapSecond)
{
    // 2016-12-31T23:59:60Z
    size_t timeCode = (DateUtils::getDay(Date{2016, 12, 31, 23, 59, 60})
                       - DateUtils::getDay(Date{1958, 1, 1, 0, 0, 0}) + 1)
                              * 86400
                      + 36;
    EXPECT_EQ(1861920036U, timeCode);
}
