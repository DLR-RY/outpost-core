/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_CLOCK_H
#define COBC_RTOS_CLOCK_H

#if defined(__rtems__)
#	include "rtems/clock.h"
#elif defined(__unix__)
#	include "posix/clock.h"
#elif defined(FREERTOS)
#	include "freertos/clock.h"
#else
#	include "none/clock.h"
#endif

#endif // COBC_RTOS_CLOCK_H
