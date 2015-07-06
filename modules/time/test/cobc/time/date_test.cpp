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
#include <cobc/time/date.h>

using namespace cobc::time;

TEST(TimeModelTest, convertDatesToGpsTime)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"
    // test the beginning of GPS seconds (06.01.1980)
    GpsTime time = Date::toGpsTime(Date { 1980, 1, 6, 0, 0, 0 });
    EXPECT_EQ(Seconds(0), time.timeSinceEpoch());
}

TEST(TimeModelTest, convertDatesToGpsTimeForRandomDates)
{
	GpsTime time = Date::toGpsTime(Date { 1992, 1, 1, 12, 12, 12 });
    EXPECT_EQ(Seconds(378303139), time.timeSinceEpoch());
    // random test
    time = Date::toGpsTime(Date { 2000, 6, 1, 23, 59, 58 });
    EXPECT_EQ(Seconds(643939211), time.timeSinceEpoch());
    // random test
    time = Date::toGpsTime(Date { 2000, 1, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(630720013), time.timeSinceEpoch());
}

TEST(TimeModelTest, convertDatesToGpsTimeForLeapYears)
{
    GpsTime time = Date::toGpsTime(Date { 1980, 2, 28, 23, 59, 59 });
    EXPECT_EQ(Seconds(4665599), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 1980, 2, 29, 0, 0, 0 });
    EXPECT_EQ(Seconds(4665600), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 1980, 3, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(4752000), time.timeSinceEpoch());
}

TEST(TimeModelTest, convertDatesToGpsTimeDuringLeapSeconds)
{
	GpsTime time = Date::toGpsTime(Date { 1989, 12, 31, 23, 59, 59 });
	EXPECT_EQ(Seconds(315187204), time.timeSinceEpoch());

	time = Date::toGpsTime(Date { 1990, 1, 1, 0, 0, 0 });
	EXPECT_EQ(Seconds(315187206), time.timeSinceEpoch());


    time = Date::toGpsTime(Date { 2005, 12, 31, 23, 59, 59 });
    EXPECT_EQ(Seconds(820108812), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 2006, 1, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(820108814), time.timeSinceEpoch());


    time = Date::toGpsTime(Date { 2015, 6, 30, 23, 59, 59 });
    EXPECT_EQ(Seconds(1119744015), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 2015, 7, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(1119744017), time.timeSinceEpoch());
}

TEST(TimeModelTest, convertGpsTimeToDate)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"
    // test the beginning of GPS seconds (06.01.1980)
    Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(0U)));
    EXPECT_EQ(1980, date.year);
    EXPECT_EQ(1, date.month);
    EXPECT_EQ(6, date.day);
    EXPECT_EQ(0, date.hour);
    EXPECT_EQ(0, date.minute);
    EXPECT_EQ(0, date.second);
}

TEST(TimeModelTest, convertGpsTimeToDateForRandomTimes)
{
    Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(378303139U)));
    EXPECT_EQ(1992, date.year);
    EXPECT_EQ(1, date.month);
    EXPECT_EQ(1, date.day);
    EXPECT_EQ(12, date.hour);
    EXPECT_EQ(12, date.minute);
    EXPECT_EQ(12, date.second);
}

TEST(TimeModelTest, convertGpsTimeToDateForLeapYears)
{
	Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(4665599U)));
    EXPECT_EQ(1980, date.year);
    EXPECT_EQ(2, date.month);
    EXPECT_EQ(28, date.day);
    EXPECT_EQ(23, date.hour);
    EXPECT_EQ(59, date.minute);
    EXPECT_EQ(59, date.second);

    date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(4665600U)));
    EXPECT_EQ(1980, date.year);
    EXPECT_EQ(2, date.month);
    EXPECT_EQ(29, date.day);
    EXPECT_EQ(0, date.hour);
    EXPECT_EQ(0, date.minute);
    EXPECT_EQ(0, date.second);
}

TEST(TimeModelTest, convertGpsTimeToDateForLeapSeconds)
{
    Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(1119744015U)));
    EXPECT_EQ(2015, date.year);
    EXPECT_EQ(6, date.month);
    EXPECT_EQ(30, date.day);
    EXPECT_EQ(23, date.hour);
    EXPECT_EQ(59, date.minute);
    EXPECT_EQ(59, date.second);

    date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(1119744017U)));
    EXPECT_EQ(2015, date.year);
    EXPECT_EQ(7, date.month);
    EXPECT_EQ(1, date.day);
    EXPECT_EQ(0, date.hour);
    EXPECT_EQ(0, date.minute);
    EXPECT_EQ(0, date.second);
}

