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

#include "packet_transport_serial.h"

#include <outpost/time/timeout.h>

outpost::sip::PacketTransportSerial::PacketTransportSerial(
        outpost::time::Clock& clock,
        outpost::hal::Serial& serial,
        outpost::sip::PacketCoder& packetCoderIn,
        outpost::time::Duration serialReadTimeoutIn,
        outpost::time::Duration waitForDataSleepTimeIn) :
    mClock(clock),
    mSerial(serial),
    mPacketCoder(packetCoderIn),
    mSerialReadTimeout(serialReadTimeoutIn),
    mWaitForDataSleepTime(waitForDataSleepTimeIn),
    mBuffer{}
{
}

outpost::sip::PacketTransportSerial::~PacketTransportSerial()
{
}

bool
outpost::sip::PacketTransportSerial::transmit(outpost::Slice<const uint8_t> const& packet)
{
    bool result = false;
    outpost::Slice<uint8_t> transmitBuffer(mBuffer);
    if (mPacketCoder.encode(packet, transmitBuffer))
    {
        size_t transmitLength = mSerial.write(transmitBuffer);
        if (transmitLength == transmitBuffer.getNumberOfElements())
        {
            result = true;
        }
    }
    return result;
}

outpost::sip::PacketTransport::receptionResult
outpost::sip::PacketTransportSerial::receive(outpost::Slice<uint8_t>& packet,
                                             outpost::time::Duration timeout)
{
    outpost::Slice<uint8_t> receiveBuffer(mBuffer);
    outpost::Serialize receivedData(receiveBuffer);

    outpost::sip::PacketTransport::receptionResult status =
            outpost::sip::PacketTransport::receptionResult::timeOut;
    bool startBoundary = false;
    bool out = true;

    // timeout
    outpost::time::Timeout timer(mClock, timeout);

    // count the bytes of the buffer
    uint16_t bytesCounter = 0;

    do
    {
        // read one byte
        uint8_t temp[1];
        outpost::Slice<uint8_t> tempSlice = outpost::asSlice(temp);
        size_t bytesRead = mSerial.read(tempSlice, mSerialReadTimeout);
        bytesCounter = bytesCounter + bytesRead;

        if (timer.isExpired(mClock))
        {
            // set timeout
            status = outpost::sip::PacketTransport::receptionResult::timeOut;

            // leave
            out = false;
        }
        else if (bytesCounter > receiveBuffer.getNumberOfElements())
        {
            // mReceiveBuffer is not enough
            // set buffer error
            status = outpost::sip::PacketTransport::receptionResult::bufferError;

            // leave
            out = false;
        }
        else if (bytesRead == 0)
        {
            // not received
            outpost::rtos::Thread::sleep(mWaitForDataSleepTime);
        }
        else if (mPacketCoder.isPacketBoundaryByte(tempSlice[0]) == true)
        {
            // boundary found
            // end boundary
            //-----------------------
            if (startBoundary == true)
            {
                receivedData.store<uint8_t>(tempSlice[0]);

                // decode
                bool result = mPacketCoder.decode(receiveBuffer, packet);

                if (result == true)
                {
                    // set success
                    status = outpost::sip::PacketTransport::receptionResult::success;
                }
                else
                {
                    status = outpost::sip::PacketTransport::receptionResult::decodeError;
                }

                // leave
                out = false;
            }
            else
            {
                // startBoundary
                receivedData.store<uint8_t>(tempSlice[0]);
                startBoundary = true;
            }
        }
        else if (startBoundary == true)
        {
            // data bytes
            receivedData.store<uint8_t>(tempSlice[0]);
        }
    } while (out == true);

    return status;
}
