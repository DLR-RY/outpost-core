/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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

#include "duration.h"

namespace cobc
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
template<typename Epoch_>
class TimePoint
{
public:
	typedef Epoch_ Epoch;

	inline
	TimePoint() :
		mDuration(Seconds(0))
	{
	}

    inline
    TimePoint(const TimePoint& other) :
        mDuration(other.mDuration)
    {
    }

    inline
    ~TimePoint()
    {
    }

    inline Duration
    operator-(TimePoint other) const
    {
        return Duration(mDuration - other.mDuration);
    }

    inline TimePoint
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

    inline TimePoint
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

    inline bool
    operator==(TimePoint other) const
    {
        return (mDuration == other.mDuration);
    }

    inline bool
    operator!=(TimePoint other) const
    {
        return (mDuration != other.mDuration);
    }

    inline bool
    operator<(TimePoint other) const
    {
        return (mDuration - other.mDuration) < Seconds(0);
    }

    inline bool
    operator>(TimePoint other) const
    {
        return (mDuration - other.mDuration) > Seconds(0);
    }

    inline bool
    operator<=(TimePoint other) const
    {
        return (mDuration - other.mDuration) <= Seconds(0);
    }

    inline bool
    operator>=(TimePoint other) const
    {
        return (mDuration - other.mDuration) >= Seconds(0);
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
    inline Duration
	timeSinceEpoch() const
    {
    	return mDuration;
    }

    /**
     * Construct a time point a given time after the start of the epoch.
     */
    static inline TimePoint<Epoch_>
    afterEpoch(Duration duration)
	{
    	return TimePoint(duration);
	}

    static inline TimePoint<Epoch_>
	startOfEpoch()
	{
		return TimePoint(Seconds(0));
	}

    static inline TimePoint<Epoch_>
    endOfEpoch()
    {
        return TimePoint(Duration::maximum());
    }

    template <typename To>
	inline To
	convertTo() const
	{
    	return TimeEpochConverter<Epoch_, typename To::Epoch>::convert(*this);
	}

protected:
    explicit inline
    TimePoint(const Duration duration) :
            mDuration(duration)
    {
    }

private:
    Duration mDuration;
};

}
}

#include "time_epoch.h"

#endif // COBC_TIME_TIME_POINT_H
