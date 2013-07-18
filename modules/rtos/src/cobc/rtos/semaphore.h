/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_SEMAPHORE_HPP
#define COBC_RTOS_SEMAPHORE_HPP

#ifdef __rtems__
#	include "rtems/semaphore.h"
#else
#	include "posix/semaphore.h"
#endif

#endif // COBC_RTOS_SEMAPHORE_HPP
