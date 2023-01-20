/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 * - 2014, Benjamin Weps (DLR SC-SRV)
 * - 2015, Annika Ofenloch (DLR RY-AVS)
 */

#include "date.h"

namespace outpost
{
namespace time
{
static const int32_t secondsPerMinute = 60;
static const int32_t secondsPerHour = 60 * secondsPerMinute;  //  3600
static const int32_t secondsPerDay = 24 * secondsPerHour;     // 86400

static const int64_t secondsPerWeek = 7L * secondsPerDay;

// Days from the 0-03-01 to 1970-01-01.
// Value is equal to DateUtils::getDay(Date { 1970, 1, 1, 0, 0, 0 })
static const int32_t unixEpochStartDayCount = 719468;

// ----------------------------------------------------------------------------
/**
 * Get the number of days before the given year.
 */
static inline int64_t
getDaysBeforeYear(int32_t year)
{
    int64_t days = 365L * year;

    // accommodate for leap years
    days += ((year / 4) - (year / 100)) + (year / 400);

    return days;
}

/**
 * Get the number of days which have already passed in the current year
 * before the start of the current month.
 *
 * The year begins with the 1th of march. Numbering the months in this way has
 * the special advantage that leap days are always added at the end of the year,
 * and do not change the day offsets for the beginnings of the months.
 *
 * Using basic linear regression on the month indexes and the day offsets, we
 * can find functions to map each to the other. Given the month index, we can
 * calculate the day number for the first of the month from the integer part of
 * the function:
 *
 *   f(m) = (306*m + 5)/10:
 *
 *          m     f(m)  int32_t(f)
 *   Mar    0     0.5      0
 *   Apr    1    31.1     31
 *   May    2    61.7     61
 *   Jun    3    92.3     92
 *   Jul    4   122.9    122
 *   Aug    5   153.5    153
 *   Sep    6   184.1    184
 *   Oct    7   214.7    214
 *   Nov    8   245.3    245
 *   Dec    9   275.9    275
 *   Jan   10   306.5    306
 *   Feb   11   337.1    337
 */
static inline constexpr int32_t
getDaysBeforeMonth(int32_t month)
{
    return ((month * 306) + 5) / 10;
}

/**
 * Get the month from the number of days in the current year.
 *
 * Uses the inverse function of 'getDaysBeforeMonth()'.
 *
 *   m = f(d) = (100*d + 52)/3060
 *
 *   month  length  index  s=first e=last      f(s)     f(e)
 *                           day     day
 *   Mar      31      0       0       30      0.017     0.997
 *   Apr      30      1      31       60      1.030     1.978
 *   May      31      2      61       91      2.010     2.991
 *   Jun      30      3      92      121      3.024     3.971
 *   Jul      31      4     122      152      4.004     4.984
 *   Aug      31      5     153      183      5.017     5.997
 *   Sep      30      6     184      213      6.030     6.978
 *   Oct      31      7     214      244      7.010     7.991
 *   Nov      30      8     245      274      8.024     8.971
 *   Dec      31      9     275      305      9.004     9.984
 *   Jan      30     10     306      336     10.017    10.997
 *   Feb     28/29   11     337    365/366   11.030    11.945
 */
static inline constexpr int32_t
getMonthFromDayOfYear(int32_t day)
{
    return ((100 * day) + 52) / 3060;
}

/**
 * Calculate the number of days to a reference time point.
 *
 * Based on the following algorithm:
 * https://alcor.concordia.ca/~gpkatch/gdate-method.html
 *
 * Uses the 1th of March, Year 0 as reference. This simplifies the calculations
 * greatly. The algorithm heavily relies on integer division.
 */
int64_t
DateUtils::getDay(Date date)
{
    // March = 0, February = 11
    int32_t m = (date.month + 9) % 12;

    // If Jan or Feb subtract one
    int32_t y = date.year - (m / 10);

    int64_t days = getDaysBeforeYear(y) + getDaysBeforeMonth(m) + (date.day - 1);
    return days;
}

/**
 * Calculate a Gregorian date from a the day count.
 *
 * In the Gregorian calendar the length of a year is defined as
 * 365.2425 days (= 365 + 1/4 - 1/100 + 1/400).
 */
Date
DateUtils::getDate(int64_t day)
{
    // Guess the year from the day count
    int32_t y = (10000 * day + 14780) / 3652425;

    int32_t daysInYear = day - getDaysBeforeYear(y);
    if (daysInYear < 0)
    {
        // Correct the year if the guess was not correct
        y = y - 1;
        daysInYear = day - getDaysBeforeYear(y);
    }

    int32_t mi = getMonthFromDayOfYear(daysInYear);

    Date date;

    // Correct date from month=0 -> March to month=0 -> January
    date.year = y + ((mi + 2) / 12);
    date.month = ((mi + 2) % 12) + 1;
    date.day = (daysInYear - getDaysBeforeMonth(mi)) + 1;
    date.hour = 0;
    date.minute = 0;
    date.second = 0;

    return date;
}

// ----------------------------------------------------------------------------
/*
 * The Unix Time starts at 1970-01-01T00:00:00Z
 */
UnixTime
Date::toUnixTime(const Date& date)
{
    // Calculate the number of days from the beginning of the Unix epoch
    int64_t days = DateUtils::getDay(date) - unixEpochStartDayCount;

    int64_t seconds = date.second + (60 * (date.minute + (60 * (date.hour + (24 * days)))));

    return UnixTime::afterEpoch(Seconds(seconds));
}

Date
Date::fromUnixTime(UnixTime time)
{
    const int64_t secondsSinceEpoch = time.timeSinceEpoch().seconds();

    int64_t days = secondsSinceEpoch / secondsPerDay;
    int64_t seconds = secondsSinceEpoch - (days * secondsPerDay);

    // Calculate the date without the time
    Date date = DateUtils::getDate(days + unixEpochStartDayCount);

    date.hour = seconds / secondsPerHour;
    int32_t m = seconds % secondsPerHour;
    date.minute = m / secondsPerMinute;
    int32_t s = m % secondsPerMinute;
    date.second = s;

    return date;
}

static constexpr bool
isLeapYear(int32_t year)
{
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

static constexpr int32_t daysPerMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool
Date::isValid() const
{
    bool valid = true;

    if ((year < 1) || (month == 0) || (month > 12))
    {
        valid = false;
    }
    else if ((day == 0) || (isLeapYear(year) && (month == 2) && (day > 29))
             || (!isLeapYear(year) && (month != 2) && (day > daysPerMonth[month - 1])))
    {
        valid = false;
    }
    // Allow for leap second representation by setting seconds to 60
    else if ((hour > 23) || (minute > 59) || (second > 60))
    {
        valid = false;
    }
    else
    {
        valid = true;
    }

    return valid;
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
    int64_t seconds = 0;

    seconds += date.weekNumber * secondsPerWeek;
    seconds += date.timeOfWeek;

    return GpsTime::afterEpoch(Seconds(seconds));
}

}  // namespace time
}  // namespace outpost