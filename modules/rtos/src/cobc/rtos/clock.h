/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_CLOCK_H
#define COBC_RTOS_CLOCK_H

#include "detect.h"

#if COBC_RTOS == COBC_RTOS_NONE
#	include "none/clock.h"
#elif COBC_RTOS == COBC_RTOS_RTEMS
#	include "rtems/clock.h"
#elif COBC_RTOS == COBC_RTOS_FREERTOS
#	include "freertos/clock.h"
#elif COBC_RTOS == COBC_RTOS_POSIX
#	include "posix/clock.h"
#endif

#endif // COBC_RTOS_CLOCK_H
