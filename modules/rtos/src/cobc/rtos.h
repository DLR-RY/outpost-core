/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_H
#define COBC_RTOS_H

/**
 * @defgroup	rtos	RTOS Wrappers
 * @brief		Real Time Operating System
 *
 * This library provide an operating system abstraction layer.
 *
 */

#include "rtos/clock.h"
#include "rtos/failure_handler.h"
#include "rtos/mutex.h"
#include "rtos/rate_monotonic_period.h"
#include "rtos/semaphore.h"
#include "rtos/thread.h"
#include "rtos/timer.h"

#endif // COBC_RTOS_H
