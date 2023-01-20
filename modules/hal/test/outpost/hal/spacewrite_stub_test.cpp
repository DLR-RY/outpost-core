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
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 * - 2017, Muhammad Bassam (DLR RY-AVS)
 */

#include <unittest/hal/spacewire_stub.h>
#include <unittest/harness.h>

using outpost::hal::SpaceWire;

class SpaceWireStubTest : public testing::Test
{
public:
    SpaceWireStubTest() : mSpaceWire(100)
    {
    }

    virtual void
    SetUp() override
    {
        mSpaceWire.open();
        mSpaceWire.up(outpost::time::Duration::zero());
    }

    virtual void
    TearDown() override
    {
    }

    unittest::hal::SpaceWireStub mSpaceWire;
};

// ----------------------------------------------------------------------------
TEST(SpaceWireStubConnectionTest, shouldEnableLink)
{
    unittest::hal::SpaceWireStub spaceWire(100);
    EXPECT_TRUE(spaceWire.open());
}

TEST(SpaceWireStubConnectionTest, shouldBringLinkUp)
{
    unittest::hal::SpaceWireStub spaceWire(100);

    spaceWire.open();
    spaceWire.up(outpost::time::Duration::zero());

    EXPECT_TRUE(spaceWire.isUp());
}

TEST(SpaceWireStubConnectionTest, shouldBringLinkDown)
{
    unittest::hal::SpaceWireStub spaceWire(100);

    spaceWire.open();

    spaceWire.up(outpost::time::Duration::zero());
    spaceWire.down(outpost::time::Duration::zero());

    EXPECT_FALSE(spaceWire.isUp());
}

TEST(SpaceWireStubConnectionTest, shouldBringLinkDownOnClose)
{
    unittest::hal::SpaceWireStub spaceWire(100);

    spaceWire.open();
    spaceWire.up(outpost::time::Duration::zero());

    spaceWire.close();

    EXPECT_FALSE(spaceWire.isUp());
}

TEST(SpaceWireStubConnectionTest, shouldNotBringLinkUpOnClosedChannel)
{
    unittest::hal::SpaceWireStub spaceWire(100);

    spaceWire.up(outpost::time::Duration::zero());

    EXPECT_FALSE(spaceWire.isUp());
}

// ----------------------------------------------------------------------------
TEST_F(SpaceWireStubTest, shouldProvideTransmitBuffer)
{
    SpaceWire::TransmitBuffer* buffer = nullptr;
    ASSERT_EQ(SpaceWire::Result::success,
              mSpaceWire.requestBuffer(buffer, outpost::time::Duration::zero()));

    ASSERT_NE(nullptr, buffer);
    EXPECT_FALSE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(SpaceWireStubTest, shouldReleaseTransmitBuffer)
{
    SpaceWire::TransmitBuffer* buffer = nullptr;
    mSpaceWire.requestBuffer(buffer, outpost::time::Duration::zero());
    ASSERT_EQ(SpaceWire::Result::success, mSpaceWire.send(buffer, outpost::time::Duration::zero()));

    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(SpaceWireStubTest, shouldTransmitData)
{
    std::vector<uint8_t> expectedData = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xAB, 0xCD};

    SpaceWire::TransmitBuffer* buffer = nullptr;
    ASSERT_EQ(SpaceWire::Result::success,
              mSpaceWire.requestBuffer(buffer, outpost::time::Duration::zero()));

    ASSERT_TRUE(buffer->getData().copyFrom(&expectedData.front(), expectedData.size()));
    buffer->setLength(expectedData.size());
    buffer->setEndMarker(SpaceWire::eop);

    mSpaceWire.send(buffer, outpost::time::Duration::zero());

    ASSERT_EQ(1U, mSpaceWire.mSentPackets.size());

    auto& packet = mSpaceWire.mSentPackets.front();
    EXPECT_EQ(expectedData, packet.data);
    EXPECT_EQ(SpaceWire::eop, packet.end);
}

TEST_F(SpaceWireStubTest, shouldReceiveTimeout)
{
    SpaceWire::ReceiveBuffer buffer;
    ASSERT_EQ(SpaceWire::Result::timeout,
              mSpaceWire.receive(buffer, outpost::time::Duration::zero()));
}

TEST_F(SpaceWireStubTest, shouldReceiveData)
{
    std::vector<uint8_t> expectedData = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xAB, 0xCD};

    mSpaceWire.mPacketsToReceive.emplace_back(
            unittest::hal::SpaceWireStub::Packet{expectedData, SpaceWire::eep});

    SpaceWire::ReceiveBuffer buffer;
    ASSERT_EQ(SpaceWire::Result::success,
              mSpaceWire.receive(buffer, outpost::time::Duration::zero()));

    EXPECT_EQ(expectedData.size(), buffer.getLength());
    EXPECT_EQ(SpaceWire::eep, buffer.getEndMarker());
    ASSERT_THAT(expectedData,
                testing::ElementsAreArray(buffer.getData().begin(), buffer.getLength()));

    mSpaceWire.releaseBuffer(buffer);

    EXPECT_TRUE(mSpaceWire.mPacketsToReceive.empty());
    EXPECT_TRUE(mSpaceWire.noUsedReceiveBuffers());
}
