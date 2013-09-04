/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "duration.h"

// ----------------------------------------------------------------------------
cobc::time::Seconds::Seconds(int32_t seconds) :
	Duration(static_cast<int64_t>(seconds) * 1000 * 1000)
{
}

cobc::time::Milliseconds::Milliseconds(int32_t milliseconds) :
	Duration(static_cast<int64_t>(milliseconds) * 1000)
{
}

cobc::time::Microseconds::Microseconds(int32_t microseconds) :
	Duration(static_cast<int64_t>(microseconds))
{
}
