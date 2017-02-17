/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 * - 2014, Norbert Toth (DLR RY-AVS)
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
#include "rtos/queue.h"
#include "rtos/thread.h"
#include "rtos/timer.h"

#include <outpost/rtos/failure_handler.h>
#include <outpost/rtos/mutex_guard.h>
#include <outpost/utils/callable.h>

#endif // COBC_RTOS_H
