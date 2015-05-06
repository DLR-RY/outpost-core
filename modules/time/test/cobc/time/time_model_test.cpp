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
#include <cobc/time/time_model.h>

using namespace cobc::time;

TEST(TimeModelTest, convertUTCdataToGPSseconds)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"

    // random test
    uint32_t gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(1992, 1, 1, 12, 12, 12);
    EXPECT_EQ(378303139U, gpsSeconds);
    // random test
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(2000, 6, 1, 23, 59, 58);
    EXPECT_EQ(643939211U, gpsSeconds);
    // random test
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(2000, 1, 1, 0, 0, 0);
    EXPECT_EQ(630720013U, gpsSeconds);

    // test the beginning of GPS seconds (06.01.1980)
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(1980, 1, 6, 0, 0, 0);
    EXPECT_EQ(0U, gpsSeconds);

    // check the leap year
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(1980, 2, 28, 23, 59, 59);
    EXPECT_EQ(4665599U, gpsSeconds);
    // ...
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(1980, 2, 29, 0, 0, 0);
    EXPECT_EQ(4665600U, gpsSeconds);
    // ...
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(1980, 3, 1, 0, 0, 0);
    EXPECT_EQ(4752000U, gpsSeconds);

    // check the leap second
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(1989, 12, 31, 23, 59, 59);
    EXPECT_EQ(315187204U, gpsSeconds);
    // ...
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(1990, 1, 1, 0, 0, 0);
    EXPECT_EQ(315187206U, gpsSeconds);

    // check the leap second
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(2005, 12, 31, 23, 59, 59);
    EXPECT_EQ(820108812U, gpsSeconds);
    // ...
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(2006, 1, 1, 0, 0, 0);
    EXPECT_EQ(820108814U, gpsSeconds);

    // check the leap second
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(2015, 6, 30, 23, 59, 59);
    EXPECT_EQ(1119744015U, gpsSeconds);
    // ...
    gpsSeconds = TimeModel::convertUtcDataToGpsSeconds(2015, 7, 1, 0, 0, 0);
    EXPECT_EQ(1119744017U, gpsSeconds);

}

TEST(TimeModelTest, convertGpsSecondsToUtcData)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"

    TimeModel::UtcData utcData;

    // random test
    utcData = TimeModel::convertGpsSecondsToUtcData(378303139U);
    EXPECT_EQ(1992, static_cast<int>(utcData.years));
    EXPECT_EQ(1, static_cast<int>(utcData.months));
    EXPECT_EQ(1, static_cast<int>(utcData.days));
    EXPECT_EQ(12, static_cast<int>(utcData.hours));
    EXPECT_EQ(12, static_cast<int>(utcData.minutes));
    EXPECT_EQ(12, static_cast<int>(utcData.seconds));

    // test the beginning of GPS seconds (06.01.1980)
    utcData = TimeModel::convertGpsSecondsToUtcData(0U);
    EXPECT_EQ(1980, static_cast<int>(utcData.years));
    EXPECT_EQ(1, static_cast<int>(utcData.months));
    EXPECT_EQ(6, static_cast<int>(utcData.days));
    EXPECT_EQ(0, static_cast<int>(utcData.hours));
    EXPECT_EQ(0, static_cast<int>(utcData.minutes));
    EXPECT_EQ(0, static_cast<int>(utcData.seconds));

    // check the leap year
    utcData = TimeModel::convertGpsSecondsToUtcData(4665599U);
    EXPECT_EQ(1980, static_cast<int>(utcData.years));
    EXPECT_EQ(2, static_cast<int>(utcData.months));
    EXPECT_EQ(28, static_cast<int>(utcData.days));
    EXPECT_EQ(23, static_cast<int>(utcData.hours));
    EXPECT_EQ(59, static_cast<int>(utcData.minutes));
    EXPECT_EQ(59, static_cast<int>(utcData.seconds));
    // ...
    utcData = TimeModel::convertGpsSecondsToUtcData(4665600U);
    EXPECT_EQ(1980, static_cast<int>(utcData.years));
    EXPECT_EQ(2, static_cast<int>(utcData.months));
    EXPECT_EQ(29, static_cast<int>(utcData.days));
    EXPECT_EQ(0, static_cast<int>(utcData.hours));
    EXPECT_EQ(0, static_cast<int>(utcData.minutes));
    EXPECT_EQ(0, static_cast<int>(utcData.seconds));

    // check the leap second
    utcData = TimeModel::convertGpsSecondsToUtcData(1119744015U);
    EXPECT_EQ(2015, static_cast<int>(utcData.years));
    EXPECT_EQ(6, static_cast<int>(utcData.months));
    EXPECT_EQ(30, static_cast<int>(utcData.days));
    EXPECT_EQ(23, static_cast<int>(utcData.hours));
    EXPECT_EQ(59, static_cast<int>(utcData.minutes));
    EXPECT_EQ(59, static_cast<int>(utcData.seconds));
    // ...
    utcData = TimeModel::convertGpsSecondsToUtcData(1119744017U);
    EXPECT_EQ(2015, static_cast<int>(utcData.years));
    EXPECT_EQ(7, static_cast<int>(utcData.months));
    EXPECT_EQ(1, static_cast<int>(utcData.days));
    EXPECT_EQ(0, static_cast<int>(utcData.hours));
    EXPECT_EQ(0, static_cast<int>(utcData.minutes));
    EXPECT_EQ(0, static_cast<int>(utcData.seconds));

}

