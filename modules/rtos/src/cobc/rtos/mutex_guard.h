/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#ifndef COBC_RTOS_MUTEX_GUARD_HPP
#define COBC_RTOS_MUTEX_GUARD_HPP

#include <cobc/rtos/mutex.h>

namespace cobc
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
