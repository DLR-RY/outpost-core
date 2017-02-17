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
 * - 2014, Benjamin Weps (DLR SC-SRV)
 * - 2015, Annika Ofenloch (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_TIME_TIME_EPOCH_H
#define COBC_TIME_TIME_EPOCH_H

#include "time_point.h"

namespace outpost
{
namespace time
{
/**
 * International Atomic Time (TAI).
 *
 * Time since 1958-01-01T00:00:00Z, no leap seconds
 */
class TaiEpoch
{
};

/**
 * Time since 1980-01-06T00:00:00Z, no leap seconds
 */
class GpsEpoch
{
};

/**
 * Time since 1970-01-01T00:00:00Z, ignores leap seconds.
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
 * Time since start of OBC (SCET).
 *
 * Strictly monotonous counter.
 */
class SpacecraftElapsedTimeEpoch
{
};


typedef TimePoint<SpacecraftElapsedTimeEpoch> SpacecraftElapsedTime;
typedef TimePoint<GpsEpoch> GpsTime;
typedef TimePoint<TaiEpoch> AtomicTime;
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

/**
 * Set offset between SCET and GPS time.
 *
 * Convenience function for
 * TimeEpochConverter<SpacecraftElapsedTimeEpoch, GpsEpoch>::setOffset(...)
 */
void
setOffsetBetweenScetAndGps(SpacecraftElapsedTime scet, GpsTime gps);

}
}

#include "time_epoch_impl.h"

#endif // COBC_TIME_TIME_EPOCH_H
