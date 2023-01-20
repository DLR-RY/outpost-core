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
 *  - 2021, Norbert Toth (DLR RY-AVS)
 */

#include "worker.h"

#include <outpost/sip/packet/packet_writer.h>

outpost::sip::Worker::Worker(uint8_t workerIdIn, outpost::sip::PacketTransport& packetTransportIn) :
    mWorkerId(workerIdIn), mPacketTransport(packetTransportIn), mBuffer{}, mBufferToWrite(mBuffer)
{
}

bool
outpost::sip::Worker::sendResponse(uint8_t counter,
                                   uint8_t type,
                                   outpost::Slice<uint8_t> payloadData)
{
    outpost::sip::PacketWriter packetWriter(mBufferToWrite);
    packetWriter.setWorkerId(mWorkerId);
    packetWriter.setCounter(counter);
    packetWriter.setType(type);
    packetWriter.setPayloadData(payloadData);
    // send response
    return mPacketTransport.transmit(packetWriter.get());
}
