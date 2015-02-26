/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#include <unittest/harness.h>
#include <cobc/time/time_model.h>

using namespace cobc::time;

TEST(TimeModelTest, convertUTCdataToGPSseconds)
{

    // random test
    uint32_t GPSseconds = TimeModel::convertUTCdataToGPSSeconds(1992, 1, 1, 12,
            12, 12);
    EXPECT_EQ(378303139, GPSseconds);
    // random test
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(2000, 6, 1, 23, 59, 58);
    EXPECT_EQ(643939211, GPSseconds);
    // random test
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(2000, 1, 1, 0, 0, 0);
    EXPECT_EQ(630720013, GPSseconds);

    // test the beginning of GPS seconds (06.01.1980)
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(1980, 1, 6, 0, 0, 0);
    EXPECT_EQ(0, GPSseconds);

    // check the leap year
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(1980, 2, 28, 23, 59, 59);
    EXPECT_EQ(4665599, GPSseconds);
    // ...
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(1980, 2, 29, 0, 0, 0);
    EXPECT_EQ(4665600, GPSseconds);
    // ...
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(1980, 3, 1, 0, 0, 0);
    EXPECT_EQ(4752000, GPSseconds);

    // check the leap second
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(1989, 12, 31, 23, 59,
            59);
    EXPECT_EQ(315187204, GPSseconds);
    // ...
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(1990, 1, 1, 0, 0, 0);
    EXPECT_EQ(315187206, GPSseconds);

    // check the leap second
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(2005, 12, 31, 23, 59,
            59);
    EXPECT_EQ(820108812, GPSseconds);
    // ...
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(2006, 1, 1, 0, 0, 0);
    EXPECT_EQ(820108814, GPSseconds);

    // check the leap second
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(2015, 6, 30, 23, 59, 59);
    EXPECT_EQ(1119744015, GPSseconds);
    // ...
    GPSseconds = TimeModel::convertUTCdataToGPSSeconds(2015, 7, 1, 0, 0, 0);
    EXPECT_EQ(1119744017, GPSseconds);

}

TEST(TimeModelTest, convertGPSsecondsToUTCdata)
{

    TimeModel::UTCdata utcData;

    // random test
    utcData = TimeModel::convertGPSsecondsToUTCdata(378303139);
    EXPECT_EQ(1992, static_cast<int>(utcData.years));
    EXPECT_EQ(1, static_cast<int>(utcData.months));
    EXPECT_EQ(1, static_cast<int>(utcData.days));
    EXPECT_EQ(12, static_cast<int>(utcData.hours));
    EXPECT_EQ(12, static_cast<int>(utcData.minutes));
    EXPECT_EQ(12, static_cast<int>(utcData.seconds));

    // test the beginning of GPS seconds (06.01.1980)
    utcData = TimeModel::convertGPSsecondsToUTCdata(0);
    EXPECT_EQ(1980, static_cast<int>(utcData.years));
    EXPECT_EQ(1, static_cast<int>(utcData.months));
    EXPECT_EQ(6, static_cast<int>(utcData.days));
    EXPECT_EQ(0, static_cast<int>(utcData.hours));
    EXPECT_EQ(0, static_cast<int>(utcData.minutes));
    EXPECT_EQ(0, static_cast<int>(utcData.seconds));

    // check the leap year
    utcData = TimeModel::convertGPSsecondsToUTCdata(4665599);
    EXPECT_EQ(1980, static_cast<int>(utcData.years));
    EXPECT_EQ(2, static_cast<int>(utcData.months));
    EXPECT_EQ(28, static_cast<int>(utcData.days));
    EXPECT_EQ(23, static_cast<int>(utcData.hours));
    EXPECT_EQ(59, static_cast<int>(utcData.minutes));
    EXPECT_EQ(59, static_cast<int>(utcData.seconds));
    // ...
    utcData = TimeModel::convertGPSsecondsToUTCdata(4665600);
    EXPECT_EQ(1980, static_cast<int>(utcData.years));
    EXPECT_EQ(2, static_cast<int>(utcData.months));
    EXPECT_EQ(29, static_cast<int>(utcData.days));
    EXPECT_EQ(0, static_cast<int>(utcData.hours));
    EXPECT_EQ(0, static_cast<int>(utcData.minutes));
    EXPECT_EQ(0, static_cast<int>(utcData.seconds));

    // check the leap second
    utcData = TimeModel::convertGPSsecondsToUTCdata(1119744015);
    EXPECT_EQ(2015, static_cast<int>(utcData.years));
    EXPECT_EQ(6, static_cast<int>(utcData.months));
    EXPECT_EQ(30, static_cast<int>(utcData.days));
    EXPECT_EQ(23, static_cast<int>(utcData.hours));
    EXPECT_EQ(59, static_cast<int>(utcData.minutes));
    EXPECT_EQ(59, static_cast<int>(utcData.seconds));
    // ...
    utcData = TimeModel::convertGPSsecondsToUTCdata(1119744017);
    EXPECT_EQ(2015, static_cast<int>(utcData.years));
    EXPECT_EQ(7, static_cast<int>(utcData.months));
    EXPECT_EQ(1, static_cast<int>(utcData.days));
    EXPECT_EQ(0, static_cast<int>(utcData.hours));
    EXPECT_EQ(0, static_cast<int>(utcData.minutes));
    EXPECT_EQ(0, static_cast<int>(utcData.seconds));

}

