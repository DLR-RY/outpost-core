/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2022, Jan-Gerd Mess (DLR RY-AVS)
 */

#include <unittest/hal/can_bus_stub.h>
#include <unittest/harness.h>

using outpost::hal::CanBus;

// ----------------------------------------------------------------------------
TEST(CanFrameTest, frameConstruction)
{
    CanBus::CanFrame frame;
    EXPECT_EQ(frame.getId(), 0U);
    EXPECT_FALSE(frame.isExtended());
    EXPECT_FALSE(frame.isRequest());
    auto data = frame.getData();
    EXPECT_EQ(data.getNumberOfElements(), 0U);

    CanBus::CanFrame frame2(123U);
    EXPECT_EQ(frame2.getId(), 123U);
    EXPECT_FALSE(frame2.isExtended());
    EXPECT_FALSE(frame2.isRequest());
    auto data2 = frame2.getData();
    EXPECT_EQ(data2.getNumberOfElements(), 0U);

    uint8_t input[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    outpost::Slice<uint8_t> slice(input);

    frame2.writeData(slice);
    EXPECT_EQ(frame2.getId(), 123U);
    EXPECT_FALSE(frame2.isExtended());
    EXPECT_FALSE(frame2.isRequest());
    data2 = frame2.getData();
    EXPECT_EQ(data2.getNumberOfElements(), 8U);
    EXPECT_EQ(frame2.getLength(), 8U);
    EXPECT_ARRAY_EQ(uint8_t, input, data2.getDataPointer(), 8);

    frame2.writeData(slice.first(4));
    data2 = frame2.getData();
    EXPECT_EQ(data2.getNumberOfElements(), 4U);
    EXPECT_ARRAY_EQ(uint8_t, input, data2.getDataPointer(), 4);

    frame = frame2;
    EXPECT_EQ(frame.getId(), 123U);
    EXPECT_FALSE(frame.isExtended());
    EXPECT_FALSE(frame.isRequest());
    data = frame.getData();
    EXPECT_EQ(data.getNumberOfElements(), 4U);
    EXPECT_ARRAY_EQ(uint8_t, input, data.getDataPointer(), 4);

    CanBus::CanFrame frame3 = CanBus::CanFrame(456U, outpost::Slice<uint8_t>::empty(), true, true);
    EXPECT_EQ(frame3.getId(), 456U);
    EXPECT_TRUE(frame3.isExtended());
    EXPECT_TRUE(frame3.isRequest());
    auto data3 = frame3.getData();
    EXPECT_EQ(data3.getNumberOfElements(), 0U);

    frame3.writeData(slice);
    auto output = frame3.getData();
    EXPECT_ARRAY_EQ(uint8_t, input, output.getDataPointer(), 8);

    uint8_t changedInput[] = {41, 42};
    auto dataSlice = frame3.dataSlice();
    dataSlice[0] = 41;
    // cppcheck-suppress unreadVariable
    dataSlice[1] = 42;

    EXPECT_ARRAY_EQ(uint8_t, changedInput, frame3.getData().getDataPointer(), 2);
}

TEST(CanFrameTest, canIdTest)
{
    CanBus::CanFrame frame(std::numeric_limits<uint32_t>::max());
    EXPECT_FALSE(frame.isExtended());
    EXPECT_EQ(frame.getId(), 0x7FFU);

    CanBus::CanFrame frame2(
            std::numeric_limits<uint32_t>::max(), outpost::Slice<uint8_t>::empty(), true);
    EXPECT_TRUE(frame2.isExtended());
    EXPECT_EQ(frame2.getId(), ~0xE0000000U);

    frame = frame2;
    EXPECT_TRUE(frame.isExtended());
    EXPECT_EQ(frame.getId(), ~0xE0000000U);

    frame.setHeader(std::numeric_limits<uint32_t>::max(), false, true);
    EXPECT_FALSE(frame.isExtended());
    EXPECT_EQ(frame.getId(), 0x7FFU);
    EXPECT_TRUE(frame.isRequest());
}

TEST(CanStubTest, openTest)
{
    unittest::hal::CanBusStub can;

    CanBus::CanFrame frame;
    EXPECT_EQ(can.read(frame), CanBus::ReturnCode::notInitialized);
    EXPECT_EQ(can.write(frame), CanBus::ReturnCode::notInitialized);

    can.open();
    EXPECT_EQ(can.read(frame), CanBus::ReturnCode::bufferEmpty);
    EXPECT_EQ(can.write(frame), CanBus::ReturnCode::success);
}

TEST(CanStubTest, writeAndRequestTest)
{
    unittest::hal::CanBusStub can;
    can.open();

    uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    outpost::Slice<uint8_t> slice(data);

    CanBus::CanFrame frame(123U, slice);

    EXPECT_EQ(can.write(frame), CanBus::ReturnCode::success);

    ASSERT_EQ(can.mDataToTransmit.getSize(), 1U);
    unittest::hal::CanBusStub::CanFrame f = can.mDataToTransmit.getFront();

    auto fData = f.getData();
    EXPECT_EQ(fData.getNumberOfElements(), 8U);
    EXPECT_EQ(f.getLength(), 8U);
    EXPECT_EQ(f.getId(), 123U);
    EXPECT_FALSE(f.isRequest());
    EXPECT_ARRAY_EQ(uint8_t, data, fData.getDataPointer(), 8);

    // set frame to request
    frame = CanBus::CanFrame::makeRequestFrame(123U, 6U, false);
    EXPECT_EQ(can.write(frame), CanBus::ReturnCode::success);

    ASSERT_EQ(can.mDataToTransmit.getSize(), 2U);
    can.mDataToTransmit.removeFront();
    f = can.mDataToTransmit.getFront();
    EXPECT_TRUE(f.isRequest());
    EXPECT_FALSE(f.isExtended());
    EXPECT_EQ(f.getId(), 123U);
    EXPECT_EQ(f.getLength(), 6U);
}

TEST(CanStubTest, readTest)
{
    unittest::hal::CanBusStub can;
    can.open();

    uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    outpost::Slice<uint8_t> slice(data);
    unittest::hal::CanBusStub::CanFrame frame(123U, slice);
    can.mDataToReceive.append(frame);

    unittest::hal::CanBusStub::CanFrame inFrame;

    ASSERT_EQ(can.read(inFrame), CanBus::ReturnCode::success);
    EXPECT_EQ(inFrame.isRequest(), false);
    EXPECT_EQ(inFrame.getId(), 123U);
    auto inData = inFrame.getData();
    ASSERT_EQ(inData.getNumberOfElements(), 8U);
    EXPECT_ARRAY_EQ(uint8_t, slice.begin(), inData.getDataPointer(), 8U);
}

TEST(CanStubTest, clearRxBufferTest)
{
    unittest::hal::CanBusStub can;
    can.open();

    uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    outpost::Slice<uint8_t> slice(data);
    unittest::hal::CanBusStub::CanFrame frame(123U, slice);
    can.mDataToReceive.append(frame);

    unittest::hal::CanBusStub::CanFrame inFrame;

    EXPECT_FALSE(can.mDataToReceive.isEmpty());

    EXPECT_EQ(can.clearReceiveBuffer(), outpost::hal::CanBus::ReturnCode::success);

    EXPECT_TRUE(can.mDataToReceive.isEmpty());
}

TEST(CanStubTest, transmitTest)
{
    unittest::hal::CanBusStub can;
    can.open();
    uint8_t data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    outpost::Slice<uint8_t> slice(data);
    outpost::hal::CanBus::CanFrame frame(123U, slice);

    ASSERT_FALSE(can.isDataAvailable());
    EXPECT_EQ(can.write(frame), CanBus::ReturnCode::success);

    ASSERT_EQ(can.mDataToTransmit.getSize(), 1U);
    outpost::hal::CanBus::CanFrame f = can.mTransmitBuffer[1];
    auto fData = f.getData();
    EXPECT_EQ(fData.getNumberOfElements(), 8U);
    EXPECT_EQ(f.getId(), 123U);
    EXPECT_FALSE(f.isRequest());
    EXPECT_ARRAY_EQ(uint8_t, data, fData.getDataPointer(), 8);

    outpost::hal::CanBus::CanFrame frame2(124U, slice, true, true);
    EXPECT_EQ(can.write(frame2), CanBus::ReturnCode::success);

    ASSERT_EQ(can.mDataToTransmit.getSize(), 2U);
    auto f2 = can.mTransmitBuffer[2];
    auto f2Data = f2.getData();
    EXPECT_EQ(f2Data.getNumberOfElements(), 8U);
    EXPECT_EQ(f2.getId(), 124U);
    EXPECT_TRUE(f2.isRequest());

    unittest::hal::CanBusStub receiverCan;
    receiverCan.open();

    can.transferToOtherStub(can, false);
    ASSERT_EQ(can.mDataToTransmit.getSize(), 2U);
    ASSERT_EQ(can.mDataToReceive.getSize(), 2U);
    ASSERT_TRUE(can.isDataAvailable());

    ASSERT_FALSE(receiverCan.isDataAvailable());
    can.transferToOtherStub(receiverCan, true);
    ASSERT_EQ(can.mDataToTransmit.getSize(), 0U);
    ASSERT_EQ(receiverCan.mDataToReceive.getSize(), 2U);
    ASSERT_TRUE(receiverCan.isDataAvailable());

    outpost::hal::CanBus::CanFrame inFrame;

    ASSERT_EQ(receiverCan.read(inFrame), CanBus::ReturnCode::success);
    EXPECT_FALSE(inFrame.isRequest());
    EXPECT_EQ(inFrame.getId(), 123U);
    auto inData = inFrame.getData();
    ASSERT_EQ(inData.getNumberOfElements(), 8U);
    EXPECT_ARRAY_EQ(uint8_t, slice.begin(), inData.begin(), 8U);

    EXPECT_EQ(receiverCan.mDataToReceive.getSize(), 1U);

    ASSERT_EQ(receiverCan.read(inFrame), CanBus::ReturnCode::success);
    EXPECT_TRUE(inFrame.isRequest());
    EXPECT_EQ(inFrame.getId(), 124U);
    auto inData2 = inFrame.getData();
    ASSERT_EQ(inData2.getNumberOfElements(), 8U);
    EXPECT_ARRAY_EQ(uint8_t, slice.begin(), inData2.begin(), 8U);

    EXPECT_EQ(receiverCan.mDataToReceive.getSize(), 0U);
}
