/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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
cobc::rtos::Timer::~Timer()
{

}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::start(time::Duration duration)
{
    (void) duration;
}

void
cobc::rtos::Timer::reset()
{
}

void
cobc::rtos::Timer::cancel()
{
}

// ----------------------------------------------------------------------------
void
cobc::rtos::Timer::createTimer(const char* name)
{
    (void) name;
}

// ----------------------------------------------------------------------------
//void
//cobc::rtos::Timer::invokeTimer(rtems_id id, void* parameter)
//{
//    Timer* timer = reinterpret_cast<Timer *>(parameter);
//    (timer->object->*(timer->function))(timer);
//}
