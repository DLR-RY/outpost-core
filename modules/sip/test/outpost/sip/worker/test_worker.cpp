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
#include <outpost/sip/packet_coder/packet_coder_hdlc.h>
#include <outpost/sip/packet_transport/packet_transport_serial.h>
#include <outpost/sip/worker/worker.h>

#include <unittest/hal/serial_stub.h>
#include <unittest/harness.h>

class TestWorker : public ::testing::Test
{
public:
    TestWorker() :
        mWorkerId(0x55),
        packetTransportSerial(clock, serial, packetCoderHdlc),
        worker(mWorkerId, packetTransportSerial)
    {
    }

    outpost::rtos::SystemClock clock;

    unittest::hal::SerialStub serial;

    outpost::sip::PacketCoderHdlc packetCoderHdlc;

    uint8_t mWorkerId;

    outpost::sip::PacketTransportSerial packetTransportSerial;

    outpost::sip::Worker worker;
};

TEST_F(TestWorker, sendResponse)
{
    EXPECT_TRUE(worker.sendResponse(2, 3));

    EXPECT_EQ(serial.mDataToTransmit[0], 0x7E);
    EXPECT_EQ(serial.mDataToTransmit[1], 0);
    EXPECT_EQ(serial.mDataToTransmit[2], 5);
    EXPECT_EQ(serial.mDataToTransmit[3], mWorkerId);
    EXPECT_EQ(serial.mDataToTransmit[4], 2);
    EXPECT_EQ(serial.mDataToTransmit[5], 3);
    // CRC
    // EXPECT_EQ(serial.mDataToTransmit[6], 0);
    // EXPECT_EQ(serial.mDataToTransmit[7], 0);
    EXPECT_EQ(serial.mDataToTransmit[8], 0x7E);
}
