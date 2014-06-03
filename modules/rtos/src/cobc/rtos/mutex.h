/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_MUTEX_HPP
#define COBC_RTOS_MUTEX_HPP

#include "detect.h"

#if COBC_RTOS == COBC_RTOS_NONE
#    include "none/mutex.h"
#elif COBC_RTOS == COBC_RTOS_RTEMS
#    include "rtems/mutex.h"
#elif COBC_RTOS == COBC_RTOS_FREERTOS
#    include "freertos/mutex.h"
#elif COBC_RTOS == COBC_RTOS_POSIX
#    include "posix/mutex.h"
#endif

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
             *         The mutex to acquire.
             */
            inline MutexGuard(Mutex& m) :
                mutex(m)
            {
                mutex.acquire();
            }

            /**
             * Release the mutex.
             */
            inline ~MutexGuard()
            {
                mutex.release();
            }

        private:
            Mutex& mutex;
        };
    }
}

#endif // COBC_RTOS_MUTEX_HPP
