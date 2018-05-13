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
