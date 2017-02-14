/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#ifndef COBC_TIME_TIME_EPOCH_IMPL_H
#define COBC_TIME_TIME_EPOCH_IMPL_H

#include "time_epoch.h"

namespace outpost
{
namespace time
{

// ----------------------------------------------------------------------------
template <>
class TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>
{
public:
    static Duration offsetToGpsTime;

    static inline TimePoint<GpsEpoch>
    convert(TimePoint<SpacecraftElapsedTimeEpoch> from)
    {
        return TimePoint<GpsEpoch>::afterEpoch(from.timeSinceEpoch()
                + offsetToGpsTime);
    }

    static inline void
    setOffset(Duration duration)
    {
        offsetToGpsTime = duration;
    }

    /**
     * Set the offset so that the given SCET equals the GPS time.
     */
    static inline void
    setOffset(TimePoint<SpacecraftElapsedTimeEpoch> scet, TimePoint<GpsEpoch> gps)
    {
        offsetToGpsTime = gps.timeSinceEpoch() - scet.timeSinceEpoch();
    }
};

template <>
class TimeEpochConverter<GpsEpoch, SpacecraftElapsedTimeEpoch>
{
public:
    static inline TimePoint<SpacecraftElapsedTimeEpoch>
    convert(TimePoint<GpsEpoch> from)
    {
        return TimePoint<SpacecraftElapsedTimeEpoch>::afterEpoch(from.timeSinceEpoch()
                - TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::offsetToGpsTime);
    }
};

// ----------------------------------------------------------------------------
template <>
class TimeEpochConverter<TaiEpoch, UnixEpoch>
{
public:
    static const int64_t offsetDaysFromTaiToUnix = 4383;
    static const int64_t leapSecondsAtUnixEpoch = 10;

    static const int64_t initialOffsetInSeconds = offsetDaysFromTaiToUnix * Duration::secondsPerDay
                                                + leapSecondsAtUnixEpoch;

    struct LeapSecondCorrection
    {
        enum Type
        {
            remove,
            add
        };
    };

    static int64_t
    getCorrectionFactorForLeapSeconds(int64_t seconds,
                                      LeapSecondCorrection::Type correction);

    static TimePoint<UnixEpoch>
    convert(TimePoint<TaiEpoch> from);
};

template <>
class TimeEpochConverter<UnixEpoch, TaiEpoch>
{
public:
    static const int64_t initialOffsetInSeconds = TimeEpochConverter<TaiEpoch, UnixEpoch>::initialOffsetInSeconds;

    static TimePoint<TaiEpoch>
    convert(TimePoint<UnixEpoch> from);
};

// ----------------------------------------------------------------------------
template <>
class TimeEpochConverter<GpsEpoch, TaiEpoch>
{
public:
    static const int64_t offsetDaysTaiToGps = 8040;
    static const int64_t offsetLeapSecondsTaiToGps = 19;

    static const int64_t offsetInSeconds = offsetDaysTaiToGps * Duration::secondsPerDay
                                         + offsetLeapSecondsTaiToGps;

    static inline TimePoint<TaiEpoch>
    convert(TimePoint<GpsEpoch> from)
    {
        return TimePoint<TaiEpoch>::afterEpoch(from.timeSinceEpoch() + Seconds(offsetInSeconds));
    }
};

template <>
class TimeEpochConverter<TaiEpoch, GpsEpoch>
{
public:
    static const int64_t offsetInSeconds = TimeEpochConverter<GpsEpoch, TaiEpoch>::offsetInSeconds;

    static inline TimePoint<GpsEpoch>
    convert(TimePoint<TaiEpoch> from)
    {
        return TimePoint<GpsEpoch>::afterEpoch(from.timeSinceEpoch() - Seconds(offsetInSeconds));
    }
};

// ----------------------------------------------------------------------------
template <>
class TimeEpochConverter<GpsEpoch, UnixEpoch>
{
public:
    static inline TimePoint<UnixEpoch>
    convert(TimePoint<GpsEpoch> from)
    {
        return from.convertTo<TimePoint<TaiEpoch> >().convertTo<TimePoint<UnixEpoch> >();
    }
};

template <>
class TimeEpochConverter<UnixEpoch, GpsEpoch>
{
public:
    static inline TimePoint<GpsEpoch>
    convert(TimePoint<UnixEpoch> from)
    {
        return from.convertTo<TimePoint<TaiEpoch> >().convertTo<TimePoint<GpsEpoch> >();
    }
};

}
}

#endif // COBC_TIME_TIME_EPOCH_IMPL_H
