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

#include <outpost/sip/packet/packet_writer.h>

#include <unittest/harness.h>

class TestPacketWriter : public ::testing::Test
{
public:
    TestPacketWriter()
    {
    }
};

TEST_F(TestPacketWriter, write)
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

    // check length
    ASSERT_EQ(bufferToWrite[1], 7);
    // check unitId
    ASSERT_EQ(bufferToWrite[2], 1);
    // check counter
    ASSERT_EQ(bufferToWrite[3], 2);
    // check type
    ASSERT_EQ(bufferToWrite[4], 3);
    // check payload
    ASSERT_EQ(bufferToWrite[5], 55);
    // check payload
    ASSERT_EQ(bufferToWrite[6], 66);

    std::size_t elements = 9;
    ASSERT_EQ(bufferToWrite.getNumberOfElements(), elements);
}
