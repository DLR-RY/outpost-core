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
 * - 2015, Jan Sommer (DLR SC-SRV)
 */

#ifndef OUTPOST_RTOS_POSIX_MUTEX_HPP
#define OUTPOST_RTOS_POSIX_MUTEX_HPP

#include <pthread.h>

#include <outpost/time/duration.h>

namespace outpost
{
namespace rtos
{
/**
 * Mutex
 *
 * \author    Fabian Greif
 */
class Mutex
{
public:
    Mutex();

    // disable copy constructor
    Mutex(const Mutex& other) = delete;

    // disable assignment operator
    Mutex&
    operator=(const Mutex& other) = delete;

    inline ~Mutex()
    {
        pthread_mutex_destroy(&mMutex);
    }

    /**
     * Acquire the mutex.
     *
     * This function may block if the mutex is currently held by an
     * other thread.
     *
     * \returns    \c true if the mutex could be acquired.
     */
    inline bool
    acquire()
    {
        return (pthread_mutex_lock(&mMutex) == 0);
    }

    /**
     * Acquire the mutex.
     *
     * Same as acquire() but blocks only for \p timeout milliseconds.
     *
     * \param    timeout
     *         Timeout in milliseconds.
     *
     * \return    \c true if the mutex could be acquired, \c false in
     *             case of an error or timeout.
     */
    bool
    acquire(::outpost::time::Duration timeout);

    /**
     * Release the mutex.
     */
    inline void
    release()
    {
        pthread_mutex_unlock(&mMutex);
    }

private:
    pthread_mutex_t mMutex;
};

}  // namespace rtos
}  // namespace outpost

#endif
