/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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
#include "time_model.h"

#include <cobc/utils/limits.h>

using cobc::time::TimeModel;

cobc::time::TimePoint
TimeModel::endOfEpoch()
{
    return TimePoint(cobc::Limits<TimePoint::Type>::max / 2 - 1);
}

cobc::time::TimePoint
TimeModel::startOfEpoch()
{
    return TimePoint(0);
}

bool
TimeModel::isLeapYear(uint16_t year)
{
    if (!(year % 4) && (year % 100 || !(year % 400)))
        return true;

    return false;
}

uint32_t
TimeModel::convertUTCdataToGPSSeconds(uint16_t year,
                                      uint8_t month,
                                      uint8_t day,
                                      uint8_t hour,
                                      uint8_t minute,
                                      uint8_t second)
{
    const uint16_t daysSinceTheBeginningOfYear[12] = /* days since the beginning of the year without the leap day */
    { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

    uint8_t leapYear = ((year - 1) - 1976) / 4 - ((year - 1) - 1900) / 100
            + ((year - 1) - 1600) / 400;

    uint32_t daysSince1980 = static_cast<uint32_t>((year - 1980) * 365
            + leapYear + daysSinceTheBeginningOfYear[month - 1] + day - 6);  // -6, because the GPS time began at 06.01.1980

    if ((month > 2) && isLeapYear(year))
    {
        daysSince1980 += 1; /* + leap day, if the year is a leap year */
    }

    uint32_t seconds = static_cast<uint32_t>(second
            + 60 * (minute + 60 * (hour + 24 * daysSince1980)));

    seconds += static_cast<uint32_t>(calculateTheLeapSecsForGPSafter(seconds));  // leap seconds

    return seconds;
}

TimeModel::UTCdata
TimeModel::convertGPSsecondsToUTCdata(uint32_t seconds)
{
    // Structure for time data
    UTCdata UTCdata;

    // LEAP SECONDS
    seconds -= calculateTheLeapSecsForGPSbefore(seconds);


    // FOR THE YEAR

    bool leapYear = false;
    uint16_t startYear = 1980;
    int32_t yearSec = -432000;  // because GPS seconds began at 06.01.1980 (5 days = 432000 sec.)

    static const uint32_t daysSinceJan1st[2][13] = { { 0, 31, 59, 90, 120, 151,
        181, 212, 243, 273, 304, 334, 365 },  // 365 days, non-leap
        { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }  // 366 days, leap
    };

    while (seconds - yearSec >= 31536000)
    {
        if (isLeapYear(startYear))
        {
            if (seconds - yearSec < 31622400)
                break;

            yearSec += 31622400;  // leap year (366 days)
        }
        else
        {
            yearSec += 31536000;  // normal year (365 days)
        }

        startYear++;
    }

    leapYear = isLeapYear(startYear);

    UTCdata.years = startYear;


    // FOR THE DAY AND MONTH

    uint16_t yearDay = 0;
    uint8_t dataHours = 0;
    uint8_t dataMinutes = 0;

    uint32_t secondsVal = ((seconds - yearSec));

    // Calculate the day of the year and the time
    yearDay = secondsVal / 86400;

    uint8_t monthDay = 0;
    uint8_t counterMonth = 0;
    // Calculate the month
    for (monthDay = counterMonth = 1; counterMonth < 13; counterMonth++)
    {
        if (yearDay < daysSinceJan1st[leapYear][counterMonth])
        {
            monthDay += yearDay - daysSinceJan1st[leapYear][counterMonth - 1];
            break;
        }
    }

    UTCdata.months = counterMonth;

    // FOR THE HOURS, MINUTES AND SECONDS
    secondsVal %= 86400;
    dataHours = secondsVal / 3600;
    secondsVal %= 3600;
    dataMinutes = secondsVal / 60;
    secondsVal %= 60;

    // Fill the UTCdata structure ...
    UTCdata.days = monthDay;
    UTCdata.hours = dataHours;
    UTCdata.minutes = dataMinutes;
    UTCdata.seconds = secondsVal;

    return UTCdata;
}

uint8_t
TimeModel::calculateTheLeapSecsForGPSafter(uint32_t seconds)
{
    if (seconds >= 1119744000)
    {
        return 17;
    }
    else if (seconds >= 1025136000)
    {
        return 16;
    }
    else if (seconds >= 914803200)
    {
        return 15;
    }
    else if (seconds >= 820108800)
    {
        return 14;
    }
    else if (seconds >= 599184000)
    {
        return 13;
    }
    else if (seconds >= 551750400)
    {
        return 12;
    }
    else if (seconds >= 504489600)
    {
        return 11;
    }
    else if (seconds >= 457056000)
    {
        return 10;
    }
    else if (seconds >= 425520000)
    {
        return 9;
    }
    else if (seconds >= 393984000)
    {
        return 8;
    }
    else if (seconds >= 346723200)
    {
        return 7;
    }
    else if (seconds >= 315187200)
    {
        return 6;
    }
    else if (seconds >= 252028800)
    {
        return 5;
    }
    else if (seconds >= 173059200)
    {
        return 4;
    }
    else if (seconds >= 109900800)
    {
        return 3;
    }
    else if (seconds >= 78364800)
    {
        return 2;
    }
    else if (seconds >= 46828800)
    {
        return 1;
    }

    return 0;
}


uint8_t
TimeModel::calculateTheLeapSecsForGPSbefore(uint32_t seconds)
{
    if (seconds >= 1119744017)
    {
        return 17;
    }
    else if (seconds >= 1025136016)
    {
        return 16;
    }
    else if (seconds >= 914803215)
    {
        return 15;
    }
    else if (seconds >= 820108814)
    {
        return 14;
    }
    else if (seconds >= 599184013)
    {
        return 13;
    }
    else if (seconds >= 5517504012)
    {
        return 12;
    }
    else if (seconds >= 504489611)
    {
        return 11;
    }
    else if (seconds >= 457056010)
    {
        return 10;
    }
    else if (seconds >= 425520009)
    {
        return 9;
    }
    else if (seconds >= 393984008)
    {
        return 8;
    }
    else if (seconds >= 346723207)
    {
        return 7;
    }
    else if (seconds >= 315187206)
    {
        return 6;
    }
    else if (seconds >= 252028805)
    {
        return 5;
    }
    else if (seconds >= 173059204)
    {
        return 4;
    }
    else if (seconds >= 109900803)
    {
        return 3;
    }
    else if (seconds >= 78364802)
    {
        return 2;
    }
    else if (seconds >= 46828801)
    {
        return 1;
    }

    return 0;
}
