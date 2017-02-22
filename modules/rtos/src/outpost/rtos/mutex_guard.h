/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_RTOS_MUTEX_GUARD_HPP
#define OUTPOST_RTOS_MUTEX_GUARD_HPP

#include <outpost/rtos/mutex.h>

namespace outpost
{
namespace rtos
{

/**
 * Implements a RAII-style locking.
 *
 * Locks the Mutex when created and unlocks it on destruction.
 *
 * Example:
 *
 *     Mutex mutex;
 *     ...
 *     {
 *         MutexGuard lock(mutex);
 *         // Mutex is locked on creation of lock
 *
 *         ... do something ...
 *
 *         // End of scope for lock => Mutex is unlocked.
 *     }
 *
 * \author    Fabian Greif
 * \ingroup    rtos
 */
template <typename T>
class Guard
{
public:
    /**
     * Acquire the mutex.
     *
     * \param m
     *      The mutex to acquire.
     */
    explicit inline
    Guard(T& m) :
        mLock(m)
    {
        mLock.acquire();
    }

    /**
     * Release the mutex.
     */
    inline
    ~Guard()
    {
        mLock.release();
    }

private:
    // Disable copy constructor
    Guard(const Guard&);

    // Disable assignment operator
    Guard&
    operator=(const Guard& other);

    T& mLock;
};

/**
 * Lock for Mutexes.
 */
typedef Guard<Mutex> MutexGuard;

}
}

#endif
