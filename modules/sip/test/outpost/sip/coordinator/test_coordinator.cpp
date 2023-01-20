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
#include <outpost/sip/coordinator/coordinator.h>
#include <outpost/sip/packet_coder/packet_coder_hdlc.h>
#include <outpost/sip/packet_transport/packet_transport_serial.h>

#include <unittest/hal/serial_stub.h>
#include <unittest/harness.h>

class TestCoordinator : public ::testing::Test
{
public:
    TestCoordinator() :
        packetTransportSerial(clock, serial, packetCoderHdlc), coordinator(packetTransportSerial)
    {
    }

    outpost::rtos::SystemClock clock;

    unittest::hal::SerialStub serial;

    outpost::sip::PacketCoderHdlc packetCoderHdlc;

    outpost::sip::PacketTransportSerial packetTransportSerial;

    outpost::sip::Coordinator coordinator;
};

TEST_F(TestCoordinator, responseError)
{
    EXPECT_EQ(coordinator.sendRequest(1, 2, 3, 4),
              outpost::sip::Coordinator::requestResults::responseError);
}

TEST_F(TestCoordinator, success)
{
    const uint8_t workerId = 1;
    const uint8_t counter = 2;
    const uint8_t type = 3;
    const uint8_t expectedType = 4;

    outpost::sip::Coordinator::ResponseData data;
    data.length = 5;
    data.workerId = workerId;
    data.counter = counter;
    data.type = expectedType;

    EXPECT_TRUE(coordinator.sendResponseQueue(data));

    EXPECT_EQ(coordinator.sendRequest(workerId, counter, type, expectedType),
              outpost::sip::Coordinator::requestResults::success);
}

TEST_F(TestCoordinator, workerIdError)
{
    const uint8_t workerId = 1;
    const uint8_t counter = 2;
    const uint8_t type = 3;
    const uint8_t expectedType = 4;

    outpost::sip::Coordinator::ResponseData data;
    data.length = 5;
    data.workerId = 0;
    data.counter = counter;
    data.type = expectedType;

    EXPECT_TRUE(coordinator.sendResponseQueue(data));

    EXPECT_EQ(coordinator.sendRequest(workerId, counter, type, expectedType),
              outpost::sip::Coordinator::requestResults::workerIdError);
}

TEST_F(TestCoordinator, responseTypeError)
{
    const uint8_t workerId = 1;
    const uint8_t counter = 2;
    const uint8_t type = 3;
    const uint8_t expectedType = 4;

    outpost::sip::Coordinator::ResponseData data;
    data.length = 5;
    data.workerId = workerId;
    data.counter = counter;
    data.type = 0;

    EXPECT_TRUE(coordinator.sendResponseQueue(data));

    EXPECT_EQ(coordinator.sendRequest(workerId, counter, type, expectedType),
              outpost::sip::Coordinator::requestResults::responseTypeError);
}

TEST_F(TestCoordinator, responseErrorWithData)
{
    uint8_t payloadWorkerData[1];
    outpost::Slice<uint8_t> workerData = outpost::asSlice(payloadWorkerData);
    EXPECT_EQ(coordinator.sendRequestGetResponseData(1, 2, 3, 4, workerData),
              outpost::sip::Coordinator::requestResults::responseError);
}

TEST_F(TestCoordinator, successWithData)
{
    const uint8_t workerId = 1;
    const uint8_t counter = 2;
    const uint8_t type = 3;
    const uint8_t expectedType = 4;

    outpost::sip::Coordinator::ResponseData data;
    data.length = 5;
    data.workerId = workerId;
    data.counter = counter;
    data.type = expectedType;
    data.payloadData[0] = 55;
    data.payloadDataLength = 1;

    EXPECT_TRUE(coordinator.sendResponseQueue(data));

    uint8_t payloadWorkerData[1];
    outpost::Slice<uint8_t> workerData = outpost::asSlice(payloadWorkerData);
    EXPECT_EQ(coordinator.sendRequestGetResponseData(
                      workerId, counter, type, expectedType, workerData),
              outpost::sip::Coordinator::requestResults::success);

    EXPECT_EQ(data.payloadData[0], workerData[0]);
}

TEST_F(TestCoordinator, workerIdErrorWithData)
{
    const uint8_t workerId = 1;
    const uint8_t counter = 2;
    const uint8_t type = 3;
    const uint8_t expectedType = 4;

    outpost::sip::Coordinator::ResponseData data;
    data.length = 5;
    data.workerId = 0;
    data.counter = counter;
    data.type = expectedType;
    data.payloadData[0] = 55;
    data.payloadDataLength = 1;

    EXPECT_TRUE(coordinator.sendResponseQueue(data));

    uint8_t payloadWorkerData[1];
    outpost::Slice<uint8_t> workerData = outpost::asSlice(payloadWorkerData);
    EXPECT_EQ(coordinator.sendRequestGetResponseData(
                      workerId, counter, type, expectedType, workerData),
              outpost::sip::Coordinator::requestResults::workerIdError);
}

TEST_F(TestCoordinator, responseTypeErrorWithData)
{
    const uint8_t workerId = 1;
    const uint8_t counter = 2;
    const uint8_t type = 3;
    const uint8_t expectedType = 4;

    outpost::sip::Coordinator::ResponseData data;
    data.length = 5;
    data.workerId = workerId;
    data.counter = counter;
    data.type = 0;
    data.payloadData[0] = 55;
    data.payloadDataLength = 1;

    EXPECT_TRUE(coordinator.sendResponseQueue(data));

    uint8_t payloadWorkerData[1];
    outpost::Slice<uint8_t> workerData = outpost::asSlice(payloadWorkerData);
    EXPECT_EQ(coordinator.sendRequestGetResponseData(
                      workerId, counter, type, expectedType, workerData),
              outpost::sip::Coordinator::requestResults::responseTypeError);
}
