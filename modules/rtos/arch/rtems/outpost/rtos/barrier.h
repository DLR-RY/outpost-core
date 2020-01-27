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

#include <rtems.h>

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
 * Implemented using RTEMS barrier. Only using the features also provided by POSIX.
 * Not used RTEMS functionality, timeout and manual release barriers.
 *
 * \author    Jan Malburg
 * \ingroup    rtos
 */
class Barrier
{
public:
    /**
     * Create a new Barrier.
     *
     * Make sure the maximum number of allocated barriers
     * is smaller than the number given via \c CONFIGURE_MAXIMUM_BARRIERS.
     *
     * @param numberOfThreads	number of threads that must wait on the barrier for them to continue
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
    ~Barrier();

    /**
     * Waits till the set amount of threads are waiting.
     */
    void
    wait();

private:
    rtems_id mId;
};

}  // namespace rtos
}  // namespace outpost

#endif
