/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_THREAD_H
#define COBC_RTOS_THREAD_H

#ifdef __rtems__
#	include "rtems/thread.h"
#else
#	include "posix/thread.h"
#endif

#endif // COBC_RTOS_THREAD_H
