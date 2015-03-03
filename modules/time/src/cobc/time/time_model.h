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
#ifndef COBC_TIME_TIME_MODEL_H
#define COBC_TIME_TIME_MODEL_H

#include "time_point.h"

namespace cobc
{
namespace time
{

/**
 * Time model used to convert between different time representations.
 *
 * \ingroup time
 * \author  Fabian Greif
 */
class TimeModel
{
public:
    static TimePoint
    endOfEpoch();

    static TimePoint
    startOfEpoch();

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

private:
    // disable unneeded implicitly member functions
    TimeModel();

    ~TimeModel();

    TimeModel(const TimeModel&);

    TimeModel&
    operator=(const TimeModel&);
};

}
}

#endif // COBC_TIME_TIME_MODEL_H
