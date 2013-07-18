/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_TIMER_H
#define COBC_RTOS_TIMER_H

#ifdef __rtems__
#	include "rtems/timer.h"
#else
#	include "posix/timer.h"
#endif

#endif // COBC_RTOS_TIMER_H
