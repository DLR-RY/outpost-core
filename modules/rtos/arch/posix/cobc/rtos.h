/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_H
#define COBC_RTOS_H

/**
 * \defgroup rtos   RTOS Wrappers
 * \brief    Real Time Operating System
 *
 * This library provide an operating system abstraction layer.
 */

#include "rtos/clock.h"
#include "rtos/mutex.h"
#include "rtos/periodic_task_manager.h"
#include "rtos/semaphore.h"
#include "rtos/thread.h"
#include "rtos/timer.h"

#include <cobc/rtos/failure_handler.h>
#include <cobc/rtos/mutex_guard.h>
#include <cobc/rtos/callable.h>

#endif // COBC_RTOS_H
