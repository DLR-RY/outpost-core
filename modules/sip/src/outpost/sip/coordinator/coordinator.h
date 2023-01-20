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

#ifndef OUTPOST_SIP_COORDINATOR_H_
#define OUTPOST_SIP_COORDINATOR_H_

#include <outpost/base/slice.h>
#include <outpost/rtos/queue.h>
#include <outpost/sip/packet_transport/packet_transport.h>
#include <outpost/sip/sip.h>

namespace outpost
{
namespace sip
{
/*
 * Send a request to a worker and receive the response from the queue.
 *
 */
class Coordinator
{
public:
    /*
     * Constructs the Coordinator.
     *
     *  \param packetTransportIn
     *  	Packet Transport layer of SIP (UART, SpW, ...)
     */
    explicit Coordinator(outpost::sip::PacketTransport& packetTransportIn);

    enum class requestResults
    {
        success,
        transmitError,
        responseError,
        workerIdError,
        responseTypeError
    };

    /**
     * Send a request to the Worker with data
     *
     * \param workerId
     *      The ID of worker
     *
     * \param counter
     * 		Packet counter
     *
     * \param type
     * 		Type of the packet
     *
     * \param expectedResponseType
     * 		Expected response packet type
     *
     * \param sendData
     * 		Payload data to be send
     *
     * \retval success
     * 		Success
     * \retval transmitError
     * 		Transmit failed
     * \retval responseError
     * 		Response time out
     * \retval workerIdError
     * 		Wrong worker ID in the response
     * \retval responseTypeError
     * 		Wrong response Type
     */
    requestResults
    sendRequest(uint8_t workerId,
                uint8_t counter,
                uint8_t type,
                uint8_t expectedResponseType,
                outpost::Slice<uint8_t> sendData = outpost::Slice<uint8_t>::empty());

    /**
     * Send a request to the Worker and get response data
     *
     * \param workerId
     *      The ID of worker
     *
     * \param counter
     * 		Packet counter
     *
     * \param type
     * 		Type of the packet
     *
     * \param expectedResponseType
     * 		Expected response packet type
     *
     * \param workerResponseData
     * 		Payload data to be read
     *
     * \retval success
     * 		Success
     * \retval transmitError
     * 		Transmit failed
     * \retval responseError
     * 		Response time out
     * \retval workerIdError
     * 		Wrong worker ID in the response
     * \retval responseTypeError
     * 		Wrong response Type
     */
    requestResults
    sendRequestGetResponseData(uint8_t workerId,
                               uint8_t counter,
                               uint8_t type,
                               uint8_t expectedResponseType,
                               outpost::Slice<uint8_t>& workerResponseData);

    struct ResponseData
    {
        uint16_t length;
        uint8_t workerId;
        uint8_t counter;
        uint8_t type;
        uint16_t payloadDataLength;
        uint8_t payloadData[outpost::sip::maxPayloadLength];
    };

    /*
     * Send response data to the queue.
     *
     * \param data
     * 		Response data to be send to the queue
     *
     * \retval true
     * 		Success
     * \retval false
     * 		failed
     */
    inline bool
    sendResponseQueue(ResponseData data)
    {
        return mResponseQueue.send(data);
    }

private:
    outpost::sip::PacketTransport& mPacketTransport;
    uint8_t mBuffer[outpost::sip::maxPacketLength];
    outpost::Slice<uint8_t> mBufferToWrite;

    // Parallel transaction are not allowed therefore for the queue is enough to hold one response.
    const static uint8_t sizeOfQueue = 1;
    outpost::rtos::Queue<ResponseData> mResponseQueue;

    inline bool
    receiveResponseQueue(ResponseData& data, outpost::time::Duration timeout)
    {
        return mResponseQueue.receive(data, timeout);
    }
};

}  // namespace sip
}  // namespace outpost
#endif /* OUTPOST_SIP_COORDINATOR_H_ */
