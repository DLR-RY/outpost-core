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
 */

#include "timer.h"

// ----------------------------------------------------------------------------
outpost::rtos::Timer::~Timer()
{
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::start(time::Duration duration)
{
    (void) duration;
}

void
outpost::rtos::Timer::reset()
{
}

void
outpost::rtos::Timer::cancel()
{
}

bool
outpost::rtos::Timer::isRunning()
{
    return false;
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Timer::createTimer(const char* name)
{
    (void) name;
}

// ----------------------------------------------------------------------------
// void
// outpost::rtos::Timer::invokeTimer(rtems_id id, void* parameter)
//{
//    Timer* timer = reinterpret_cast<Timer *>(parameter);
//    (timer->object->*(timer->function))(timer);
//}
