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
 * - 2015, Annika Ofenloch (DLR RY-AVS)
 * - 2016, Jan Sommer (DLR SC-SRV)
 * - 2021, Felix Passenberg (DLR RY-AVS)
 */

#ifndef OUTPOST_TIME_DURATION_H
#define OUTPOST_TIME_DURATION_H

#include <limits>
#include <stdint.h>

namespace outpost
{
namespace time
{
/**
 * A length of time unattached to any point on the time continuum.
 *
 * Available operations:
 *
 *     Duration + Duration  --> Duration
 *     Duration - Duration  --> Duration
 *
 *     Duration * Integer   --> Duration
 *     Integer  * Duration  --> Duration
 *     Duration / Integer   --> Duration  (Integer Division rules)
 *     Duration / Duration  --> Integer   (Integer Division rules)
 *     Duration % Duration  --> Duration
 *
 * Resolution is microseconds.
 *
 * \ingroup time
 * \author  Fabian Greif
 */
class Duration
{
    friend constexpr Duration
    operator*(int64_t muliplier, const Duration& duration);

    template <class T>
    friend struct std::numeric_limits;

public:
    template <typename ReferenceEpoch>
    friend class TimePoint;

    static constexpr int64_t nanosecondsPerMicrosecond = 1000;
    static constexpr int64_t microsecondsPerMillisecond = 1000;
    static constexpr int64_t millisecondsPerSecond = 1000;
    static constexpr int64_t secondsPerMinute = 60;
    static constexpr int64_t minutesPerHour = 60;

    static constexpr int64_t secondsPerDay = 86400;

    inline ~Duration() = default;

    /**
     * Default constructor
     */
    inline constexpr Duration() : mTicks(0)
    {
    }

    /**
     * Copy constructor
     */
    inline constexpr Duration(const Duration& other) : mTicks(other.mTicks)
    {
    }

    inline Duration&
    operator=(const Duration& other)
    {
        // This gracefully handles self assignment
        mTicks = other.mTicks;
        return *this;
    }

    /**
     * Get duration in hours.
     *
     * \return  Returns number of hours truncating any fractional hours.
     */
    inline constexpr int64_t
    hours() const
    {
        return mTicks
               / (microsecondsPerMillisecond * millisecondsPerSecond * secondsPerMinute
                  * minutesPerHour);
    }

    /**
     * Get duration in minutes.
     *
     * \return  Returns number of minutes truncating any fractional minutes.
     */
    inline constexpr int64_t
    minutes() const
    {
        return mTicks / (microsecondsPerMillisecond * millisecondsPerSecond * secondsPerMinute);
    }

    /**
     * Get duration in seconds.
     *
     * \return  Returns number of seconds truncating any fractional seconds.
     */
    inline constexpr int64_t
    seconds() const
    {
        return mTicks / (microsecondsPerMillisecond * millisecondsPerSecond);
    }

    /**
     * Get duration in milliseconds.
     *
     * \return    Returns number of milliseconds truncating any
     *             fractional milliseconds.
     */
    inline constexpr int64_t
    milliseconds() const
    {
        return mTicks / microsecondsPerMillisecond;
    }

    /**
     * Get duration in microseconds.
     *
     * \return    Returns number of microseconds truncating any
     *             fractional microseconds.
     */
    inline constexpr int64_t
    microseconds() const
    {
        return mTicks;
    }

    /**
     * the maximum value the type can hold
     *
     * for reasonably long durations better use myriad
     * they are safer against calculations.
     * maximum will overflow.
     *
     */
    static inline constexpr Duration
    maximum()
    {
        return Duration(maximumValue);
    }

#if __cplusplus >= 201309L
    [[deprecated("ambiguous: use myriad for long time, use numeric_limits<Duration>::max() for the "
                 "absolute maximum value")]]
#endif
    static inline constexpr Duration
    infinity()
    {
        return Duration(maximumValue);
    }

    /**
     * approximately 10000 years.
     *
     * does not calculate leap years, leap seconds and other stuff.
     * some places use it to consider the timeout as never happening.
     * Banzai!
     */
    static inline constexpr Duration
    myriad()  // 10000 years
    {
        return Duration(10000 * 365 * 24 * minutesPerHour * secondsPerMinute * millisecondsPerSecond
                        * microsecondsPerMillisecond);
    }

    static inline constexpr Duration
    zero()
    {
        return Duration(0);
    }

    inline constexpr Duration
    operator-() const
    {
        return Duration(-mTicks);
    }

    inline constexpr Duration
    operator-(Duration other) const
    {
        return Duration(mTicks - other.mTicks);
    }

    inline constexpr Duration
    operator+(Duration other) const
    {
        return Duration(mTicks + other.mTicks);
    }

    inline constexpr Duration
    operator/(int32_t divisor) const
    {
        return Duration(mTicks / divisor);
    }

    inline constexpr int64_t
    operator/(Duration divisor) const
    {
        return mTicks / divisor.mTicks;
    }

    inline constexpr Duration
    operator%(Duration divisor) const
    {
        return Duration(mTicks % divisor.mTicks);
    }

    inline Duration
    operator-=(Duration other)
    {
        mTicks = mTicks - other.mTicks;
        return Duration(mTicks);
    }

    inline Duration
    operator+=(Duration other)
    {
        mTicks = mTicks + other.mTicks;
        return Duration(mTicks);
    }

    inline Duration
    operator/=(int64_t divisor)
    {
        mTicks = mTicks / divisor;
        return Duration(mTicks);
    }

    inline constexpr Duration
    operator*(int32_t rhs) const
    {
        return Duration(mTicks * rhs);
    }

    inline Duration
    operator*=(int64_t divisor)
    {
        mTicks = mTicks * divisor;
        return Duration(mTicks);
    }

    inline constexpr bool
    operator<(Duration rhs) const
    {
        return mTicks < rhs.mTicks;
    }

    inline constexpr bool
    operator>(Duration rhs) const
    {
        return mTicks > rhs.mTicks;
    }

    inline constexpr bool
    operator<=(Duration rhs) const
    {
        return mTicks <= rhs.mTicks;
    }

    inline constexpr bool
    operator>=(Duration rhs) const
    {
        return mTicks >= rhs.mTicks;
    }

    inline constexpr bool
    operator==(Duration rhs) const
    {
        return mTicks == rhs.mTicks;
    }

    inline constexpr bool
    operator!=(Duration rhs) const
    {
        return mTicks != rhs.mTicks;
    }

    inline constexpr Duration
    abs() const
    {
        return mTicks < 0 ? Duration(-mTicks) : Duration(mTicks);
    }

protected:
    explicit constexpr inline Duration(int64_t numberOfTicks) : mTicks(numberOfTicks)
    {
    }

private:
    // When using a microsecond resolution a 32 bit value would only
    // provide a range of ~2200 seconds which is insufficient.
    //
    // With 64 bit a time span of 9 * 10^12 seconds is possible:
    // (2^63 - 1) / 1,000,000 / 60 / 60 / 24 /365 = 292,471 years
    //
    // times longer than 10,000 years may be considered to never happen.
    int64_t mTicks;

    static constexpr int64_t maximumValue = 9223372036854775807LL;
    static constexpr int64_t minimalValue = -maximumValue - 1;
};

inline constexpr Duration
operator*(int64_t muliplier, const Duration& duration)
{
    return Duration(duration.mTicks * muliplier);
}

class Hours : public Duration
{
public:
    explicit inline constexpr Hours(int64_t value) :
        Duration(value * minutesPerHour * secondsPerMinute * millisecondsPerSecond
                 * microsecondsPerMillisecond)
    {
    }
};

class Minutes : public Duration
{
public:
    explicit inline constexpr Minutes(int64_t value) :
        Duration(value * secondsPerMinute * millisecondsPerSecond * microsecondsPerMillisecond)
    {
    }
};

/**
 * Allows expression of durations as a seconds count.
 *
 * \ingroup    time
 */
class Seconds : public Duration
{
public:
    explicit inline constexpr Seconds(int64_t value) :
        Duration(value * millisecondsPerSecond * microsecondsPerMillisecond)
    {
    }
};

/**
 * Allows expression of durations as a milliseconds.
 *
 * \ingroup    time
 */
class Milliseconds : public Duration
{
public:
    explicit inline constexpr Milliseconds(int64_t value) :
        Duration(value * microsecondsPerMillisecond)
    {
    }
};

/**
 * Allows expression of durations as a microseconds.
 *
 * \ingroup    time
 */
class Microseconds : public Duration
{
public:
    explicit inline constexpr Microseconds(int64_t value) : Duration(value)
    {
    }
};

}  // namespace time
}  // namespace outpost

namespace std
{
// only replacing where the return type was int64_t, thus the rest is inherited
template <>
struct numeric_limits<outpost::time::Duration> : public numeric_limits<int64_t>
{
    static constexpr outpost::time::Duration
    max()
    {
        return outpost::time::Duration(numeric_limits<int64_t>::max());
    }

    static constexpr outpost::time::Duration
    min()
    {
        return outpost::time::Duration(numeric_limits<int64_t>::min());
    }
#if __cplusplus >= 201103L
    static constexpr outpost::time::Duration
    lowest() noexcept
    {
        return min();
    }
#endif
    static constexpr outpost::time::Duration
    epsilon() noexcept
    {
        return outpost::time::Duration::zero();
    }
};
}  // namespace std

#endif
