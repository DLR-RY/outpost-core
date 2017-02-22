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
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_TIME_DURATION_H
#define OUTPOST_TIME_DURATION_H

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
 *
 * Resolution is microseconds.
 *
 * \ingroup time
 * \author  Fabian Greif
 */
class Duration
{
public:
    template <typename ReferenceEpoch>
    friend class TimePoint;

    static const int64_t nanosecondsPerMicrosecond = 1000;
    static const int64_t microsecondsPerMillisecond = 1000;
    static const int64_t millisecondsPerSecond = 1000;
    static const int64_t secondsPerMinute = 60;
    static const int64_t minutesPerHour = 60;

    static const int64_t secondsPerDay = 86400;

    inline
    ~Duration()
    {
    }

    /**
     * Copy constructor
     */
    inline
    Duration(const Duration& other) :
        mTicks(other.mTicks)
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
    inline int64_t
    hours() const
    {
        return mTicks / (microsecondsPerMillisecond *
                        millisecondsPerSecond *
                        secondsPerMinute *
                        minutesPerHour);
    }

    /**
     * Get duration in minutes.
     *
     * \return  Returns number of minutes truncating any fractional minutes.
     */
    inline int64_t
    minutes() const
    {
        return mTicks / (microsecondsPerMillisecond *
                        millisecondsPerSecond *
                        secondsPerMinute);
    }

    /**
     * Get duration in seconds.
     *
     * \return  Returns number of seconds truncating any fractional seconds.
     */
    inline int64_t
    seconds() const
    {
        return mTicks / (microsecondsPerMillisecond *
                        millisecondsPerSecond);
    }

    /**
     * Get duration in milliseconds.
     *
     * \return    Returns number of milliseconds truncating any
     *             fractional milliseconds.
     */
    inline int64_t
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
    inline int64_t
    microseconds() const
    {
        return mTicks;
    }

    static inline Duration
    maximum()
    {
        return Duration(maximumValue);
    }

    static inline Duration
    infinity()
    {
        return Duration(maximumValue);
    }

    static inline Duration
    zero()
    {
        return Duration(0);
    }

    inline Duration
    operator-() const
    {
        return Duration(-mTicks);
    }

    inline Duration
    operator-(Duration other) const
    {
        return Duration(mTicks - other.mTicks);
    }

    inline Duration
    operator+(Duration other) const
    {
        return Duration(mTicks + other.mTicks);
    }

    inline Duration
    operator*(Duration other) const
    {
        return Duration(mTicks + other.mTicks);
    }

    inline Duration
    operator/(int divisor) const
    {
        return Duration(mTicks / divisor);
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
    operator/=(int divisor)
    {
        mTicks = mTicks / divisor;
        return Duration(mTicks);
    }

    inline Duration
    operator*(int rhs) const
    {
        return Duration(mTicks * rhs);
    }

    inline Duration
    operator*=(int divisor)
    {
        mTicks = mTicks * divisor;
        return Duration(mTicks);
    }

    inline bool
    operator<(Duration rhs)  const
    {
        return mTicks < rhs.mTicks;
    }

    inline bool
    operator>(Duration rhs)  const
    {
        return mTicks > rhs.mTicks;
    }

    inline bool
    operator<=(Duration rhs)  const
    {
        return mTicks <= rhs.mTicks;
    }

    inline bool
    operator>=(Duration rhs)  const
    {
        return mTicks >= rhs.mTicks;
    }

    inline bool
    operator==(Duration rhs)  const
    {
        return mTicks == rhs.mTicks;
    }

    inline bool
    operator!=(Duration rhs)  const
    {
        return mTicks != rhs.mTicks;
    }

    inline Duration
    abs() const
    {
        return mTicks < 0 ? Duration(-mTicks) : Duration(mTicks);
    }

protected:
    explicit inline
    Duration(int64_t numberOfTicks) :
        mTicks(numberOfTicks)
    {
    }

private:
    // When using a microsecond resolution a 32 bit value would only
    // provide a range of ~2200 seconds which is insufficient.
    // With 64 bit a time span of 9 * 10^12 seconds is possible.
    int64_t mTicks;

    static const int64_t maximumValue = 9223372036854775807LL;
    static const int64_t minimalValue = -maximumValue - 1;
};

class Hours : public Duration
{
public:
    explicit inline
    Hours(int64_t value) :
        Duration(value * minutesPerHour *
                         secondsPerMinute *
                         millisecondsPerSecond *
                         microsecondsPerMillisecond)
    {
    }
};

class Minutes : public Duration
{
public:
    explicit inline
    Minutes(int64_t value) :
        Duration(value * secondsPerMinute *
                         millisecondsPerSecond *
                         microsecondsPerMillisecond)
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
    explicit inline
    Seconds(int64_t value) :
        Duration(value * millisecondsPerSecond *
                         microsecondsPerMillisecond)
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
    explicit inline
    Milliseconds(int64_t value) :
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
    explicit inline
    Microseconds(int64_t value) :
        Duration(value)
    {
    }
};

}
}

#endif
