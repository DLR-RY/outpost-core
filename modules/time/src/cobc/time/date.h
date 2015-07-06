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

#ifndef COBC_TIME_DATE_H
#define COBC_TIME_DATE_H

#include "time_epoch.h"

namespace cobc
{
namespace time
{

/**
 * Represents a specific instant in time.
 *
 * The date is given in UTC and the conversion functions accommodates for leap
 * seconds in the time between January 1980 and May 2015 (before the leap seconds
 * of June 2015).
 *
 * \author	Fabian Greif
 * \author	Annika Ofenloch
 */
struct Date
{
	/**
	 * Convert a GPS timestamp to a UTC date.
	 */
	static Date
	fromGpsTime(GpsTime time);

	/**
	 * Convert a date to GPS timestamp.
	 */
	static GpsTime
	toGpsTime(const Date& date);

	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
};

}
}

#endif // COBC_TIME_DATE_H
