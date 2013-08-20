/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_THREAD_H
#define COBC_RTOS_THREAD_H

#include "detect.h"

#if COBC_RTOS == COBC_RTOS_NONE
#	include "none/thread.h"
#elif COBC_RTOS == COBC_RTOS_RTEMS
#	include "rtems/thread.h"
#elif COBC_RTOS == COBC_RTOS_FREERTOS
#	include "freertos/thread.h"
#elif COBC_RTOS == COBC_RTOS_POSIX
#	include "posix/thread.h"
#endif

#endif // COBC_RTOS_THREAD_H
