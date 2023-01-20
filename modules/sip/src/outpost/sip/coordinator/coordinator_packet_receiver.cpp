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

#include <outpost/sip/coordinator/coordinator_packet_receiver.h>
#include <outpost/sip/packet/packet_reader.h>

constexpr outpost::time::Duration outpost::sip::CoordinatorPacketReceiver::packetWaitTime;

outpost::sip::CoordinatorPacketReceiver::CoordinatorPacketReceiver(
        outpost::sip::PacketTransport& packetTransportIn,
        outpost::sip::Coordinator& coordinatorIn,
        outpost::support::parameter::HeartbeatSource heartbeatSource,
        uint8_t threadPriority,
        size_t stackSize) :
    outpost::rtos::Thread(threadPriority, stackSize, "CooReceiver"),
    mPacketTransport(packetTransportIn),
    mCoordinator(coordinatorIn),
    mHeartbeatSource(heartbeatSource),
    mBuffer{}
{
}

outpost::sip::CoordinatorPacketReceiver::receiveResult
outpost::sip::CoordinatorPacketReceiver::receivePacket(outpost::time::Duration timeout)
{
    outpost::Slice<uint8_t> bufferToRead(mBuffer);
    // wait for incoming response
    if (mPacketTransport.receive(bufferToRead, timeout)
        == outpost::sip::PacketTransport::receptionResult::success)
    {
        outpost::sip::PacketReader packetReader(bufferToRead);

        if (packetReader.readPacket() == outpost::sip::PacketReader::readResults::success)
        {
            outpost::sip::Coordinator::ResponseData data;
            data.length = packetReader.getLength();
            data.workerId = packetReader.getWorkerId();
            data.counter = packetReader.getCounter();
            data.type = packetReader.getType();
            // copy payload data
            outpost::Slice<uint8_t> payloadData(packetReader.getPayloadData());
            data.payloadDataLength = payloadData.getNumberOfElements();
            memcpy(data.payloadData, payloadData.getDataPointer(), data.payloadDataLength);

            // send response to the queue
            bool result = mCoordinator.sendResponseQueue(data);
            if (result == false)
            {
                // send to Queue: failed
                return outpost::sip::CoordinatorPacketReceiver::receiveResult::queueError;
            }
            else
            {
                return outpost::sip::CoordinatorPacketReceiver::receiveResult::success;
            }
        }
        else
        {
            // read failed
            return outpost::sip::CoordinatorPacketReceiver::receiveResult::readError;
        }
    }
    else
    {
        // receive failed
        return outpost::sip::CoordinatorPacketReceiver::receiveResult::receiveError;
    }
}

void
outpost::sip::CoordinatorPacketReceiver::run()
{
    while (1)
    {
        outpost::support::Heartbeat::send(
                mHeartbeatSource, packetWaitTime + outpost::support::parameter::heartbeatTolerance);

        receivePacket(packetWaitTime);
    }
}
