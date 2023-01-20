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

#include "coordinator.h"

#include <outpost/sip/packet/packet_reader.h>
#include <outpost/sip/packet/packet_writer.h>
#include <outpost/sip/sip.h>

outpost::sip::Coordinator::Coordinator(outpost::sip::PacketTransport& packetTransportIn) :
    mPacketTransport(packetTransportIn),
    mBuffer{},
    mBufferToWrite(mBuffer),
    mResponseQueue(sizeOfQueue)
{
}

outpost::sip::Coordinator::requestResults
outpost::sip::Coordinator::sendRequest(uint8_t workerId,
                                       uint8_t counter,
                                       uint8_t type,
                                       uint8_t expectedResponseType,
                                       outpost::Slice<uint8_t> sendData)
{
    // packet writer
    outpost::sip::PacketWriter packetWriter(mBufferToWrite);

    packetWriter.setWorkerId(workerId);
    packetWriter.setCounter(counter);
    packetWriter.setType(type);
    packetWriter.setPayloadData(sendData);

    outpost::Slice<uint8_t> dataToSend = packetWriter.get();

    // send request
    if (mPacketTransport.transmit(dataToSend) == false)
    {
        return outpost::sip::Coordinator::requestResults::transmitError;
    }

    // check response
    ResponseData data;
    if (receiveResponseQueue(data, outpost::time::Seconds(2)) == false)
    {
        return outpost::sip::Coordinator::requestResults::responseError;
    }

    // check worker id
    if (data.workerId != workerId)
    {
        return outpost::sip::Coordinator::requestResults::workerIdError;
    }

    // check response type
    if (data.type != expectedResponseType)
    {
        return outpost::sip::Coordinator::requestResults::responseTypeError;
    }

    return outpost::sip::Coordinator::requestResults::success;
}

outpost::sip::Coordinator::requestResults
outpost::sip::Coordinator::sendRequestGetResponseData(uint8_t workerId,
                                                      uint8_t counter,
                                                      uint8_t type,
                                                      uint8_t expectedResponseType,
                                                      outpost::Slice<uint8_t>& workerResponseData)
{
    // packet writer
    outpost::sip::PacketWriter packetWriter(mBufferToWrite);

    packetWriter.setWorkerId(workerId);
    packetWriter.setCounter(counter);
    packetWriter.setType(type);

    outpost::Slice<uint8_t> dataToSend = packetWriter.get();

    // send request
    if (mPacketTransport.transmit(dataToSend) == false)
    {
        return outpost::sip::Coordinator::requestResults::transmitError;
    }

    // check response
    ResponseData data;
    if (receiveResponseQueue(data, outpost::time::Seconds(2)) == false)
    {
        return outpost::sip::Coordinator::requestResults::responseError;
    }

    // check worker id
    if (data.workerId != workerId)
    {
        return outpost::sip::Coordinator::requestResults::workerIdError;
    }

    // check response type
    if (data.type != expectedResponseType)
    {
        return outpost::sip::Coordinator::requestResults::responseTypeError;
    }

    // get worker data
    for (uint16_t i = 0; i < data.payloadDataLength; i++)
    {
        workerResponseData[i] = data.payloadData[i];
    }

    return outpost::sip::Coordinator::requestResults::success;
}
