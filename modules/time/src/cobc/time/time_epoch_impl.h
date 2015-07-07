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
class TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>
{
public:
	static Duration offsetToGpsTime;

	static TimePoint<GpsEpoch>
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
};

template <>
class TimeEpochConverter<GpsEpoch, SpacecraftElapsedTimeEpoch>
{
public:
	static TimePoint<SpacecraftElapsedTimeEpoch>
	convert(TimePoint<GpsEpoch> from)
	{
		return TimePoint<SpacecraftElapsedTimeEpoch>::afterEpoch(from.timeSinceEpoch()
				- TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::offsetToGpsTime);
	}
};

struct LeapSecondCorrection
{
	enum Type
	{
		remove,
		add
	};
};

Duration
getCorrectionFactorForLeapSeconds(int64_t seconds,
								  LeapSecondCorrection::Type correction);

// ----------------------------------------------------------------------------
template <>
class TimeEpochConverter<GpsEpoch, UnixEpoch>
{
public:
	static const int64_t initialOffsetInSeconds = 315964800;

	static TimePoint<UnixEpoch>
	convert(TimePoint<GpsEpoch> from)
	{
		// leap second correction
		Duration correction = getCorrectionFactorForLeapSeconds(from.timeSinceEpoch().seconds(),
																LeapSecondCorrection::remove);
		return TimePoint<UnixEpoch>::afterEpoch(
				from.timeSinceEpoch()
				+ Seconds(initialOffsetInSeconds)
				- correction);
	}
};

template <>
class TimeEpochConverter<UnixEpoch, GpsEpoch>
{
public:
	static TimePoint<GpsEpoch>
	convert(TimePoint<UnixEpoch> from)
	{
		// leap second correction
		int64_t seconds = from.timeSinceEpoch().seconds()
				- TimeEpochConverter<GpsEpoch, UnixEpoch>::initialOffsetInSeconds;

		Duration correction = getCorrectionFactorForLeapSeconds(seconds, LeapSecondCorrection::add);
		return TimePoint<GpsEpoch>::afterEpoch(
				from.timeSinceEpoch()
				- Seconds(TimeEpochConverter<GpsEpoch, UnixEpoch>::initialOffsetInSeconds)
				+ correction);
	}
};

}
}

#endif // COBC_TIME_TIME_EPOCH_IMPL_H
