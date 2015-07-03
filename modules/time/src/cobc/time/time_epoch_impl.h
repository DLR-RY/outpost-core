/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
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

#ifndef COBC_TIME_TIME_EPOCH_IMPL_H
#define COBC_TIME_TIME_EPOCH_IMPL_H

#include "time_epoch.h"

namespace cobc
{
namespace time
{

// ----------------------------------------------------------------------------
template <>
class TimeEpochConverter<epoch::SpacecraftElapsedTime<epoch::GpsTime>, epoch::GpsTime>
{
public:
	static TimePoint<epoch::GpsTime>
	convert(TimePoint<epoch::SpacecraftElapsedTime<epoch::GpsTime> > from)
	{
		return TimePoint<epoch::GpsTime>::afterEpoch(from.timeSinceEpoch()
				+ epoch::SpacecraftElapsedTime<epoch::GpsTime>::offsetToGpsTime);
	}
};

template <>
class TimeEpochConverter<epoch::GpsTime, epoch::SpacecraftElapsedTime<epoch::GpsTime> >
{
public:
	static TimePoint<epoch::SpacecraftElapsedTime<epoch::GpsTime> >
	convert(TimePoint<epoch::GpsTime> from)
	{
		return TimePoint<epoch::SpacecraftElapsedTime<epoch::GpsTime> >::afterEpoch(from.timeSinceEpoch()
				- epoch::SpacecraftElapsedTime<epoch::GpsTime>::offsetToGpsTime);
	}
};

// ----------------------------------------------------------------------------
template <>
class TimeEpochConverter<epoch::GpsTime, epoch::UnixTime>
{
public:
	static const uint64_t offsetInSeconds = 315964800;

	static TimePoint<epoch::UnixTime>
	convert(TimePoint<epoch::GpsTime> from)
	{
		return TimePoint<epoch::UnixTime>::afterEpoch(from.timeSinceEpoch()
				+ Seconds(offsetInSeconds));
	}
};

template <>
class TimeEpochConverter<epoch::UnixTime, epoch::GpsTime>
{
public:
	static TimePoint<epoch::GpsTime>
	convert(TimePoint<epoch::UnixTime> from)
	{
		return TimePoint<epoch::GpsTime>::afterEpoch(from.timeSinceEpoch()
				- Seconds(TimeEpochConverter<epoch::GpsTime, epoch::UnixTime>::offsetInSeconds));
	}
};

}
}

#endif // COBC_TIME_TIME_EPOCH_IMPL_H
