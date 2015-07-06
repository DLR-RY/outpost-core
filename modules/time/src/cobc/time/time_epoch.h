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

#ifndef COBC_TIME_TIME_EPOCH_H
#define COBC_TIME_TIME_EPOCH_H

#include "time_point.h"

namespace cobc
{
namespace time
{
/**
 * Time since 1980-01-06T00:00:00Z, no leap seconds
 */
class GpsEpoch
{
};

/**
 * Time since 1970-01-01T00:00:00Z, no leap seconds
 */
class UnixEpoch
{
};

/**
 * Time since 1999-08-22T00:00:00Z with leap seconds
 */
class GalileoSystemTimeEpoch
{
};

/**
 * Time since 2000-01-01T11:58:55Z
 */
class J2000Epoch
{
};

/**
 * Time since start of OBC.
 */
class SpacecraftElapsedTimeEpoch
{
};


typedef TimePoint<SpacecraftElapsedTimeEpoch> SpacecraftElapsedTime;
typedef TimePoint<GpsEpoch> GpsTime;

// forward declaration
template <typename Epoch>
class TimePoint;

template <typename From, typename To>
class TimeEpochConverter
{
public:
	static TimePoint<To>
	convert(TimePoint<From> from);
};

/**
 * Time model used to convert between different time representations.
 *
 * \ingroup time
 * \author  Fabian Greif
 */
class TimeModel
{
public:
    struct UtcData
    {
        uint16_t years;
        uint8_t months;
        uint8_t weekDay;
        uint8_t days;
        uint8_t hours;
        uint8_t minutes;
        uint8_t seconds;
    };

    static uint32_t
    convertUtcDataToGpsSeconds(uint16_t year,
                               uint8_t month,
                               uint8_t day,
                               uint8_t hour,
                               uint8_t minute,
                               uint8_t second);

    static UtcData
    convertGpsSecondsToUtcData(uint32_t seconds);

    static bool
    isLeapYear(uint16_t year);

    static uint8_t
    calculateTheLeapSecondsForGpsAfter(uint32_t seconds);

    static uint8_t
    calculateTheLeapSecondsForGpsBefore(uint32_t seconds);

    template <typename To, typename From>
    static TimePoint<To>
    convertTo(TimePoint<From> from);

private:
    static const int secondsPerDay = 24 * 60 * 60;  // 86400

    static const int daysPerYear = 365;
    static const int daysPerLeapYear = 366;

    static const int secondsPerYear = secondsPerDay * daysPerYear;
    static const int secondsPerLeapYear = secondsPerDay * daysPerLeapYear;

    // disable unneeded implicitly member functions
    TimeModel();

    ~TimeModel();

    TimeModel(const TimeModel&);

    TimeModel&
    operator=(const TimeModel&);
};

}
}

#include "time_epoch_impl.h"

#endif // COBC_TIME_TIME_EPOCH_H
