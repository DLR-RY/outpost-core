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
 * Authors:
 * - 2021, Norbert Toth (DLR RY-AVS)
 */

#include <outpost/rtos.h>
#include <outpost/sip/coordinator/coordinator_packet_receiver.h>
#include <outpost/sip/packet/packet_writer.h>
#include <outpost/sip/packet_coder/packet_coder_hdlc.h>
#include <outpost/sip/packet_transport/packet_transport_serial.h>

#include <unittest/hal/serial_stub.h>
#include <unittest/harness.h>

class TestCoordinatorPacketReceiver : public ::testing::Test
{
public:
    TestCoordinatorPacketReceiver() :
        packetTransportSerial(clock, serial, packetCoderHdlc),
        coordinator(packetTransportSerial),
        coordinatorPacketReceiver(packetTransportSerial,
                                  coordinator,
                                  outpost::support::parameter::HeartbeatSource::default0,
                                  140)
    {
    }

    outpost::rtos::SystemClock clock;

    unittest::hal::SerialStub serial;

    outpost::sip::PacketCoderHdlc packetCoderHdlc;

    outpost::sip::PacketTransportSerial packetTransportSerial;

    outpost::sip::Coordinator coordinator;

    outpost::sip::CoordinatorPacketReceiver coordinatorPacketReceiver;

    uint8_t queueError = 0;
    uint8_t readError = 1;
    uint8_t receiveError = 2;
    uint8_t success = 3;
    uint8_t unknown = 4;

    uint8_t
    testReceivePacket()
    {
        switch (coordinatorPacketReceiver.receivePacket(outpost::time::Seconds(1)))
        {
            case outpost::sip::CoordinatorPacketReceiver::receiveResult::queueError:
                return queueError;
                break;
            case outpost::sip::CoordinatorPacketReceiver::receiveResult::readError:
                return readError;
                break;
            case outpost::sip::CoordinatorPacketReceiver::receiveResult::receiveError:
                return receiveError;
                break;
            case outpost::sip::CoordinatorPacketReceiver::receiveResult::success:
                return success;
                break;
        }
        return unknown;
    }
};

TEST_F(TestCoordinatorPacketReceiver, receiveError)
{
    EXPECT_EQ(testReceivePacket(), receiveError);
}

TEST_F(TestCoordinatorPacketReceiver, readError)
{
    serial.mDataToReceive.clear();
    serial.mDataToReceive.push_back(0x7E);
    serial.mDataToReceive.push_back(0x55);
    serial.mDataToReceive.push_back(0x7E);

    EXPECT_EQ(testReceivePacket(), readError);
}

TEST_F(TestCoordinatorPacketReceiver, success)
{
    uint8_t bufferWriter[outpost::sip::maxPacketLength];
    outpost::Slice<uint8_t> bufferPacketWriter(bufferWriter);
    outpost::sip::PacketWriter packetWriter(bufferPacketWriter);

    packetWriter.setWorkerId(1);
    packetWriter.setCounter(2);
    packetWriter.setType(3);

    uint8_t buffer[256];
    outpost::Slice<uint8_t> bufferToWrite(buffer);
    bufferToWrite = packetWriter.get();

    serial.mDataToReceive.clear();
    serial.mDataToReceive.push_back(0x7E);
    // length
    serial.mDataToReceive.push_back(bufferToWrite[0]);
    serial.mDataToReceive.push_back(bufferToWrite[1]);
    // id
    serial.mDataToReceive.push_back(bufferToWrite[2]);
    // counter
    serial.mDataToReceive.push_back(bufferToWrite[3]);
    // type
    serial.mDataToReceive.push_back(bufferToWrite[4]);
    // CRC
    serial.mDataToReceive.push_back(bufferToWrite[5]);
    serial.mDataToReceive.push_back(bufferToWrite[6]);
    serial.mDataToReceive.push_back(0x7E);

    EXPECT_EQ(testReceivePacket(), success);
}
