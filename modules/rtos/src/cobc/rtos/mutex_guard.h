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
class MutexGuard
{
public:
    /**
     * Acquire the mutex.
     *
     * \param m
     *      The mutex to acquire.
     */
    explicit inline
    MutexGuard(Mutex& m) :
        mutex(m)
    {
        mutex.acquire();
    }

    /**
     * Release the mutex.
     */
    inline
    ~MutexGuard()
    {
        mutex.release();
    }

private:
    // Disable copy constructor
    MutexGuard(const MutexGuard&);

    // Disable assignment operator
    MutexGuard&
    operator=(const MutexGuard& other);

    Mutex& mutex;
};

}
}

#endif // COBC_RTOS_MUTEX_GUARD_HPP
