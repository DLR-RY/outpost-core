/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#ifndef COBC_RTOS_RTEMS_PERIODIC_TASK_MANAGER_H
#define COBC_RTOS_RTEMS_PERIODIC_TASK_MANAGER_H

#include "rtems/interval.h"

#include <outpost/rtos/failure_handler.h>

namespace outpost
{
namespace rtos
{

/**
 * Helper class for Rate-Monotonic Scheduling (RMS).
 *
 * \author  Fabian Greif
 * \ingroup rtos
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

    /**
     * Create a new periodic task manager.
     *
     * The periodic task manager has to be created in the thread which will
     * be used later to call the nextPeriod function.
     */
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
     * \warning
     *      The nextPeriod function must only be called by the thread
     *      that create the PeriodicTaskManager object.
     *
     * \param  period
     *      Length of the next period. Can be different from the
     *      previous one.
     *
     * \retval    Status::running
     *      Period is currently running.
     * \retval  Status::timeout
     *      Last period was missed, this may require some different
     *      handling from the user.
     */
    inline Status::Type
    nextPeriod(time::Duration period)
    {
        rtems_status_code result = rtems_rate_monotonic_period(mId, rtems::getInterval(period));
        return static_cast<Status::Type>(result);
    }

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
    inline Status::Type
    status()
    {
        rtems_status_code result = rtems_rate_monotonic_period(mId, RTEMS_PERIOD_STATUS);
        return static_cast<Status::Type>(result);
    }

    /**
     * Period measurement is stopped.
     *
     * Can be restarted with the invocation of \c nextPeriod.
     */
    inline void
    cancel()
    {
        rtems_rate_monotonic_cancel(mId);
    }

private:
    rtems_id mId;
};

}
}

#endif // COBC_RTOS_RTEMS_PERIODIC_TASK_MANAGER_H