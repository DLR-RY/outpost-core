/*
 * Copyright (c) 2015-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2018, Fabian Greif (DLR RY-AVS)
 * - 2016, Jan Sommer (DLR SC-SRV)
 * - 2017, Olaf Maibaum (DLR SC-SRV)
 * - 2018, Annika Ofenloch (DLR RY-AVS)
 */

#ifndef OUTPOST_SUPPORT_HEARTBEAT_H
#define OUTPOST_SUPPORT_HEARTBEAT_H

#include <outpost/parameter/support.h>
#include <outpost/smpc/topic.h>
#include <outpost/time/time_epoch.h>
#include <outpost/time/time_point.h>

namespace outpost
{
namespace support
{
/**
 * Support class for the generation of thread heartbeats.
 *
 * The heartbeats are used to detect dead-locks and check that all threads are
 * still running. The threads protected by the heartbeats must send ticks through
 * the heartbeat topic in regular intervals.
 *
 * ## Timing
 *
 * As different threads have different timing constraints, the
 * heartbeat timing is defined by three durations:
 *
 * 1. Heartbeat interval
 * 2. Processing time
 * 3. Heartbeat tolerance
 *
 * The *heartbeat interval* defines the minimal time between two heartbeat
 * ticks. E.g. when a threads waits on a resource this would be the polling
 * interval. A default value is defined per project, but can be overwritten
 * for each thread.
 *
 * The *processing time* defines how long a thread might take to finish
 * the processing of its current task. This is different per thread and might
 * even be different per task the thread is processing.
 *
 * The *heartbeat tolerance* allows for some jitter in the processing time. In
 * a best-effort system this value can/should be higher than in a system with
 * tight real-time requirements. The value is defined by project and is always
 * added to the processing time.
 *
 * A typical example would be:
 * \code
 * while (1)
 * {
 *     Heartbeat::send(HeartbeatSource::task,
 *                     heartbeatInterval + heartbeatTolerance);
 *     if (newTaskSemaphore.acquire(heartbeatInterval))
 *     {
 *         while (hasNextTask())
 *         {
 *             Heartbeat::send(HeartbeatSource::task,
 *                             processingTime + heartbeatTolerance);
 *             doNextTask();
 *         }
 *     }
 * }
 * \endcode
 *
 * If the thread only processes one task this might also be collapsed to:
 * \code
 * while (1)
 * {
 *     Heartbeat::send(HeartbeatSource::task,
 *                     heartbeatInterval + processingTime + heartbeatTolerance);
 *     if (newTaskSemaphore.acquire(heartbeatInterval))
 *     {
 *         // process task in less or equal than processingTime
 *         doTask();
 *     }
 * }
 * \endcode
 */
struct Heartbeat
{
    enum class TimeoutType
    {
        relativeTime,
        absoluteTime
    };

    outpost::support::parameter::HeartbeatSource mSource;
    TimeoutType mTimeoutType;
    outpost::time::SpacecraftElapsedTime mTimeout;

    /**
     * Send heartbeat tick.
     *
     * \param   source
     *      Thread for which the heartbeat tick is reported.
     * \param   timeToNextTick
     *      Time until which the next tick will be reported.
     */
    static inline void
    send(outpost::support::parameter::HeartbeatSource source,
         outpost::time::Duration timeToNextTick);

    /**
     * Send heartbeat tick.
     *
     * \param   source
     *      Thread for which the heartbeat tick is reported.
     * \param   nextTick
     *      Absolute time point until which the next tick will be reported.
     */
    static inline void
    send(outpost::support::parameter::HeartbeatSource source,
         outpost::time::SpacecraftElapsedTime nextTick);

    /**
     * Suspend the generation of heartbeats.
     *
     * This can be used if the thread e.g. waits for the switch-on of
     * a device. Should be used with caution, the better option is to have
     * continuous heartbeat signals, e.g. by using timeouts while waiting.
     *
     * The suspension can be released by sending a normal heartbeat tick.
     */
    static inline void
    suspend(outpost::support::parameter::HeartbeatSource source);
};

extern outpost::smpc::Topic<const Heartbeat> watchdogHeartbeat;

// Implementation of inline functions
void
Heartbeat::send(outpost::support::parameter::HeartbeatSource source,
                outpost::time::Duration timeToNextTick)
{
    Heartbeat trigger{source,
                      TimeoutType::relativeTime,
                      outpost::time::SpacecraftElapsedTime::afterEpoch(timeToNextTick)};
    watchdogHeartbeat.publish(trigger);
}

void
Heartbeat::send(outpost::support::parameter::HeartbeatSource source,
                outpost::time::SpacecraftElapsedTime nextTick)
{
    Heartbeat trigger{source, TimeoutType::absoluteTime, nextTick};
    watchdogHeartbeat.publish(trigger);
}

void
Heartbeat::suspend(outpost::support::parameter::HeartbeatSource source)
{
    // The heartbeat is suspended by setting the next time to
    // the highest possible relative time.
    Heartbeat trigger{
            source,
            TimeoutType::relativeTime,
            outpost::time::SpacecraftElapsedTime::afterEpoch(outpost::time::Duration::maximum())};
    watchdogHeartbeat.publish(trigger);
}

}  // namespace support
}  // namespace outpost

#endif
