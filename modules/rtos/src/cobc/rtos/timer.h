/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_TIMER_H
#define COBC_RTOS_TIMER_H

#if defined(__rtems__)
#	include "rtems/timer.h"
#elif defined(__unix__)
#	include "posix/timer.h"
#elif defined(FREERTOS)
#	include "freertos/timer.h"
#else
#	include "none/timer.h"
#endif

#endif // COBC_RTOS_TIMER_H
