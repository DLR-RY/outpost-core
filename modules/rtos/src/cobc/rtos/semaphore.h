/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_SEMAPHORE_HPP
#define COBC_RTOS_SEMAPHORE_HPP

#if defined(__rtems__)
#	include "rtems/semaphore.h"
#elif defined(__unix__)
#	include "posix/semaphore.h"
#elif defined(FREERTOS)
#	include "freertos/semaphore.h"
#else
#	include "none/semaphore.h"
#endif

#endif // COBC_RTOS_SEMAPHORE_HPP
