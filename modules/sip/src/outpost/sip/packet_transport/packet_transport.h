/*
 * Copyright (c) 2015-2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of the Simple Interface
 * Protocol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2018, Fabian Greif (DLR RY-AVS)
 * - 2020, Thomas Firchau (DLR RY-AVS)
 * - 2021, Norbert Toth (DLR RY-AVS)
 */

#ifndef OUTPOST_SIP_PACKET_TRANSPORT_H
#define OUTPOST_SIP_PACKET_TRANSPORT_H

#include <outpost/base/slice.h>
#include <outpost/time/timeout.h>

namespace outpost
{
namespace sip
{
/**
 * Transmitting and receiving of a packet.
 *
 */
class PacketTransport
{
public:
    enum class receptionResult
    {
        // Successful reception
        success,
        decodeError,
        bufferError,
        // No packet received within the given time limit.
        timeOut
    };

    virtual ~PacketTransport() = default;

    /**
     * Transmit a packet.
     *
     * \ param packet
     * 		Packet to be send
     *
     * \retval true
     * 		Success
     * \retval false
     * 		Failed
     *
     */
    virtual bool
    transmit(outpost::Slice<const uint8_t> const& packet) = 0;

    /**
     * Receive a packet.
     *
     * This a synchronous operation, the function will only return after receiving
     * a packet or reaching a timeout.
     *
     * \param packet
     * 		Packet receive
     * \param timeout
     * 		Time out
     *
     * \retval success
     * 		Success
     * \retval decodeError
     * 		PacketCoder decode error
     * \retval bufferError
     * 		Buffer size error
     * \retval timeOut
     * 		Time out occur
     *
     */
    virtual receptionResult
    receive(outpost::Slice<uint8_t>& packet, outpost::time::Duration timeout) = 0;
};

}  // namespace sip
}  // namespace outpost
#endif
