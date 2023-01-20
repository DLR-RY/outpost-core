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

#ifndef OUTPOST_SIP_PACKET_TRANSPORT_SERIAL_H
#define OUTPOST_SIP_PACKET_TRANSPORT_SERIAL_H

#include "packet_transport.h"

#include <outpost/hal/serial.h>
#include <outpost/rtos/thread.h>
#include <outpost/sip/packet_coder/packet_coder.h>
#include <outpost/sip/sip.h>
#include <outpost/time/clock.h>
#include <outpost/utils/storage/serialize.h>

namespace outpost
{
namespace sip
{
/**
 * Transmitting and receiving of a packet through a serial connection.
 */
class PacketTransportSerial : public PacketTransport
{
public:
    /*
     * Creates the packet transport serial
     *
     * \param clock
     * 		Clock for timeout and sleep
     * \param serial
     * 		Serial interface to receive data
     * \param packetCoderIn
     * 		Packet coder (HDLC, ...)
     * \param serialReadTimeoutIn
     * 		Time out for serial read
     * \param waitForDataSleep
     * 		Sleep time for wait new data
     */
    PacketTransportSerial(
            outpost::time::Clock& clock,
            outpost::hal::Serial& serial,
            outpost::sip::PacketCoder& packetCoderIn,
            outpost::time::Duration serialReadTimeoutIn = outpost::time::Milliseconds(10),
            outpost::time::Duration waitForDataSleepTimeIn = outpost::time::Milliseconds(10));

    virtual ~PacketTransportSerial();

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
    bool
    transmit(outpost::Slice<const uint8_t> const& packet) override;

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
    receptionResult
    receive(outpost::Slice<uint8_t>& packet, outpost::time::Duration timeout) override;

    // margin for HDLC -> at least 2 bytes (0x7E) boundary
    const static uint8_t hdlcMargin = 10;

private:
    outpost::time::Clock& mClock;
    outpost::hal::Serial& mSerial;
    outpost::sip::PacketCoder& mPacketCoder;
    outpost::time::Duration mSerialReadTimeout;
    outpost::time::Duration mWaitForDataSleepTime;

    uint8_t mBuffer[sip::maxPacketLength + sip::PacketTransportSerial::hdlcMargin];
};

}  // namespace sip
}  // namespace outpost

#endif
