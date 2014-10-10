/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
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
#include "rtos/queue.h"
#include "rtos/semaphore.h"
#include "rtos/thread.h"
#include "rtos/timer.h"

#include <cobc/rtos/failure_handler.h>
#include <cobc/rtos/mutex_guard.h>
#include <cobc/utils/callable.h>

#endif // COBC_RTOS_H
