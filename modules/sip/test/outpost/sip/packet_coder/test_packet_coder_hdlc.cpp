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

#include <outpost/sip/packet/packet_reader.h>
#include <outpost/sip/packet/packet_writer.h>
#include <outpost/sip/packet_coder/packet_coder_hdlc.h>

#include <unittest/harness.h>

class TestPacketCoderHdlc : public ::testing::Test
{
public:
    TestPacketCoderHdlc()
    {
    }

    outpost::sip::PacketCoderHdlc mCoder;
};

TEST_F(TestPacketCoderHdlc, encode)
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

    uint8_t encodeBuffer[256];
    outpost::Slice<uint8_t> output(encodeBuffer);
    EXPECT_TRUE(mCoder.encode(bufferToWrite, output));

    // boundary
    EXPECT_EQ(output[0], 0x7E);
    // length
    EXPECT_EQ(output[1], 0x00);
    EXPECT_EQ(output[2], 0x05);
    // check unitId
    ASSERT_EQ(output[3], 1);
    // check counter
    ASSERT_EQ(output[4], 2);
    // check type
    ASSERT_EQ(output[5], 3);
    // boundary
    EXPECT_EQ(output[8], 0x7E);
}

TEST_F(TestPacketCoderHdlc, encodeDoubleWriter)
{
    // no payload
    uint8_t bufferWriter[outpost::sip::maxPacketLength];
    outpost::Slice<uint8_t> bufferPacketWriter(bufferWriter);
    outpost::sip::PacketWriter packetWriter(bufferPacketWriter);
    uint8_t encodeBuffer[256];
    outpost::Slice<uint8_t> output(encodeBuffer);

    packetWriter.setWorkerId(1);
    packetWriter.setCounter(2);
    packetWriter.setType(3);
    EXPECT_TRUE(mCoder.encode(packetWriter.get(), output));

    // boundary
    EXPECT_EQ(output[0], 0x7E);
    // length
    EXPECT_EQ(output[1], 0x00);
    EXPECT_EQ(output[2], 0x05);
    // check unitId
    ASSERT_EQ(output[3], 1);
    // check counter
    ASSERT_EQ(output[4], 2);
    // check type
    ASSERT_EQ(output[5], 3);
    // boundary
    EXPECT_EQ(output[8], 0x7E);

    // with payload
    outpost::Slice<uint8_t> output2(encodeBuffer);
    packetWriter.setWorkerId(1);
    packetWriter.setCounter(2);
    packetWriter.setType(3);
    uint8_t payloadBuffer[1];
    outpost::Slice<uint8_t> payload(payloadBuffer);
    payload[0] = 55;
    packetWriter.setPayloadData(payload);
    EXPECT_TRUE(mCoder.encode(packetWriter.get(), output2));

    // boundary
    EXPECT_EQ(output[0], 0x7E);
    // length
    EXPECT_EQ(output[1], 0x00);
    EXPECT_EQ(output[2], 0x06);
    // check unitId
    ASSERT_EQ(output[3], 1);
    // check counter
    ASSERT_EQ(output[4], 2);
    // check type
    ASSERT_EQ(output[5], 3);
    // check payload
    ASSERT_EQ(output[6], 55);
    // boundary
    EXPECT_EQ(output[9], 0x7E);
}

TEST_F(TestPacketCoderHdlc, decodeSuccess)
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

    uint8_t outputEncodeBuffer[256];
    outpost::Slice<uint8_t> outputEncode(outputEncodeBuffer);
    EXPECT_TRUE(mCoder.encode(packetWriter.get(), outputEncode));

    uint8_t outputDecodeBuffer[256];
    outpost::Slice<uint8_t> outputDecode(outputDecodeBuffer);
    bool result = mCoder.decode(outputEncode, outputDecode);

    EXPECT_TRUE(result);
}

TEST_F(TestPacketCoderHdlc, decodeWithSliceSuccess)
{
    uint8_t outputEncodeBuffer[256];
    outpost::Slice<uint8_t> outputEncode(outputEncodeBuffer);

    outputEncode[0] = 0x7E;
    outputEncode[1] = 0x55;
    outputEncode[2] = 0x7E;

    uint8_t outputDecodeBuffer[256];
    outpost::Slice<uint8_t> outputDecode(outputDecodeBuffer);
    bool result = mCoder.decode(outputEncode, outputDecode);

    EXPECT_TRUE(result);

    EXPECT_EQ(outputEncode[1], outputDecode[0]);
}

TEST_F(TestPacketCoderHdlc, decodeWithSliceFail)
{
    uint8_t outputEncodeBuffer[3];
    outpost::Slice<uint8_t> outputEncode(outputEncodeBuffer);

    outputEncode[0] = 0x7E;
    outputEncode[1] = 0x55;
    outputEncode[2] = 0x66;

    uint8_t outputDecodeBuffer[3];
    outpost::Slice<uint8_t> outputDecode(outputDecodeBuffer);
    bool result = mCoder.decode(outputEncode, outputDecode);

    EXPECT_FALSE(result);
}
