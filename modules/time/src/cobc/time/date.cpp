/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#include "date.h"

using namespace cobc::time;

static const int secondsPerDay = 24 * 60 * 60;  // 86400
static const int daysPerYear = 365;
static const int secondsPerYear = secondsPerDay * daysPerYear;

GpsTime
Date::toGpsTime(const Date& date)
{
    UnixTime unixTime = Date::toUnixTime(date);
    GpsTime gpsTime = unixTime.convertTo<GpsTime>();
    return gpsTime;
}

Date
Date::fromGpsTime(GpsTime time)
{
	UnixTime unixTime = time.convertTo<UnixTime>();
    return Date::fromUnixTime(unixTime);
}

// ----------------------------------------------------------------------------
static bool
isLeapYear(uint16_t year)
{
    bool leapYear = ((!(year % 4U)) && ((year % 100U) || (!(year % 400U))));
    return leapYear;
}

UnixTime
Date::toUnixTime(const Date& date)
{
    // days since the beginning of the year without the leap day
    static const uint16_t daysSinceTheBeginningOfYear[12] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };

    uint8_t leapYear = (((date.year - 1) - 1968) / 4)
            		 - (((date.year - 1) - 1900) / 100)
					 + (((date.year - 1) - 1600) / 400);

    // -6, because the GPS time began at 06.01.1980
    uint32_t daysSince1980 = static_cast<uint32_t>((date.year - 1970) * 365
            + leapYear + daysSinceTheBeginningOfYear[date.month - 1] + date.day - 1);

    if ((date.month > 2U) && isLeapYear(date.year))
    {
        // + leap day, if the year is a leap year
        daysSince1980 += 1;
    }

    int64_t seconds = static_cast<uint32_t>(date.second
            + 60 * (date.minute + 60 * (date.hour + 24 * daysSince1980)));

    return UnixTime::afterEpoch(Seconds(seconds));
}

Date
Date::fromUnixTime(UnixTime time)
{
	uint64_t seconds = time.timeSinceEpoch().seconds();

    uint16_t startYear = 1970;
    int32_t yearSec = 0;

    static const uint32_t daysSinceTheBeginningOfYear[2][13] = {
        // 365 days, non-leap
        { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
        // 366 days, leap
        { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
    };

    while (seconds - yearSec >= static_cast<uint32_t>(secondsPerYear))
    {
        if (isLeapYear(startYear))
        {
            if (seconds - yearSec < static_cast<uint32_t>(secondsPerYear + secondsPerDay))
            {
                break;
            }

            yearSec += secondsPerYear + secondsPerDay;  // leap year (366 days)
        }
        else
        {
            yearSec += secondsPerYear;  // normal year (365 days)
        }

        startYear++;
    }

    bool leapYear = isLeapYear(startYear);

    // FOR THE DAY AND MONTH
    uint16_t yearDay = 0;
    uint32_t secondsVal = seconds - yearSec;

    // Calculate the day of the year and the time
    yearDay = secondsVal / 86400;

    // Calculate the month
    uint8_t month;
    for (month = 1; (month < 13) &&
                    (yearDay >= daysSinceTheBeginningOfYear[leapYear][month]); ++month)
    {
    }
    uint8_t monthDay = 1 + yearDay - daysSinceTheBeginningOfYear[leapYear][month - 1];

    // FOR THE HOURS, MINUTES AND SECONDS
    secondsVal %= 86400;
    uint8_t dataHours = secondsVal / 3600;
    secondsVal %= 3600;
    uint8_t dataMinutes = secondsVal / 60;
    secondsVal %= 60;

    // Fill the UTCdata structure ...
    Date date;
    date.year = startYear;
    date.month = month;
    date.day = monthDay;
    date.hour = dataHours;
    date.minute = dataMinutes;
    date.second = secondsVal;

    return date;
}

bool
Date::operator==(const Date& other) const
{
	return ((year == other.year)
			&& (month == other.month)
			&& (day == other.day)
			&& (hour == other.hour)
			&& (minute == other.minute)
			&& (second == other.second));
}

bool
Date::operator!=(const Date& other) const
{
	return !(*this == other);
}

// ----------------------------------------------------------------------------
GpsDate
GpsDate::fromGpsTime(GpsTime time)
{
	GpsDate date;

	int64_t seconds = time.timeSinceEpoch().seconds();

	date.weekNumber = seconds / secondsPerWeek;
	date.timeOfWeek = seconds - (date.weekNumber * secondsPerWeek);

	return date;
}

GpsTime
GpsDate::toGpsTime(GpsDate date)
{
	return GpsTime::afterEpoch(Seconds(0));
}
