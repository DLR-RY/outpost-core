/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_RTOS_RTEMS_BARRIER_HPP
#define OUTPOST_RTOS_RTEMS_BARRIER_HPP

#include "mutex"
#include "semaphore.h"

#include <stdint.h>

namespace outpost
{
namespace rtos
{
/**
 * Barrier class.
 *
 * Barrier are used to ensure that a set of threads are at a defined position before continuing
 *
 * Implementation using Semaphore and mutex and no freertos specific part directly, hence, as such
 * is a general implementation.
 *
 * \author    Jan Malburg
 * \ingroup    rtos
 */
class Barrier
{
public:
    /**
     * Create a new Barrier. Parameter here for same interface with versions for other OSs.
     */
    Barrier(uint32_t numberOfThreads);

    // disable copy constructor
    Barrier(const Barrier& other) = delete;

    // disable assignment operator
    Barrier&
    operator=(const Barrier& other) = delete;

    /**
     * Destroy the Barrier and free it's resources.
     */
    ~Barrier() = default;

    /**
     * Waits till the set amount of threads are waiting.
     */
    void
    wait();

private:
    Mutex mCounterMutex;
    BinarySemaphore mWaitSemaphore;
    BinarySemaphore mReleaseSemaphore;

    uint32_t mMaxWaiting;
    uint32_t mCurrentlyWaiting;
};

}  // namespace rtos
}  // namespace outpost

#endif
