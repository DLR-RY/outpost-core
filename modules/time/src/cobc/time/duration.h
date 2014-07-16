/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
    friend class TimePoint;
    friend class TimeModel;

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

    inline Duration &
    operator =(const Duration& other)
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
        return ticks / static_cast<int64_t>(60 * 60 * 1000 * 1000L);
    }

    /**
     * Get duration in minutes.
     *
     * \return  Returns number of minutes truncating any fractional minutes.
     */
    inline int64_t
    minutes() const
    {
        return ticks / (60 * 1000 * 1000);
    }

    /**
     * Get duration in seconds.
     *
     * \return  Returns number of seconds truncating any fractional seconds.
     */
    inline int64_t
    seconds() const
    {
        return ticks / (1000 * 1000);
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
        return ticks / 1000;
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
    zero()
    {
        return Duration(0);
    }

    inline Duration
    operator -() const
    {
        return Duration(-ticks);
    }

    inline Duration
    operator -(const Duration& other) const
    {
        return Duration(ticks - other.ticks);
    }

    inline Duration
    operator +(const Duration& other) const
    {
        return Duration(ticks + other.ticks);
    }

    inline Duration
    operator *(const Duration& other) const
    {
        return Duration(ticks + other.ticks);
    }

    inline Duration
    operator /(int divisor) const
    {
        return Duration(ticks / divisor);
    }

    inline Duration
    operator -=(const Duration& other)
    {
        ticks = ticks - other.ticks;
        return Duration(ticks);
    }

    inline Duration
    operator +=(const Duration& other)
    {
        ticks = ticks + other.ticks;
        return Duration(ticks);
    }

    inline Duration
    operator /=(int divisor)
    {
        ticks = ticks / divisor;
        return Duration(ticks);
    }

    inline Duration
    operator *(int rhs) const
    {
        return Duration(ticks * rhs);
    }

    inline Duration
    operator *=(int divisor)
    {
        ticks = ticks * divisor;
        return Duration(ticks);
    }

    inline bool
    operator <(const Duration& rhs)  const
    {
        return ticks < rhs.ticks;
    }

    inline bool
    operator >(const Duration& rhs)  const
    {
        return ticks > rhs.ticks;
    }

    inline bool
    operator <=(const Duration& rhs)  const
    {
        return ticks <= rhs.ticks;
    }

    inline bool
    operator >=(const Duration& rhs)  const
    {
        return ticks >= rhs.ticks;
    }

    inline bool
    operator ==(const Duration& rhs)  const
    {
        return ticks == rhs.ticks;
    }

    inline bool
    operator !=(const Duration& rhs)  const
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

    static const int64_t minimalValue = (-(9223372036854775807LL)-1);
    static const int64_t maximumValue = ((9223372036854775807LL));
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
    Seconds(int32_t seconds) :
        Duration(static_cast<int64_t>(seconds) * 1000 * 1000)
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
    Milliseconds(int32_t milliseconds) :
        Duration(static_cast<int64_t>(milliseconds) * 1000)
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
    Microseconds(int32_t microseconds) :
        Duration(static_cast<int64_t>(microseconds))
    {
    }
};

}
}

#endif // COBC_TIME_DURATION_H
