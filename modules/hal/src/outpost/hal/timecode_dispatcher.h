/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_HAL_TIMECODE_DISPATCHER_H_
#define OUTPOST_HAL_TIMECODE_DISPATCHER_H_

#include "timecode.h"

#include <outpost/rtos.h>
#include <outpost/rtos/queue.h>

#include <stdint.h>

#include <array>

namespace outpost
{
namespace hal
{
/**
 * An explicit Timecode/SpWInterrupt Dispatcher class such we don't get template parameters into
 * SpaceWire
 */
class TimeCodeDispatcherInterface
{
public:
    TimeCodeDispatcherInterface() = default;
    virtual ~TimeCodeDispatcherInterface() = default;

    /**
     * Dispatches a timecode to the Listener
     * This function shall be save to call within an ISR.
     *
     * @param tc the timecode(SpW Interrupt to dispatch
     */
    virtual void
    dispatchTimeCode(const TimeCode& tc) = 0;

    /**
     * Add a listener for timecode
     * @param queue the queue to add
     * @return false if queue == nullptr or all places for Listener are filled
     */
    virtual bool
    addListener(outpost::rtos::Queue<TimeCode>* queue) = 0;
};

template <uint32_t numberOfQueues>  // how many queues can be included
class TimeCodeDispatcher : public TimeCodeDispatcherInterface
{
public:
    TimeCodeDispatcher() : mNumberOfListeners(0)
    {
        mListener.fill(nullptr);
    };
    virtual ~TimeCodeDispatcher() = default;

    virtual void
    dispatchTimeCode(const TimeCode& tc)
    {
        // For one we are in a ISR and also nNumberOfListener only increments to
        // we don't need any mutex as worst case a just added listener don't get a notify.
        for (uint32_t i = 0; i < mNumberOfListeners; i++)
        {
            mListener[i]->send(tc);
        }
    }

    virtual bool
    addListener(outpost::rtos::Queue<TimeCode>* queue)
    {
        if (queue == nullptr)
        {
            return false;
        }

        outpost::rtos::MutexGuard lock(mMutex);
        if (mNumberOfListeners >= numberOfQueues)
        {
            return false;
        }

        mListener[mNumberOfListeners] = queue;
        mNumberOfListeners++;
        return true;
    }

private:
    std::array<outpost::rtos::Queue<TimeCode>*, numberOfQueues> mListener;
    uint32_t mNumberOfListeners;
    outpost::rtos::Mutex mMutex;
};

}  // namespace hal
}  // namespace outpost

#endif /* MODULES_HAL_SRC_OUTPOST_HAL_TIMECODE_DISPATCHER_H_ */
