/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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

#ifndef CONSUMER_H
#define CONSUMER_H

#include <outpost/rtos/thread.h>
#include <outpost/rtos/queue.h>
#include <outpost/rtos/mutex.h>
#include <outpost/rtos/semaphore.h>

class Consumer : public outpost::rtos::Thread
{
public:
    Consumer(outpost::rtos::Queue<uint32_t>& queue);

    uint32_t
    getCurrentValue() const;

    void
    waitForNewValue();

private:
    virtual void
    run();

    outpost::rtos::Queue<uint32_t>& mQueue;

    mutable outpost::rtos::Mutex mMutex;
    uint32_t mCurrentValue;

    outpost::rtos::BinarySemaphore mEvent;
};

#endif // CONSUMER_H
