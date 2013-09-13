/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_PERIODIC_TASK_MANAGER_HPP
#define COBC_RTOS_PERIODIC_TASK_MANAGER_HPP

#include "detect.h"

#if COBC_RTOS == COBC_RTOS_NONE
#	include "none/periodic_task_manager.h"
#elif COBC_RTOS == COBC_RTOS_RTEMS
#	include "rtems/periodic_task_manager.h"
#elif COBC_RTOS == COBC_RTOS_FREERTOS
#	include "freertos/periodic_task_manager.h"
#elif COBC_RTOS == COBC_RTOS_POSIX
#	include "posix/periodic_task_manager.h"
#endif

#endif // COBC_RTOS_PERIODIC_TASK_MANAGER_HPP
