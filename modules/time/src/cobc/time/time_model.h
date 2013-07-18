
#ifndef COBC_TIME__TIME_MODEL_H
#define COBC_TIME__TIME_MODEL_H

#include "time_point.h"

namespace cobc
{
	namespace time
	{
		/**
		 * Time model used to convert between different time representations.
		 * 
		 * @ingroup	time
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class TimeModel
		{
		public:
			static TimePoint
			endOfEpoch();
		};
	}
}

#endif // COBC_TIME__TIME_MODEL_H
