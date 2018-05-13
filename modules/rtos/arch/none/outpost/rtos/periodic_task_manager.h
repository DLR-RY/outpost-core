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

#ifndef OUTPOST_RTOS_NONE_PERIODIC_TASK_MANAGER_H
#define OUTPOST_RTOS_NONE_PERIODIC_TASK_MANAGER_H

#include <outpost/time/duration.h>

#include <outpost/rtos/mutex.h>
#include <outpost/rtos/failure_handler.h>

namespace outpost
{
    namespace rtos
    {
        /**
         * Helper class for Rate-Monotonic Scheduling (RMS).
         *
         * The POSIX implementation builds upon the clock_nanosleep() method
         * with CLOCK_MONOTONIC as clock source.
         *
         * \author    Fabian Greif
         * \ingroup    rtos
         */
        class PeriodicTaskManager
        {
        public:
            struct Status
            {
                enum Type
                {
                    /// Period has not been started
                    idle,

                    /// Period is currently running
                    running,

                    /// Period has expired
                    timeout
                };
            };

            PeriodicTaskManager();

            ~PeriodicTaskManager();

            /**
             * Start next period.
             *
             * If the PeriodicTaskManager is running, the calling thread will
             * be blocked for the remainder of the outstanding period and,
             * upon completion of that period, the period will be reinitialized
             * with the specified period.
             *
             * If the PeriodicTaskManager is not currently running and has
             * not expired, it is initiated with a length of period ticks and
             * the calling task returns immediately.
             *
             * If the PeriodicTaskManager has expired before the thread invokes
             * the \c nextPeriod method, the period will be initiated with a
             * length of *period* and the calling task returns immediately with
             * a timeout error status.
             *
             * \param  period
             *     Length of the next period. Can be different from the
             *     previous one.
             *
             * \retval    Status::running
             *     Period is currently running.
             * \retval  Status::timeout
             *     Last period was missed, this may require some different
             *     handling from the user.
             */
            Status::Type
            nextPeriod(time::Duration period);

            /**
             * Check the status of the current period.
             *
             * \retval  Status::idle
             *     Period has not been started.
             * \retval    Status::running
             *     Period is currently running.
             * \retval  Status::timeout
             *     Last period was missed, this may require some different
             *     handling from the user.
             */
            Status::Type
            status();

            /**
             * Period measurement is stopped.
             *
             * Can be restarted with the invocation of \c nextPeriod.
             */
            void
            cancel();

        private:
        };
    }
}

#endif
