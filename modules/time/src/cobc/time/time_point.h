/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_TIME_TIME_POINT_H
#define COBC_TIME_TIME_POINT_H

#include <stdint.h>
#include "duration.h"

namespace cobc
{
namespace time
{

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
 *     Duration + TimePoint  --> Undefined
 *     Duration - TimePoint  --> Undefined
 *     TimePoint + TimePoint --> Undefined
 *
 * The resolution of a TimePoint is microseconds.
 *
 * \ingroup time
 * \author  Fabian Greif
 */
class TimePoint
{
public:
    friend class TimeModel;

    typedef uint64_t Type;
    typedef int64_t SignedType;

    inline
    TimePoint(const TimePoint& other) :
        time(other.time)
    {
    }

    inline
    ~TimePoint()
    {
    }

    inline Duration
    operator -(const TimePoint& other) const
    {
        return Duration(time - other.time);
    }

    inline TimePoint
    operator -=(const Duration& d)
    {
        time = time - d.ticks;
        return TimePoint(time);
    }

    inline TimePoint
    operator +(const Duration& d) const
    {
        return TimePoint(time + d.ticks);
    }

    inline TimePoint
    operator +=(const Duration& d)
    {
        time = time + d.ticks;
        return TimePoint(time);
    }

    inline bool
    operator ==(const TimePoint& other) const
    {
        return (time == other.time);
    }

    inline bool
    operator !=(const TimePoint& other) const
    {
        return (time != other.time);
    }

    inline bool
    operator <(const TimePoint& other) const
    {
        return (static_cast<SignedType>(time - other.time)) < 0;
    }

    inline bool
    operator >(const TimePoint& other) const
    {
        return (static_cast<SignedType>(time - other.time)) > 0;
    }

    inline bool
    operator <=(const TimePoint& other) const
    {
        return (static_cast<SignedType>(time - other.time)) <= 0;
    }

    inline bool
    operator >=(const TimePoint& other) const
    {
        return (static_cast<SignedType>(time - other.time)) >= 0;
    }

    inline TimePoint&
    operator =(const TimePoint& other)
    {
        // This gracefully handles self assignment
        time = other.time;
        return *this;
    }

//protected:
    explicit inline
    TimePoint(const Type time = 0) :
            time(time)
    {
    }

private:
    Type time;
};

}
}

#endif // COBC_TIME_TIME_POINT_H
