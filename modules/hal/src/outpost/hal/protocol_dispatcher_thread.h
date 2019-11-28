/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef MODULES_HAL_SRC_OUTPOST_HAL_PROTOCOL_DISPATCHER_THREAD_H_
#define MODULES_HAL_SRC_OUTPOST_HAL_PROTOCOL_DISPATCHER_THREAD_H_

#include "protocol_dispatcher_interface.h"
#include "receiver_interface.h"

#include <outpost/base/slice.h>
#include <outpost/rtos.h>
#include <outpost/support/heartbeat.h>
#include <outpost/time/duration.h>

#include <stdint.h>

namespace outpost
{
namespace hal
{
class ProtocolDispatcherThread : public outpost::rtos::Thread
{
public:
    /**
     * @param receiver        the object used to receive packages
     * @param buffer	      buffer for the received packages, should be equal or larger than the
     * largest package that can be received, or data will be dropped, memory shall not used by any
     * other
     * @param priority        see outpost::rtos::Thread
     * @param stackSize       see outpost::rtos::Thread
     * @param threadName      see outpost::rtos::Thread
     * @param heartbeatSource heartbeat id for the worker thread
     * @param waitTime		  Time to wait on a receive
     * @param dispatchTime    Small time addition to insert data into the queues, must be larger
     * than zero)
     */
    ProtocolDispatcherThread(ProtocolDispatcherInterfaceBase& pd,
                             ReceiverInterface& receiver,
                             outpost::Slice<uint8_t> buffer,
                             uint8_t priority,
                             size_t stackSize,
                             char* threadName,
                             outpost::support::parameter::HeartbeatSource heartbeatSource,
                             outpost::time::Duration waitTime = outpost::time::Seconds(10),
                             outpost::time::Duration dispatchTime = outpost::time::Seconds(1)) :
        outpost::rtos::Thread(priority, stackSize, threadName),
        mPD(pd),
        mReceiver(receiver),
        mBuffer(buffer),
        mHeartbeatSource(heartbeatSource),
        mWaitTime(waitTime),
        mDispatchTime(dispatchTime)
    {
    }

protected:
    void
    run() override;

private:
    ProtocolDispatcherInterfaceBase& mPD;

    ReceiverInterface& mReceiver;

    outpost::Slice<uint8_t> mBuffer;

    const outpost::support::parameter::HeartbeatSource mHeartbeatSource;

    const outpost::time::Duration mWaitTime;
    const outpost::time::Duration mDispatchTime;
};

}  // namespace hal
}  // namespace outpost

#endif /* MODULES_HAL_SRC_OUTPOST_HAL_PROTOCOL_DISPATCHER_THREAD_H_ */
