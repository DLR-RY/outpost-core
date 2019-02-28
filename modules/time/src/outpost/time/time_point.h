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
 */

#ifndef OUTPOST_TIME_TIME_POINT_H
#define OUTPOST_TIME_TIME_POINT_H

#include "duration.h"

namespace outpost
{
namespace time
{
// forward declaration
template <typename From, typename To>
class TimeEpochConverter;

/**
 * Specifier for a location in the time continuum.
 *
 * Describes a time relative to an absolute time. The reference absolute time
 * is implementation defined and not fixed. The class \c TimeModel is
 * responsible for conversions between UTC and this class.
 *
 * Another way of saying the same thing, is that a TimePoint represents an
 * epoch plus or minus a duration.
 *
 * Available Operations:
 *
 *     TimePoint + Duration  --> TimePoint
 *     TimePoint - Duration  --> TimePoint
 *     TimePoint - TimePoint --> Duration
 *
 * Unlike regular numeric types, the following operations are
 * undefined:
 *
 *     Duration  + TimePoint --> Undefined
 *     Duration  - TimePoint --> Undefined
 *     TimePoint + TimePoint --> Undefined
 *
 * The resolution of a TimePoint is microseconds.
 *
 * \ingroup time
 * \author  Fabian Greif
 */
template <typename ReferenceEpoch>
class TimePoint
{
public:
    typedef ReferenceEpoch Epoch;

    inline constexpr TimePoint() : mDuration(Duration::zero())
    {
    }

    inline constexpr TimePoint(const TimePoint& other) : mDuration(other.mDuration)
    {
    }

    inline ~TimePoint() = default;

    inline constexpr Duration
    operator-(TimePoint other) const
    {
        return Duration(mDuration - other.mDuration);
    }

    inline constexpr TimePoint
    operator-(Duration duration) const
    {
        return TimePoint(mDuration - duration);
    }

    inline TimePoint&
    operator-=(Duration d)
    {
        mDuration = mDuration - d;
        return *this;
    }

    inline constexpr TimePoint
    operator+(Duration d) const
    {
        return TimePoint(mDuration + d);
    }

    inline TimePoint&
    operator+=(Duration d)
    {
        mDuration = mDuration + d;
        return *this;
    }

    inline constexpr bool
    operator==(TimePoint other) const
    {
        return (mDuration == other.mDuration);
    }

    inline constexpr bool
    operator!=(TimePoint other) const
    {
        return (mDuration != other.mDuration);
    }

    inline constexpr bool
    operator<(TimePoint other) const
    {
        return (mDuration - other.mDuration) < Duration::zero();
    }

    inline constexpr bool
    operator>(TimePoint other) const
    {
        return (mDuration - other.mDuration) > Duration::zero();
    }

    inline constexpr bool
    operator<=(TimePoint other) const
    {
        return (mDuration - other.mDuration) <= Duration::zero();
    }

    inline constexpr bool
    operator>=(TimePoint other) const
    {
        return (mDuration - other.mDuration) >= Duration::zero();
    }

    inline TimePoint&
    operator=(TimePoint other)
    {
        // This gracefully handles self assignment
        mDuration = other.mDuration;
        return *this;
    }

    /**
     * Get the duration since the start of the epoch.
     */
    inline constexpr Duration
    timeSinceEpoch() const
    {
        return mDuration;
    }

    /**
     * Construct a time point a given time after the start of the epoch.
     */
    static inline constexpr TimePoint<ReferenceEpoch>
    afterEpoch(Duration duration)
    {
        return TimePoint(duration);
    }

    static inline constexpr TimePoint<ReferenceEpoch>
    startOfEpoch()
    {
        return TimePoint(Duration::zero());
    }

    static inline constexpr TimePoint<ReferenceEpoch>
    endOfEpoch()
    {
        return TimePoint(Duration::maximum());
    }

    template <typename To>
    inline To
    convertTo() const
    {
        return TimeEpochConverter<ReferenceEpoch, typename To::Epoch>::convert(*this);
    }

protected:
    explicit inline constexpr TimePoint(const Duration duration) : mDuration(duration)
    {
    }

private:
    Duration mDuration;
};

}  // namespace time
}  // namespace outpost

#include "time_epoch.h"

#endif
