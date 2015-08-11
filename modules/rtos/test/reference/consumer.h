/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include <cobc/rtos/thread.h>
#include <cobc/rtos/queue.h>
#include <cobc/rtos/mutex.h>
#include <cobc/rtos/semaphore.h>

class Consumer : public cobc::rtos::Thread
{
public:
    Consumer(cobc::rtos::Queue<uint32_t>& queue);

    uint32_t
    getCurrentValue() const;

    void
    waitForNewValue();

private:
    virtual void
    run();

    cobc::rtos::Queue<uint32_t>& mQueue;

    mutable cobc::rtos::Mutex mMutex;
    uint32_t mCurrentValue;

    cobc::rtos::BinarySemaphore mEvent;
};

#endif // CONSUMER_H
