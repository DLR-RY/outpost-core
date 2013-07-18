
#ifndef COBC__TIME_H_
#define COBC__TIME_H

/**
 * @defgroup	time
 * @brief		Time management
 * 
 * In the satellite there are different time representations. TM/TC use
 * CUC (CCSDS Unsegmented time Code) and UTC encoded as CDS (CCSDS Day
 * Segmented time code) while the satellite itself uses a local time
 * representation for calculations. This local time lacks for example a
 * leap-second correction but is very efficient for time calculations.
 * 
 * The local time is simply the number of seconds from the start of the
 * satellite.
 * 
 * cobc::TimeModel is responsible for converting between local Time and UTC,
 * while ccsds::Time is used to convert between CDS and UTC and CUC and
 * local time.
 */

#include "time/duration.h"
#include "time/time_point.h"
#include "time/time_model.h"

namespace cobc
{
	/**
	 * @ingroup	time
	 */
	namespace time
	{
	}
}

#endif // COBC__TIME_H
