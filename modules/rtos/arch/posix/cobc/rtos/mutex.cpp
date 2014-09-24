/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "mutex.h"

#include <time.h>

#include "internal/time.h"

using namespace cobc::rtos;

bool
Mutex::acquire(cobc::time::Duration timeout)
{
    timespec time = toRelativeTime(timeout);
    return (pthread_mutex_timedlock(&mutex, &time) == 0);
}
