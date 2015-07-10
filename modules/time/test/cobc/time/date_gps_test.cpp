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

#include <iomanip>

#include <unittest/harness.h>
#include <cobc/time/date.h>

using namespace cobc::time;

namespace cobc
{
namespace time
{
::std::ostream&
operator<<(::std::ostream& os, const Duration& duration);

::std::ostream&
operator<<(::std::ostream& os, const Seconds& duration);

::std::ostream&
operator<<(::std::ostream& os, const Date& date);


::std::ostream&
operator<<(::std::ostream& os, const Duration& duration)
{
	return os << duration.microseconds() << " us";
}

::std::ostream&
operator<<(::std::ostream& os, const Seconds& duration)
{
	return os << duration.microseconds() << " us";
}

::std::ostream&
operator<<(::std::ostream& os, const Date& date)
{
	os << static_cast<int>(date.year) << "-";
	os << std::setfill('0') << std::setw(2) << static_cast<int>(date.month) << "-";
	os << std::setfill('0') << std::setw(2) << static_cast<int>(date.day) << "T";
	os << std::setfill('0') << std::setw(2) << static_cast<int>(date.hour) << "-";
	os << std::setfill('0') << std::setw(2) << static_cast<int>(date.minute) << "-";
	os << std::setfill('0') << std::setw(2) << static_cast<int>(date.second);

	return os;
}

}
}

TEST(DateGpsTest, convertDatesToGpsTime)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"
    // test the beginning of GPS seconds (06.01.1980)
    GpsTime time = Date::toGpsTime(Date { 1980, 1, 6, 0, 0, 0 });
    EXPECT_EQ(Seconds(0), time.timeSinceEpoch());
}

TEST(DateGpsTest, convertDatesToGpsTimeForRandomDates)
{
	GpsTime time = Date::toGpsTime(Date { 1992, 1, 1, 12, 12, 12 });
    EXPECT_EQ(Seconds(378303139), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 2000, 1, 1, 0, 0, 0 });
	EXPECT_EQ(Seconds(630720013), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 2000, 6, 1, 23, 59, 58 });
    EXPECT_EQ(Seconds(643939211), time.timeSinceEpoch());
}

TEST(DateGpsTest, convertDatesToGpsTimeForLeapYears)
{
    GpsTime time = Date::toGpsTime(Date { 1980, 2, 28, 23, 59, 59 });
    EXPECT_EQ(Seconds(4665599), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 1980, 2, 29, 0, 0, 0 });
    EXPECT_EQ(Seconds(4665600), time.timeSinceEpoch());

    time = Date::toGpsTime(Date { 1980, 3, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(4752000), time.timeSinceEpoch());
}

TEST(DateGpsTest, convertDatesToGpsTimeDuringLeapSeconds)
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

TEST(DateGpsTest, convertGpsTimeToDate)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"
    // test the beginning of GPS seconds (06.01.1980)
    Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(0)));
    Date expect { 1980, 1, 6, 0, 0, 0 };
    EXPECT_EQ(expect, date);
}

TEST(DateGpsTest, convertGpsTimeToDateForRandomTimes)
{
    Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(378303139)));
    Date expect { 1992, 1, 1, 12, 12, 12 };
    EXPECT_EQ(expect, date);
}

TEST(DateGpsTest, convertGpsTimeToDateForLeapYears)
{
	Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(4665599)));
	Date expect { 1980, 2, 28, 23, 59, 59 };
	EXPECT_EQ(expect, date);

    date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(4665600)));
    expect = Date { 1980, 2, 29, 0, 0, 0 };
    EXPECT_EQ(expect, date);
}

TEST(DateGpsTest, convertGpsTimeToDateForLeapSeconds)
{
    Date date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(1119744015)));
    Date expect { 2015, 6, 30, 23, 59, 59 };
    EXPECT_EQ(expect, date);

    date = Date::fromGpsTime(GpsTime::afterEpoch(Seconds(1119744017)));
    expect = Date { 2015, 7, 1, 0, 0, 0 };
    EXPECT_EQ(expect, date);
}

TEST(DateGpsTest, convertGpsTimeToGpsDate)
{
	GpsTime time;
	GpsDate date;

	time = Date::toGpsTime(Date { 1980, 1, 6, 0, 0, 0 });
	date = GpsDate::fromGpsTime(time);

	EXPECT_EQ(0U, date.weekNumber);
	EXPECT_EQ(0U, date.timeOfWeek);

	time = Date::toGpsTime(Date { 1980, 1, 7, 0, 0, 0 });
	date = GpsDate::fromGpsTime(time);

	EXPECT_EQ(0U, date.weekNumber);
	EXPECT_EQ(86400U, date.timeOfWeek);

	time = Date::toGpsTime(Date { 1980, 1, 14, 0, 0, 0 });
	date = GpsDate::fromGpsTime(time);

	EXPECT_EQ(1U, date.weekNumber);
	EXPECT_EQ(86400U, date.timeOfWeek);

	time = Date::toGpsTime(Date { 1982, 3, 28, 2, 0, 0 });
	date = GpsDate::fromGpsTime(time);

	EXPECT_EQ(116U, date.weekNumber);
	EXPECT_EQ(7201U, date.timeOfWeek);

	time = Date::toGpsTime(Date { 2011, 7, 7, 8, 51, 44 });
	date = GpsDate::fromGpsTime(time);

	// 15 leap seconds
	EXPECT_EQ(1643U, date.weekNumber);
	EXPECT_EQ(377504U + 15U, date.timeOfWeek);

	time = Date::toGpsTime(Date { 2015, 7, 7, 16, 41, 23 });
	date = GpsDate::fromGpsTime(time);

	// 17 leap seconds
	EXPECT_EQ(1852U, date.weekNumber);
	EXPECT_EQ(232883U + 17U, date.timeOfWeek);
}

TEST(DateGpsTest, shouldDoAGpsDateRoundtrip)
{
	GpsTime in;
	GpsTime out;
	GpsDate date;

	in = Date::toGpsTime(Date { 2011, 7, 7, 8, 51, 44 });
	date = GpsDate::fromGpsTime(in);
	out = GpsDate::toGpsTime(date);
	EXPECT_EQ(in, out);

	in = Date::toGpsTime(Date { 2015, 7, 7, 16, 41, 23 });
	date = GpsDate::fromGpsTime(in);
	out = GpsDate::toGpsTime(date);
	EXPECT_EQ(in, out);
}

TEST(DateGpsTest, shouldDoDateRoundtripForAllValidDates)
{
	Date expect { 100, 1, 1, 0, 0, 0 };
	for (int year = 100; year < 3000; ++year)
	{
		for (int month = 1; month <= 12; ++month)
		{
			expect.day = 1;
			while (expect.isValid())
			{
				GpsTime time = Date::toGpsTime(expect);
				Date actual = Date::fromGpsTime(time);

				EXPECT_EQ(expect, actual);
				++expect.day;
			}
		}
	}
}
