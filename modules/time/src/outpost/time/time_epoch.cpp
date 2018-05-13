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

#include "time_epoch.h"

#include <outpost/utils/container/slice.h>

using namespace outpost::time;

Duration TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::offsetToGpsTime =
        Duration::zero();

// ----------------------------------------------------------------------------
void
outpost::time::setOffsetBetweenScetAndGps(SpacecraftElapsedTime scet, GpsTime gps)
{
    TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::setOffset(scet, gps);
}

// ----------------------------------------------------------------------------
/**
 * Leap second correction table.
 *
 * When new leap seconds are added they have to be added here. The values are
 * calculated as followed:
 *
 *     (DateUtils::getDay(Date { 1981, 6, 30, 0, 0, 0 })
 *    - DateUtils::getDay(Date { 1958, 1,  1, 0, 0, 0 }) + 1) * 86400
 *    + number of leap seconds
 *
 * See also http://hpiers.obspm.fr/eop-pc/earthor/utc/TAI-UTC_tab.html
 */
static const int64_t leapSecondArray[] = {
        1861920036,  // 36: 2016-12-31T23:59:60Z
        1814400035,  // 35: 2015-06-30T23:59:60Z
        1719792034,  // 34: 2012-06-30T23:59:60Z
        1609459233,  // 33: 2008-12-31T23:59:60Z
        1514764832,  // 32: 2005-12-31T23:59:60Z
        1293840031,  // 31: 1998-12-31T23:59:60Z
        1246406430,  // 30: 1997-06-30T23:59:60Z
        1199145629,  // 29: 1995-12-31T23:59:60Z
        1151712028,  // 28: 1994-06-30T23:59:60Z
        1120176027,  // 27: 1993-06-30T23:59:60Z
        1088640026,  // 26: 1992-06-30T23:59:60Z
        1041379225,  // 25: 1990-12-31T23:59:60Z
        1009843224,  // 24: 1989-12-31T23:59:60Z
        946684823,   // 23: 1987-12-31T23:59:60Z
        867715222,   // 22: 1985-06-30T23:59:60Z
        804556821,   // 21: 1983-06-30T23:59:60Z
        773020820,   // 20: 1982-06-30T23:59:60Z
        741484819,   // 19: 1981-06-30T23:59:60Z
        694224018,   // 18: 1979-12-31T23:59:60Z
        662688017,   // 17: 1978-12-31T23:59:60Z
        631152016,   // 16: 1977-12-31T23:59:60Z
        599616015,   // 15: 1976-12-31T23:59:60Z
        567993614,   // 14: 1975-12-31T23:59:60Z
        536457613,   // 13: 1974-12-31T23:59:60Z
        504921612,   // 12: 1973-12-31T23:59:60Z
        473385611,   // 11: 1972-12-31T23:59:60Z
        457488010,   // 10: 1972-06-30T23:59:60Z
                     // Start of leap second correction
};

int64_t
TimeEpochConverter<TaiEpoch, UnixEpoch>::getCorrectionFactorForLeapSeconds(
        int64_t seconds, LeapSecondCorrection::Type correction)
{
    outpost::Slice<const int64_t> leapSeconds(leapSecondArray);
    int64_t correctionFactor = 0;
    for (size_t i = 0; (i < leapSeconds.getNumberOfElements()) && (correctionFactor == 0); ++i)
    {
        if (seconds >= leapSeconds[i])
        {
            correctionFactor = leapSeconds.getNumberOfElements() - i;

            // As leap seconds are accumulated, it can happen that adding
            // leap seconds causes the resulting time to overflow into the
            // next leap second.
            //
            // This is not possible for the latest leap seconds (i = 0),
            // therefore it is excluded here.
            if ((correction == LeapSecondCorrection::add) && (i != 0U))
            {
                seconds = seconds + correctionFactor;
                if (seconds >= leapSeconds[i - 1U])
                {
                    correctionFactor += 1;
                }
            }
        }
    }

    return correctionFactor;
}

// ----------------------------------------------------------------------------
TimePoint<UnixEpoch>
TimeEpochConverter<TaiEpoch, UnixEpoch>::convert(TimePoint<TaiEpoch> from)
{
    // leap second correction
    int64_t correction = getCorrectionFactorForLeapSeconds(from.timeSinceEpoch().seconds(),
                                                           LeapSecondCorrection::remove);
    return TimePoint<UnixEpoch>::afterEpoch(from.timeSinceEpoch()
                                            - Seconds(initialOffsetInSeconds + correction));
}

TimePoint<TaiEpoch>
TimeEpochConverter<UnixEpoch, TaiEpoch>::convert(TimePoint<UnixEpoch> from)
{
    int64_t seconds = from.timeSinceEpoch().seconds() + initialOffsetInSeconds;

    typedef TimeEpochConverter<TaiEpoch, UnixEpoch> Other;

    // leap second correction
    int64_t correction =
            Other::getCorrectionFactorForLeapSeconds(seconds, Other::LeapSecondCorrection::add);
    return TimePoint<TaiEpoch>::afterEpoch(from.timeSinceEpoch()
                                           + Seconds(initialOffsetInSeconds + correction));
}
