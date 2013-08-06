/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "failure_handler.h"

#ifdef __rtems__
#	include <rtems.h>
#else
#	include <cstdio>
#	include <cstdlib>

#	include <inttypes.h>
#endif

void
cobc::rtos::FailureHandler::fatal(FailureCode code)
{
#ifdef __rtems__
	rtems_fatal_error_occurred(code.getCode());
#else
	//printf("Fatal Handler: %"PRIu32"\n", code.getCode());
	printf("Fatal Handler: %i\n", static_cast<int>(code.getCode()));
	exit(1);
#endif
}
