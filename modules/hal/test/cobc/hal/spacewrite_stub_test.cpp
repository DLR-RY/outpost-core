/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include <unittest/harness.h>

#include <unittest/hal/spacewire_stub.h>

using cobc::hal::SpaceWire;

class SpaceWireStubTest : public testing::Test
{
public:
    SpaceWireStubTest() :
        mSpaceWire(100)
    {
    }

    virtual void
    SetUp() override
    {
        mSpaceWire.open();
        mSpaceWire.up();
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
    spaceWire.up();

    EXPECT_TRUE(spaceWire.isUp());
}

TEST(SpaceWireStubConnectionTest, shouldBringLinkDown)
{
    unittest::hal::SpaceWireStub spaceWire(100);

    spaceWire.open();

    spaceWire.up();
    spaceWire.down();

    EXPECT_FALSE(spaceWire.isUp());
}

TEST(SpaceWireStubConnectionTest, shouldBringLinkDownOnClose)
{
    unittest::hal::SpaceWireStub spaceWire(100);

    spaceWire.open();
    spaceWire.up();

    spaceWire.close();

    EXPECT_FALSE(spaceWire.isUp());
}

TEST(SpaceWireStubConnectionTest, shouldNotBringLinkUpOnClosedChannel)
{
    unittest::hal::SpaceWireStub spaceWire(100);

    spaceWire.up();

    EXPECT_FALSE(spaceWire.isUp());
}

// ----------------------------------------------------------------------------
TEST_F(SpaceWireStubTest, shouldProvideTransmitBuffer)
{
    SpaceWire::TransmitBuffer* buffer = nullptr;
    ASSERT_EQ(SpaceWire::success, mSpaceWire.requestBuffer(buffer));

    ASSERT_NE(nullptr, buffer);
    EXPECT_FALSE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(SpaceWireStubTest, shouldReleaseTransmitBuffer)
{
    SpaceWire::TransmitBuffer* buffer = nullptr;
    mSpaceWire.requestBuffer(buffer);
    ASSERT_EQ(SpaceWire::success, mSpaceWire.send(buffer));

    EXPECT_TRUE(mSpaceWire.noUsedTransmitBuffers());
}

TEST_F(SpaceWireStubTest, shouldTransmitData)
{
    std::vector<uint8_t> expectedData = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xAB, 0xCD
    };

    SpaceWire::TransmitBuffer* buffer = nullptr;
    mSpaceWire.requestBuffer(buffer);

    memcpy(buffer->data, &expectedData.front(), expectedData.size());
    buffer->length = expectedData.size();
    buffer->end = SpaceWire::eop;

    mSpaceWire.send(buffer);

    ASSERT_EQ(1U, mSpaceWire.mSentPackets.size());

    auto& packet = mSpaceWire.mSentPackets.front();
    EXPECT_EQ(expectedData, packet.data);
    EXPECT_EQ(SpaceWire::eop, packet.end);
}

TEST_F(SpaceWireStubTest, shouldReceiveData)
{
    std::vector<uint8_t> expectedData = {
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xAB, 0xCD
    };

    mSpaceWire.mPacketsToReceive.emplace_back(unittest::hal::SpaceWireStub::Packet { expectedData, SpaceWire::eep });

    SpaceWire::ReceiveBuffer buffer;
    ASSERT_EQ(SpaceWire::success, mSpaceWire.receive(buffer));

    EXPECT_EQ(expectedData.size(), buffer.length);
    EXPECT_EQ(SpaceWire::eep, buffer.end);
    ASSERT_THAT(expectedData, testing::ElementsAreArray(buffer.data, buffer.length));

    mSpaceWire.releaseBuffer(buffer);

    EXPECT_TRUE(mSpaceWire.mPacketsToReceive.empty());
    EXPECT_TRUE(mSpaceWire.noUsedReceiveBuffers());
}
