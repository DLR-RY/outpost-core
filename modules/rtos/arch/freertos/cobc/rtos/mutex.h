/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_FREERTOS_MUTEX_HPP
#define COBC_RTOS_FREERTOS_MUTEX_HPP

#include <stdint.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <cobc/time/duration.h>

namespace cobc
{
    namespace rtos
    {
        /**
         * Mutex class.
         *
         * Mutexes can be used to enforce mutual exclusion for a critical
         * section in user code.
         *
         * \author    Fabian Greif
         * \ingroup    rtos
         */
        class Mutex
        {
        public:
            /**
             * Create a new Mutex.
             *
             * The mutex is released after creation.
             */
            Mutex();

            /**
             * Destroy the mutex and free it's resources.
             */
            ~Mutex();

            /**
             * Acquire the mutex.
             *
             * This function may block if the mutex is currently held by an
             * other thread.
             *
             * \returns    \c true if the mutex could be acquired.
             */
            bool
            acquire();

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
            acquire(time::Duration timeout);

            /**
             * Release the mutex.
             *
             * This function will never block.
             */
            void
            release();

        private:
            // disable copy constructor
            Mutex(const Mutex& other);

            // disable assignment operator
            Mutex&
            operator=(const Mutex& other);

            void* handle;
        };
    }
}

#endif // COBC_RTOS_FREERTOS_MUTEX_HPP
