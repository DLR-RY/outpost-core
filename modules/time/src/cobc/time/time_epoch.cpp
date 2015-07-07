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

#include "time_epoch.h"

#include <cobc/utils/bounded_array.h>

using namespace cobc::time;

Duration TimeEpochConverter<SpacecraftElapsedTimeEpoch,
							GpsEpoch>::offsetToGpsTime = Duration::zero();

/**
 * Leap second correction table.
 *
 * When new leap seconds are added they have to be added here.
 *
 * see also http://www.andrews.edu/~tzs/timeconv/timedisplay.php
 */
static const int64_t leapSecondArray[] =
{
	1119744016,		// 2015-06-30T23:59:60Z
	1025136015,		// 2012-06-30T23:50:60Z
	 914803214,		// 2008-12-31T23:50:60Z
	 820108813,		// 2005-12-31T23:50:60Z
	 599184012,		// 1998-12-31T23:50:60Z
	 551750411,		// 1997-06-30T23:59:60Z
	 504489610,		// 1995-12-31T23:50:60Z
	 457056009,		// 1994-06-30T23:59:60Z
	 425520008,		// 1993-06-30T23:59:60Z
	 393984007,		// 1992-06-30T23:59:60Z
	 346723206,		// 1990-12-31T23:50:60Z
	 315187205,		// 1989-12-31T23:50:60Z
	 252028804,		// 1987-12-31T23:50:60Z
	 173059203,		// 1985-06-30T23:59:60Z
	 109900802,		// 1983-06-30T23:59:60Z
	  78364801,		// 1982-06-30T23:59:60Z
	  46828800,		// 1981-06-30T23:59:60Z
};

Duration
cobc::time::getCorrectionFactorForLeapSeconds(int64_t seconds,
											  LeapSecondCorrection::Type correction)
{
	cobc::BoundedArray<const int64_t> leapSeconds(leapSecondArray);
	int64_t correctionFactor = 0;
	for (int i = 0; (i < static_cast<int>(leapSeconds.getNumberOfElements()))
					 && (correctionFactor == 0); ++i)
	{
		if (seconds >= leapSeconds[i])
		{
			correctionFactor = leapSeconds.getNumberOfElements() - i;
			if ((correction == LeapSecondCorrection::add) && (i != 0))
			{
				// As leap seconds are accumulated, it can happen that adding
				// leap seconds causes the resulting time to overflow into the
				// next leap second.
				seconds = seconds + correctionFactor;
				if (seconds >= leapSeconds[i - 1])
				{
					correctionFactor += 1;
				}
			}
		}
	}

	return Seconds(correctionFactor);
}
