/*
 * Copyright (c) 2015-2018, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the Eu:CROPIS flight software.
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
