/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
