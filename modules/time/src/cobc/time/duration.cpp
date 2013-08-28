/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "duration.h"

int64_t
cobc::time::Duration::seconds() const
{
	return ticks / (1000 * 1000);
}

int64_t
cobc::time::Duration::milliseconds() const
{
	return ticks / 1000;
}

int64_t
cobc::time::Duration::microseconds() const
{
	return ticks;
}

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
