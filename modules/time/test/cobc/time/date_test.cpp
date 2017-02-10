/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
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
#include <cobc/time/date.h>

using namespace cobc::time;

TEST(DateTest, shouldDetectValidDates)
{
	Date date;

	date = Date { 1980, 2, 28, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1980, 2, 29, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1980, 3, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1992, 1, 1, 12, 12, 12 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2000, 1, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2000, 6, 1, 23, 59, 58 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1989, 12, 31, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 1990, 1, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2005, 12, 31, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2006, 1, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2015, 6, 30, 23, 59, 59 };
	EXPECT_TRUE(date.isValid());

	date = Date { 2015, 7, 1, 0, 0, 0 };
	EXPECT_TRUE(date.isValid());
}

TEST(DateTest, convertDatesToUnixTime)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"
    // test the beginning of GPS seconds (06.01.1980)
    UnixTime time = Date::toUnixTime(Date { 1970, 1, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(0), time.timeSinceEpoch());
}

TEST(DateTest, convertDatesToUnixTimeForRandomDates)
{
	UnixTime time = Date::toUnixTime(Date { 1992, 1, 1, 12, 12, 12 });
    EXPECT_EQ(Seconds(694267932), time.timeSinceEpoch());

    time = Date::toUnixTime(Date { 2000, 1, 1, 0, 0, 0 });
	EXPECT_EQ(Seconds(946684800), time.timeSinceEpoch());

    time = Date::toUnixTime(Date { 2000, 6, 1, 23, 59, 58 });
    EXPECT_EQ(Seconds(959903998), time.timeSinceEpoch());
}

TEST(DateTest, convertDatesToUnixTimeForLeapYears)
{
    UnixTime time = Date::toUnixTime(Date { 1980, 2, 28, 23, 59, 59 });
    EXPECT_EQ(Seconds(320630399), time.timeSinceEpoch());

    time = Date::toUnixTime(Date { 1980, 2, 29, 0, 0, 0 });
    EXPECT_EQ(Seconds(320630400), time.timeSinceEpoch());

    time = Date::toUnixTime(Date { 1980, 3, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(320716800), time.timeSinceEpoch());
}

TEST(DateTest, convertDatesToUnixTimeDuringLeapSeconds)
{
    UnixTime time = Date::toUnixTime(Date { 1989, 12, 31, 23, 59, 59 });
    EXPECT_EQ(Seconds(631151999).seconds(), time.timeSinceEpoch().seconds());

    time = Date::toUnixTime(Date { 1990, 1, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(631152000), time.timeSinceEpoch());


    time = Date::toUnixTime(Date { 2005, 12, 31, 23, 59, 59 });
    EXPECT_EQ(Seconds(1136073599), time.timeSinceEpoch());

    time = Date::toUnixTime(Date { 2006, 1, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(1136073600), time.timeSinceEpoch());


    time = Date::toUnixTime(Date { 2015, 6, 30, 23, 59, 59 });
    EXPECT_EQ(Seconds(1435708799), time.timeSinceEpoch());

    time = Date::toUnixTime(Date { 2015, 7, 1, 0, 0, 0 });
    EXPECT_EQ(Seconds(1435708800), time.timeSinceEpoch());
}

TEST(DateTest, convertUnixTimeToDate)
{
    // Calculated reference values from "http://www.andrews.edu/~tzs/timeconv/timeconvert.php"
    // test the beginning of GPS seconds (06.01.1980)
    Date date = Date::fromUnixTime(UnixTime::afterEpoch(Seconds(315964800)));
    Date expect { 1980, 1, 6, 0, 0, 0 };
	EXPECT_EQ(expect, date);
}

TEST(DateTest, convertUnixTimeToDateForRandomTimes)
{
    Date date = Date::fromUnixTime(UnixTime::afterEpoch(Seconds(694267932)));
    Date expect { 1992, 1, 1, 12, 12, 12 };
    EXPECT_EQ(expect, date);
}

TEST(DateTest, convertUnixTimeToDateForLeapYears)
{
	Date date = Date::fromUnixTime(UnixTime::afterEpoch(Seconds(320630399)));
	Date expect { 1980, 2, 28, 23, 59, 59 };
	EXPECT_EQ(expect, date);

    date = Date::fromUnixTime(UnixTime::afterEpoch(Seconds(320630400)));
    expect = Date { 1980, 2, 29, 0, 0, 0 };
    EXPECT_EQ(expect, date);
}

TEST(DateTest, convertUnixTimeToDateForLeapSeconds)
{
    Date date = Date::fromUnixTime(UnixTime::afterEpoch(Seconds(1435708799)));
    Date expect { 2015, 6, 30, 23, 59, 59 };
	EXPECT_EQ(expect, date);

	date = Date::fromUnixTime(UnixTime::afterEpoch(Seconds(1435708800)));
	expect = Date { 2015, 7, 1, 0, 0, 0 };
	EXPECT_EQ(expect, date);
}

TEST(DateTest, shouldDoDateRoundtripForAllValidDates)
{
	Date expect { 100, 1, 1, 0, 0, 0 };
	for (int year = 100; year < 3000; ++year)
	{
		for (int month = 1; month <= 12; ++month)
		{
			expect.day = 1;
			while (expect.isValid())
			{
				UnixTime time = Date::toUnixTime(expect);
				Date actual = Date::fromUnixTime(time);

				EXPECT_EQ(expect, actual);
				++expect.day;
			}
		}
	}
}
