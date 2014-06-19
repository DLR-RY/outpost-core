/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

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
//cobc::rtos::Timer::invokeTimer(rtems_id id, void * parameter)
//{
//    Timer * timer = reinterpret_cast<Timer *>(parameter);
//    (timer->object->*(timer->function))(timer);
//}
