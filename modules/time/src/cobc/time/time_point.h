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
 *     Duration  + TimePoint --> Undefined
 *     Duration  - TimePoint --> Undefined
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

    typedef int64_t Type;

    inline
    TimePoint(const TimePoint& other) :
        mTicks(other.mTicks)
    {
    }

    inline
    ~TimePoint()
    {
    }

    inline Duration
    operator-(TimePoint other) const
    {
        return Duration(mTicks - other.mTicks);
    }

    inline TimePoint
    operator-=(Duration d)
    {
        mTicks = mTicks - d.mTicks;
        return TimePoint(mTicks);
    }

    inline TimePoint
    operator+(Duration d) const
    {
        return TimePoint(mTicks + d.mTicks);
    }

    inline TimePoint
    operator+=(Duration d)
    {
        mTicks = mTicks + d.mTicks;
        return TimePoint(mTicks);
    }

    inline bool
    operator==(TimePoint other) const
    {
        return (mTicks == other.mTicks);
    }

    inline bool
    operator!=(TimePoint other) const
    {
        return (mTicks != other.mTicks);
    }

    inline bool
    operator<(TimePoint other) const
    {
        return (mTicks - other.mTicks) < 0;
    }

    inline bool
    operator>(TimePoint other) const
    {
        return (mTicks - other.mTicks) > 0;
    }

    inline bool
    operator<=(TimePoint other) const
    {
        return (mTicks - other.mTicks) <= 0;
    }

    inline bool
    operator>=(TimePoint other) const
    {
        return (mTicks - other.mTicks) >= 0;
    }

    inline TimePoint&
    operator=(TimePoint other)
    {
        // This gracefully handles self assignment
        mTicks = other.mTicks;
        return *this;
    }

//protected:
    explicit inline
    TimePoint(const Type timePoint = 0) :
            mTicks(timePoint)
    {
    }

private:
    Type mTicks;
};

}
}

#endif // COBC_TIME_TIME_POINT_H
