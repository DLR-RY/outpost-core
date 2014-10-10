/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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
