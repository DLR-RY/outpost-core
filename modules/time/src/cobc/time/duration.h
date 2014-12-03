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

#ifndef COBC_TIME_DURATION_H
#define COBC_TIME_DURATION_H

#include <stdint.h>

namespace cobc
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
    friend class TimeModel;
    friend class TimePoint;

    static const int64_t numberOfMicrosecondsPerMillisecond = 1000;
    static const int64_t numberOfMillisecondsPerSecond = 1000;
    static const int64_t numberOfSecondsPerMinute = 1000;
    static const int64_t numberOfMinutesPerHour = 1000;

    inline
    ~Duration()
    {
    }

    /**
     * Copy constructor
     */
    inline
    Duration(const Duration& other) :
        ticks(other.ticks)
    {
    }

    inline Duration&
    operator=(const Duration& other)
    {
        // This gracefully handles self assignment
        ticks = other.ticks;
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
        return ticks / (numberOfMicrosecondsPerMillisecond *
                        numberOfMillisecondsPerSecond *
                        numberOfSecondsPerMinute *
                        numberOfMinutesPerHour);
    }

    /**
     * Get duration in minutes.
     *
     * \return  Returns number of minutes truncating any fractional minutes.
     */
    inline int64_t
    minutes() const
    {
        return ticks / (numberOfMicrosecondsPerMillisecond *
                        numberOfMillisecondsPerSecond *
                        numberOfSecondsPerMinute);
    }

    /**
     * Get duration in seconds.
     *
     * \return  Returns number of seconds truncating any fractional seconds.
     */
    inline int64_t
    seconds() const
    {
        return ticks / (numberOfMicrosecondsPerMillisecond *
                        numberOfMillisecondsPerSecond);
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
        return ticks / numberOfMicrosecondsPerMillisecond;
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
        return ticks;
    }

    static inline Duration
    max()
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
        return Duration(-ticks);
    }

    inline Duration
    operator-(Duration other) const
    {
        return Duration(ticks - other.ticks);
    }

    inline Duration
    operator+(Duration other) const
    {
        return Duration(ticks + other.ticks);
    }

    inline Duration
    operator*(Duration other) const
    {
        return Duration(ticks + other.ticks);
    }

    inline Duration
    operator/(int divisor) const
    {
        return Duration(ticks / divisor);
    }

    inline Duration
    operator-=(Duration other)
    {
        ticks = ticks - other.ticks;
        return Duration(ticks);
    }

    inline Duration
    operator+=(Duration other)
    {
        ticks = ticks + other.ticks;
        return Duration(ticks);
    }

    inline Duration
    operator/=(int divisor)
    {
        ticks = ticks / divisor;
        return Duration(ticks);
    }

    inline Duration
    operator*(int rhs) const
    {
        return Duration(ticks * rhs);
    }

    inline Duration
    operator*=(int divisor)
    {
        ticks = ticks * divisor;
        return Duration(ticks);
    }

    inline bool
    operator<(Duration rhs)  const
    {
        return ticks < rhs.ticks;
    }

    inline bool
    operator>(Duration rhs)  const
    {
        return ticks > rhs.ticks;
    }

    inline bool
    operator<=(Duration rhs)  const
    {
        return ticks <= rhs.ticks;
    }

    inline bool
    operator>=(Duration rhs)  const
    {
        return ticks >= rhs.ticks;
    }

    inline bool
    operator==(Duration rhs)  const
    {
        return ticks == rhs.ticks;
    }

    inline bool
    operator!=(Duration rhs)  const
    {
        return ticks != rhs.ticks;
    }

protected:
    explicit inline
    Duration(int64_t numberOfTicks) :
        ticks(numberOfTicks)
    {
    }

// FIXME make this private again -> currently problem in cobc/pus/command_schedule/scheduled_command.h
//private:
    // When using a microsecond resolution a 32 bit value would only
    // provide a range of ~2200 seconds which is insufficient.
    // With 64 bit a time span of 9 * 10^12 seconds is possible.
    int64_t ticks;

    static const int64_t maximumValue = 9223372036854775807LL;
    static const int64_t minimalValue = -maximumValue - 1;
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
    Seconds(int32_t value) :
        Duration(static_cast<int64_t>(value) * numberOfMillisecondsPerSecond *
                                               numberOfMicrosecondsPerMillisecond)
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
    Milliseconds(int32_t value) :
        Duration(static_cast<int64_t>(value) * numberOfMicrosecondsPerMillisecond)
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
    Microseconds(int32_t value) :
        Duration(static_cast<int64_t>(value))
    {
    }
};

}
}

#endif // COBC_TIME_DURATION_H
