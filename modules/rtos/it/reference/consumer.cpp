/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "../reference/consumer.h"

#include <outpost/rtos/mutex_guard.h>

Consumer::Consumer(outpost::rtos::Queue<uint32_t>& queue) :
    Thread(40, defaultStackSize, "Consumer"),
    mQueue(queue),
    mCurrentValue(0),
    mEvent(outpost::rtos::BinarySemaphore::State::acquired)
{
}

uint32_t
Consumer::getCurrentValue() const
{
    outpost::rtos::MutexGuard lock(mMutex);
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
        if (mQueue.receive(value, outpost::time::Milliseconds(500)))
        {
            outpost::rtos::MutexGuard lock(mMutex);
            mCurrentValue = value;

            mEvent.release();
        }
    }
}
