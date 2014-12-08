/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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

#ifndef COBC_RTOS_POSIX_SEMAPHORE_HPP
#define COBC_RTOS_POSIX_SEMAPHORE_HPP

#include <stdint.h>

#include <semaphore.h>
#include <pthread.h>

#include <cobc/time/duration.h>

namespace cobc
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
        return (sem_wait(&sid) == 0);
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
     * Increment the count.
     *
     * This function will never block, but may preempt if an other
     * thread waiting for this semaphore has a higher priority.
     */
    inline void
    release()
    {
        sem_post(&sid);
    }

private:
    // disable copy constructor
    Semaphore(const Semaphore& other);

    // disable assignment operator
    Semaphore&
    operator=(const Semaphore& other);

    /// POSIX semaphore handle
    sem_t sid;
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
     * Create a binary semaphore.
     *
     * \param    initial
     *         Initial value of the semaphore.
     */
    explicit BinarySemaphore(State::Type initial);

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
    // disable copy constructor
    BinarySemaphore(const BinarySemaphore& other);

    // disable assignment operator
    BinarySemaphore&
    operator=(const BinarySemaphore& other);

    // POSIX handles
    pthread_mutex_t mutex;
    pthread_cond_t signal;
    State::Type value;
};

}
}

#endif // COBC_RTOS_POSIX_SEMAPHORE_HPP
