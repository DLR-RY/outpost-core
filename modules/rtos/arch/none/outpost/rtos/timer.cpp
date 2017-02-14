/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

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
//void
//outpost::rtos::Timer::invokeTimer(rtems_id id, void* parameter)
//{
//    Timer* timer = reinterpret_cast<Timer *>(parameter);
//    (timer->object->*(timer->function))(timer);
//}
