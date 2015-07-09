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

#ifndef COBC_TIME_TIME_EPOCH_H
#define COBC_TIME_TIME_EPOCH_H

#include "time_point.h"

namespace cobc
{
namespace time
{
/**
 * Time since 1980-01-06T00:00:00Z, no leap seconds
 */
class GpsEpoch
{
};

/**
 * Time since 1970-01-01T00:00:00Z, ignores leap seconds
 */
class UnixEpoch
{
};

/**
 * Time since 1999-08-22T00:00:00Z with leap seconds
 */
class GalileoSystemTimeEpoch
{
};

/**
 * Time since 2000-01-01T11:58:55Z
 */
class J2000Epoch
{
};

/**
 * Time since start of OBC.
 *
 * Strictly monotonous counter.
 */
class SpacecraftElapsedTimeEpoch
{
};


typedef TimePoint<SpacecraftElapsedTimeEpoch> SpacecraftElapsedTime;
typedef TimePoint<GpsEpoch> GpsTime;
typedef TimePoint<UnixEpoch> UnixTime;


// forward declaration
template <typename Epoch>
class TimePoint;

template <typename From, typename To>
class TimeEpochConverter
{
public:
	static TimePoint<To>
	convert(TimePoint<From> from);
};

}
}

#include "time_epoch_impl.h"

#endif // COBC_TIME_TIME_EPOCH_H
