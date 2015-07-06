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
static const int daysPerLeapYear = 366;

static const int secondsPerYear = secondsPerDay * daysPerYear;
static const int secondsPerLeapYear = secondsPerDay * daysPerLeapYear;

static bool
isLeapYear(uint16_t year)
{
    bool leapYear = ((!(year % 4U)) && ((year % 100U) || (!(year % 400U))));
    return leapYear;
}

static uint8_t
calculateTheLeapSecondsForGpsAfter(uint32_t seconds)
{
    uint8_t leapSeconds = 0;
    if (seconds >= 1119744000)
    {
        leapSeconds = 17;
    }
    else if (seconds >= 1025136000)
    {
        leapSeconds = 16;
    }
    else if (seconds >= 914803200)
    {
        leapSeconds = 15;
    }
    else if (seconds >= 820108800)
    {
        leapSeconds = 14;
    }
    else if (seconds >= 599184000)
    {
        leapSeconds = 13;
    }
    else if (seconds >= 551750400)
    {
        leapSeconds = 12;
    }
    else if (seconds >= 504489600)
    {
        leapSeconds = 11;
    }
    else if (seconds >= 457056000)
    {
        leapSeconds = 10;
    }
    else if (seconds >= 425520000)
    {
        leapSeconds = 9;
    }
    else if (seconds >= 393984000)
    {
        leapSeconds = 8;
    }
    else if (seconds >= 346723200)
    {
        leapSeconds = 7;
    }
    else if (seconds >= 315187200)
    {
        leapSeconds = 6;
    }
    else if (seconds >= 252028800)
    {
        leapSeconds = 5;
    }
    else if (seconds >= 173059200)
    {
        leapSeconds = 4;
    }
    else if (seconds >= 109900800)
    {
        leapSeconds = 3;
    }
    else if (seconds >= 78364800)
    {
        leapSeconds = 2;
    }
    else if (seconds >= 46828800)
    {
        leapSeconds = 1;
    }
    else
    {
        leapSeconds = 0;
    }

    return leapSeconds;
}

static uint8_t
calculateTheLeapSecondsForGpsBefore(uint32_t seconds)
{
    uint8_t leapSeconds = 0;
    if (seconds >= 1119744017)
    {
        leapSeconds = 17;
    }
    else if (seconds >= 1025136016)
    {
        leapSeconds = 16;
    }
    else if (seconds >= 914803215)
    {
        leapSeconds = 15;
    }
    else if (seconds >= 820108814)
    {
        leapSeconds = 14;
    }
    else if (seconds >= 599184013)
    {
        leapSeconds = 13;
    }
    else if (seconds >= 551750412)
    {
        leapSeconds = 12;
    }
    else if (seconds >= 504489611)
    {
        leapSeconds = 11;
    }
    else if (seconds >= 457056010)
    {
        leapSeconds = 10;
    }
    else if (seconds >= 425520009)
    {
        leapSeconds = 9;
    }
    else if (seconds >= 393984008)
    {
        leapSeconds = 8;
    }
    else if (seconds >= 346723207)
    {
        leapSeconds = 7;
    }
    else if (seconds >= 315187206)
    {
        leapSeconds = 6;
    }
    else if (seconds >= 252028805)
    {
        leapSeconds = 5;
    }
    else if (seconds >= 173059204)
    {
        leapSeconds = 4;
    }
    else if (seconds >= 109900803)
    {
        leapSeconds = 3;
    }
    else if (seconds >= 78364802)
    {
        leapSeconds = 2;
    }
    else if (seconds >= 46828801)
    {
        leapSeconds = 1;
    }
    else
    {
        leapSeconds = 0;
    }

    return leapSeconds;
}

GpsTime
Date::toGpsTime(const Date& date)
{
    // days since the beginning of the year without the leap day
    static const uint16_t daysSinceTheBeginningOfYear[12] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
    };

    uint8_t leapYear = (((date.year - 1) - 1976) / 4)
            - (((date.year - 1) - 1900) / 100)
            + (((date.year - 1) - 1600) / 400);

    // -6, because the GPS time began at 06.01.1980
    uint32_t daysSince1980 = static_cast<uint32_t>((date.year - 1980) * 365
            + leapYear + daysSinceTheBeginningOfYear[date.month - 1] + date.day - 6);

    if ((date.month > 2U) && isLeapYear(date.year))
    {
        // + leap day, if the year is a leap year
        daysSince1980 += 1;
    }

    int64_t seconds = static_cast<uint32_t>(date.second
            + 60 * (date.minute + 60 * (date.hour + 24 * daysSince1980)));

    seconds += static_cast<int64_t>(calculateTheLeapSecondsForGpsAfter(seconds));  // leap seconds

    return GpsTime::afterEpoch(Seconds(seconds));
}

Date
Date::fromGpsTime(GpsTime time)
{
    // LEAP SECONDS
	uint64_t seconds = time.timeSinceEpoch().seconds();
    seconds -= calculateTheLeapSecondsForGpsBefore(seconds);

    // FOR THE YEAR
    uint16_t startYear = 1980;

    // because GPS seconds began at 06.01.1980 (5 days = 432000 sec.)
    int32_t yearSec = -5 * secondsPerDay;

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
            if (seconds - yearSec < static_cast<uint32_t>(secondsPerLeapYear))
            {
                break;
            }

            yearSec += secondsPerLeapYear;  // leap year (366 days)
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
