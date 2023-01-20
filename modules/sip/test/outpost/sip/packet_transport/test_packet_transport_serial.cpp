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
#include <outpost/sip/packet/packet_writer.h>
#include <outpost/sip/packet_coder/packet_coder_hdlc.h>
#include <outpost/sip/packet_transport/packet_transport_serial.h>

#include <unittest/hal/serial_stub.h>
#include <unittest/harness.h>

class TestPacketTransportSerial : public ::testing::Test
{
public:
    TestPacketTransportSerial() : packetTransportSerial(clock, serial, packetCoderHdlc)
    {
    }

    outpost::rtos::SystemClock clock;

    unittest::hal::SerialStub serial;

    outpost::sip::PacketCoderHdlc packetCoderHdlc;

    outpost::sip::PacketTransportSerial packetTransportSerial;
};

TEST_F(TestPacketTransportSerial, transmit)
{
    uint8_t bufferWriter[outpost::sip::maxPacketLength];
    outpost::Slice<uint8_t> bufferPacketWriter(bufferWriter);
    outpost::sip::PacketWriter packetWriter(bufferPacketWriter);

    packetWriter.setWorkerId(1);
    packetWriter.setCounter(2);
    packetWriter.setType(3);

    uint8_t payloadBuffer[2];
    outpost::Slice<uint8_t> payload(payloadBuffer);
    payload[0] = 55;
    payload[1] = 66;
    packetWriter.setPayloadData(payload);

    EXPECT_TRUE(packetTransportSerial.transmit(packetWriter.get()));

    EXPECT_EQ(serial.mDataToTransmit[0], 0x7E);
    // length
    EXPECT_EQ(serial.mDataToTransmit[1], 0);
    EXPECT_EQ(serial.mDataToTransmit[2], 7);
    // worker id
    EXPECT_EQ(serial.mDataToTransmit[3], 1);
    // counter
    EXPECT_EQ(serial.mDataToTransmit[4], 2);
    // type
    EXPECT_EQ(serial.mDataToTransmit[5], 3);
    // payload
    EXPECT_EQ(serial.mDataToTransmit[6], 55);
    EXPECT_EQ(serial.mDataToTransmit[7], 66);

    EXPECT_EQ(serial.mDataToTransmit[10], 0x7E);
}

TEST_F(TestPacketTransportSerial, receiveTimeout)
{
    // null should be read out
    serial.mDataToReceive.clear();

    uint8_t buffer[256];
    outpost::Slice<uint8_t> receiveBuffer(buffer);
    outpost::sip::PacketTransport::receptionResult result =
            packetTransportSerial.receive(receiveBuffer, outpost::time::Milliseconds(100));

    EXPECT_EQ(result, outpost::sip::PacketTransport::receptionResult::timeOut);
}

TEST_F(TestPacketTransportSerial, receiveSuccess)
{
    uint8_t bufferWriter[outpost::sip::maxPacketLength];
    outpost::Slice<uint8_t> bufferPacketWriter(bufferWriter);
    outpost::sip::PacketWriter packetWriter(bufferPacketWriter);

    packetWriter.setWorkerId(1);
    packetWriter.setCounter(2);
    packetWriter.setType(3);

    uint8_t payloadBuffer[2];
    outpost::Slice<uint8_t> payload(payloadBuffer);
    payload[0] = 55;
    payload[1] = 66;
    packetWriter.setPayloadData(payload);

    uint8_t encodeBuffer[256];
    outpost::Slice<uint8_t> encode(encodeBuffer);
    EXPECT_TRUE(packetCoderHdlc.encode(packetWriter.get(), encode));

    serial.mDataToReceive.clear();
    serial.mDataToReceive.push_back(0x7E);
    serial.mDataToReceive.push_back(0x55);
    serial.mDataToReceive.push_back(0x7E);

    uint8_t bufferRet[256];
    outpost::Slice<uint8_t> receiveBuffer(bufferRet);
    outpost::sip::PacketTransport::receptionResult result =
            packetTransportSerial.receive(receiveBuffer, outpost::time::Milliseconds(1));

    EXPECT_EQ(result, outpost::sip::PacketTransport::receptionResult::success);
    EXPECT_EQ(receiveBuffer[0], 0x55);
}
