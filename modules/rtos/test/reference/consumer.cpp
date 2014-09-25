/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "consumer.h"

#include <cobc/rtos/mutex_guard.h>

Consumer::Consumer(cobc::rtos::Queue<uint32_t>& queue) :
    Thread(40, defaultStackSize, "Consumer"),
    mQueue(queue),
    mCurrentValue(0),
    mEvent(cobc::rtos::BinarySemaphore::State::acquired)
{
}

uint32_t
Consumer::getCurrentValue() const
{
    cobc::rtos::MutexGuard lock(mMutex);
    return mCurrentValue;
}

void
Consumer::waitForNewValue()
{
    mEvent.acquire();
}

void
Consumer::run()
{
    while (1)
    {
        uint32_t value = 0;
        if (mQueue.receive(value, cobc::time::Milliseconds(500)))
        {
            cobc::rtos::MutexGuard lock(mMutex);
            mCurrentValue = value;

            mEvent.release();
        }
    }
}
