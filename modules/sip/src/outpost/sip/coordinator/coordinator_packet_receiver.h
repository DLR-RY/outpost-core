/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of the Simple Interface
 * Protocol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *  Authors:
 * - 2021, Norbert Toth (DLR RY-AVS)
 */

#ifndef OUTPOST_SIP_COORDINATOR_PACKET_RECEIVER_H_
#define OUTPOST_SIP_COORDINATOR_PACKET_RECEIVER_H_

#include <outpost/rtos/thread.h>
#include <outpost/sip/coordinator/coordinator.h>
#include <outpost/sip/packet_transport/packet_transport.h>
#include <outpost/support/heartbeat.h>

// for unit test
class TestCoordinatorPacketReceiver;

namespace outpost
{
namespace sip
{
/*
 * Receive responses from the workers.
 * Successfully received responses will be send to a queue.
 * The communication is synchronized, it means if a request is sent than the Coordinator wait for a
 * response with time out. The Coordinator don't send parallel messages. Parallel transaction are
 * not allowed.
 */
class CoordinatorPacketReceiver : public outpost::rtos::Thread
{
    // for unit test
    friend class ::TestCoordinatorPacketReceiver;

public:
    /*
     * Constructs the Packet Receiver of the Coordinator
     *
     *  \param packetTransportIn
     *  	Packet Transport layer of SIP (UART, SpW, ...)
     *  \param coordinatorIn
     *  	Coordinator in order send data to the queue
     *  \param threadPriority
     *  	Priority of the thread
     *  \param stackSize
     *  	Stack size of the thread
     */
    CoordinatorPacketReceiver(outpost::sip::PacketTransport& packetTransportIn,
                              outpost::sip::Coordinator& coordinatorIn,
                              outpost::support::parameter::HeartbeatSource heartbeatSource,
                              uint8_t threadPriority,
                              size_t stackSize = outpost::rtos::Thread::defaultStackSize);

private:
    outpost::sip::PacketTransport& mPacketTransport;
    outpost::sip::Coordinator& mCoordinator;
    outpost::support::parameter::HeartbeatSource mHeartbeatSource;
    uint8_t mBuffer[sip::maxPacketLength];

    static constexpr outpost::time::Duration packetWaitTime = outpost::time::Seconds(1);

    void
    run() override;

    enum class receiveResult
    {
        success,
        readError,
        receiveError,
        queueError
    };

    receiveResult
    receivePacket(outpost::time::Duration timeout);
};
}  // namespace sip
}  // namespace outpost
#endif /* OUTPOST_SIP_COORDINATOR_PACKET_RECEIVER_H_ */
