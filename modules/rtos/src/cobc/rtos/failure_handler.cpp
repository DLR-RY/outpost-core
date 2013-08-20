/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "failure_handler.h"

#include "detect.h"

#if COBC_RTOS == COBC_RTOS_RTEMS
#	include <rtems.h>
#elif COBC_RTOS == COBC_RTOS_POSIX
#	include <cstdio>
#	include <cstdlib>

#	include <inttypes.h>
#endif

static void
defaultHandler(cobc::rtos::FailureCode code)
{
#if COBC_RTOS == COBC_RTOS_RTEMS
	rtems_fatal_error_occurred(code.getCode());
#elif COBC_RTOS == COBC_RTOS_POSIX
	//printf("Fatal Handler: %"PRIu32"\n", code.getCode());
	printf("Fatal Handler: %i\n", static_cast<int>(code.getCode()));
	exit(1);
#else
	// Avoid warnings about unused parameters.
	(void) code;
#endif
}

cobc::rtos::FailureHandler::Handler cobc::rtos::FailureHandler::handler = &defaultHandler;

void
cobc::rtos::FailureHandler::fatal(FailureCode code)
{
	// forward call to handler function
	handler(code);
}

void
cobc::rtos::FailureHandler::setFailureHandlerFunction(Handler newHandler)
{
	handler = newHandler;
}
