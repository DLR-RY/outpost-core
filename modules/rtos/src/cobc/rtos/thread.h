/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_THREAD_H
#define COBC_RTOS_THREAD_H

#if defined(__rtems__)
#	include "rtems/thread.h"
#elif defined(__unix__)
#	include "posix/thread.h"
#elif defined(FREERTOS)
#	include "freertos/thread.h"
#else
#	include "none/thread.h"
#endif

#endif // COBC_RTOS_THREAD_H
