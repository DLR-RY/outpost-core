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

#ifndef OUTPOST_RTOS_POSIX_SEMAPHORE_HPP
#define OUTPOST_RTOS_POSIX_SEMAPHORE_HPP

#include <pthread.h>
#include <semaphore.h>

#include <outpost/time/duration.h>

#include <stdint.h>

namespace outpost
{
namespace rtos
{
/**
 * Counting Semaphore.
 *
 * \author    Fabian Greif
 */
class Semaphore
{
public:
    /**
     * Create a Semaphore.
     *
     * \param count
     *         Initial value for the semaphore.
     */
    explicit Semaphore(uint32_t count);

    // disable copy constructor
    Semaphore(const Semaphore& other) = delete;

    // disable assignment operator
    Semaphore&
    operator=(const Semaphore& other) = delete;

    /**
     * Destroy the semaphore and release it's resources.
     */
    ~Semaphore();

    /**
     * Decrement the count.
     *
     * Blocks if the count is currently zero until it is incremented
     * by another thread calling the release() method.
     */
    inline bool
    acquire()
    {
        return (sem_wait(&mSid) == 0);
    }

    /**
     * Decrement the count.
     *
     * Same a acquire() but abort after \p timeout milliseconds.
     *
     * \param timeout
     *         Timeout in milliseconds.
     *
     * \return    \c true if the semaphore could be successfully acquired,
     *             \c false in case of an error or timeout.
     */
    bool
    acquire(time::Duration timeout);

    /**
     * Decrement the count. Only to be used from within ISRs.
     *
     * \param taskWoken
     *     Is set to true if a higher priority task was woken by the call.
     *     In that case, a yield shout be called before exiting the ISR.
     *
     * \return True if the count is currently greater than zero and the semaphore could be obtained,
     *     false otherwise.
     */
    inline bool
    acquireFromISR(bool& hasWokenTask)
    {
        hasWokenTask = false;
        return acquire();
    }

    /**
     * Increment the count.
     *
     * This function will never block, but may preempt if an other
     * thread waiting for this semaphore has a higher priority.
     */
    inline void
    release()
    {
        sem_post(&mSid);
    }

    /**
     * Decrement the count. Only to be used from within ISRs.
     *
     * This function will never block, but may preempt if an other
     * thread waiting for this semaphore has a higher priority.
     *
     * \param taskWoken
     *     Is set to true if a higher priority task was woken by the call.
     *     In that case, a yield shout be called before exiting the ISR.
     */
    inline void
    releaseFromISR(bool& hasWokenTask)
    {
        release();
        hasWokenTask = false;
    }

private:
    /// POSIX semaphore handle
    sem_t mSid;
};

/**
 * Binary semaphore.
 *
 * Restricts the value of the semaphore to 0 and 1.
 *
 * \author    Fabian Greif
 */
class BinarySemaphore
{
public:
    struct State
    {
        enum Type
        {
            acquired,
            released
        };
    };

    /**
     * Create a binary semaphore in the released state.
     *
     * \param    initial
     *         Initial value of the semaphore.
     */
    BinarySemaphore();

    /**
     * Create a binary semaphore.
     *
     * \param    initial
     *         Initial value of the semaphore.
     */
    explicit BinarySemaphore(State::Type initial);

    // disable copy constructor
    BinarySemaphore(const BinarySemaphore& other) = delete;

    // disable assignment operator
    BinarySemaphore&
    operator=(const BinarySemaphore& other) = delete;

    /**
     * Destroy the semaphore and release it's resources.
     */
    ~BinarySemaphore();

    /**
     * Decrement the count.
     *
     * Blocks if the count is currently zero until it is incremented
     * by another thread calling the release() method.
     */
    bool
    acquire();

    /**
     * Decrement the count.
     *
     * Same a acquire() but abort after \p timeout milliseconds.
     *
     * \param timeout
     *         Timeout in milliseconds.
     *
     * \return    \c true if the semaphore could be successfully acquired,
     *             \c false in case of an error or timeout.
     */
    bool
    acquire(time::Duration timeout);

    /**
     * Increment the count.
     *
     * This function will never block, but may preempt if an other
     * thread waiting for this semaphore has a higher priority.
     */
    void
    release();

private:
    // POSIX handles
    pthread_mutex_t mMutex;
    pthread_cond_t mSignal;
    State::Type mValue;
};

}  // namespace rtos
}  // namespace outpost

#endif
