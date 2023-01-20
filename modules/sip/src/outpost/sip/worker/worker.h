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

#ifndef OUTPOST_SIP_WORKER_H_
#define OUTPOST_SIP_WORKER_H_

#include <outpost/base/slice.h>
#include <outpost/sip/packet_transport/packet_transport.h>
#include <outpost/sip/sip.h>

namespace outpost
{
namespace sip
{
/*
 * Create a response packet and send back to the coordinator.
 */
class Worker
{
public:
    /*
     * Created the Worker
     *
     * /param packetTransportIn
     * 		Packet transport layer for SIP.
     */
    Worker(uint8_t workerIdIn, outpost::sip::PacketTransport& packetTransportIn);

    /*
     * \retval WokrerId
     */
    inline uint8_t
    getId() const
    {
        return mWorkerId;
    }

    /**
     * Send a response back to the Coordinator
     *
     * \param counter
     * 		Packet counter
     *
     * \param type
     * 		Type of the packet
     *
     * \param payloadData
     * 		Payload data to be send
     *
     * \retval true     Data successfully sent
     * \retval false    Data sent failed
     */
    bool
    sendResponse(uint8_t counter,
                 uint8_t type,
                 outpost::Slice<uint8_t> payloadData = outpost::Slice<uint8_t>::empty());

private:
    uint8_t mWorkerId;
    outpost::sip::PacketTransport& mPacketTransport;
    uint8_t mBuffer[outpost::sip::maxPacketLength];
    outpost::Slice<uint8_t> mBufferToWrite;
};
}  // namespace sip
}  // namespace outpost

#endif /* OUTPOST_SIP_WORKER_H_ */
