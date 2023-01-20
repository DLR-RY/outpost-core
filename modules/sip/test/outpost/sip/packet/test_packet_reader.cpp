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
#include <outpost/utils/storage/serialize.h>

#include <unittest/harness.h>

class TestPacketReader : public ::testing::Test
{
public:
    TestPacketReader()
    {
    }
};

TEST_F(TestPacketReader, minPacketSize)
{
    uint8_t bufferWriter[outpost::sip::maxPacketLength];
    outpost::Slice<uint8_t> bufferPacketWriter(bufferWriter);
    outpost::sip::PacketWriter packetWriter(bufferPacketWriter);

    packetWriter.setWorkerId(1);
    packetWriter.setCounter(2);
    packetWriter.setType(3);

    uint8_t buffer[256];
    outpost::Slice<uint8_t> bufferToWrite(buffer);
    // cppcheck-suppress unreadVariable
    bufferToWrite = packetWriter.get();

    uint8_t bufferRead[2];
    outpost::Slice<uint8_t> bufferToRead(bufferRead);
    outpost::sip::PacketReader packetReader(bufferToRead);
    EXPECT_EQ(packetReader.readPacket(), outpost::sip::PacketReader::readResults::lengthError);
}

TEST_F(TestPacketReader, lengthError)
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
    // change length
    // cppcheck-suppress unreadVariable
    bufferToWrite[1] = 4;

    uint8_t bufferRead[2];
    outpost::Slice<uint8_t> bufferToRead(bufferRead);
    outpost::sip::PacketReader packetReader(bufferToRead);
    EXPECT_EQ(packetReader.readPacket(), outpost::sip::PacketReader::readResults::lengthError);
}

TEST_F(TestPacketReader, crcError)
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
    // change crc
    bufferToWrite[6] = 0xFF;

    outpost::sip::PacketReader packetReader(bufferToWrite);
    EXPECT_EQ(packetReader.readPacket(), outpost::sip::PacketReader::readResults::crcError);
}

TEST_F(TestPacketReader, readNoPayload)
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

    outpost::sip::PacketReader packetReader(bufferToWrite);
    EXPECT_EQ(packetReader.readPacket(), outpost::sip::PacketReader::readResults::success);

    // check length
    ASSERT_EQ(packetReader.getLength(), 5);
    // check unitId
    ASSERT_EQ(packetReader.getWorkerId(), 1);
    // check counter
    ASSERT_EQ(packetReader.getCounter(), 2);
    // check type
    ASSERT_EQ(packetReader.getType(), 3);
}

TEST_F(TestPacketReader, readPayload)
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

    uint8_t buffer[256];
    outpost::Slice<uint8_t> bufferToWrite(buffer);
    bufferToWrite = packetWriter.get();

    outpost::sip::PacketReader packetReader(bufferToWrite);
    EXPECT_EQ(packetReader.readPacket(), outpost::sip::PacketReader::readResults::success);
    payload = packetReader.getPayloadData();

    // check length
    ASSERT_EQ(packetReader.getLength(), 7);
    // check unitId
    ASSERT_EQ(packetReader.getWorkerId(), 1);
    // check counter
    ASSERT_EQ(packetReader.getCounter(), 2);
    // check type
    ASSERT_EQ(packetReader.getType(), 3);
    // check payload
    ASSERT_EQ(payload[0], 55);
    // check payload
    ASSERT_EQ(payload[1], 66);
}
