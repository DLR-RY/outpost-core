/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_CLOCK_H
#define COBC_RTOS_CLOCK_H

#ifdef __rtems__
#	include "rtems/clock.h"
#else
#	include "posix/clock.h"
#endif

#endif // COBC_RTOS_CLOCK_H
