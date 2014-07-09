
#include <unittest/harness.h>
#include <l3test/script/channel.h>

using namespace l3test::script;

/*
 * Check the default values of the object.
 */
TEST(ChannelTest, creation)
{
	Channel channel;

	EXPECT_FALSE(channel.hasPackets());
	EXPECT_EQ(0U, channel.getPacketLength());
}

TEST(ChannelTest, pushData)
{
	Channel channel;

	uint8_t data[16];
	channel.append(data, 16);
	EXPECT_FALSE(channel.hasPackets());

	channel.finishPacket();
	EXPECT_TRUE(channel.hasPackets());
}

TEST(ChannelTest, receiveData)
{
	Channel channel;

	uint8_t data[16];
	for (uint_fast8_t i = 0; i < 16; ++i) {
		data[i] = 16 - i;
	}

	channel.append(data, 16);
	channel.finishPacket();

	ASSERT_TRUE(channel.hasPackets());
	EXPECT_EQ(16U, channel.getPacketLength());

	std::vector<uint8_t> recv = channel.getPacket();

	EXPECT_EQ(16U, recv.size());
	EXPECT_ARRAY_EQ(uint8_t, data, recv.data(), 16);

	// remove the packet form the list
	channel.nextPacket();
	EXPECT_FALSE(channel.hasPackets());
}

/*
 * Send and receive multiple packets.
 *
 * Check that the order of packets is correct.
 */
TEST(ChannelTest, receiveMultiple)
{
	Channel channel;
	uint8_t data[16];

	for (uint_fast8_t i = 0; i < 16; ++i) {
		data[i] = 16 - i;
	}

	channel.append(data, 16);
	channel.finishPacket();

	uint8_t data2[8];
	for (uint_fast8_t i = 0; i < 8; ++i) {
		data2[i] = i;
	}

	channel.append(data2, 8);
	channel.finishPacket();

	ASSERT_TRUE(channel.hasPackets());
	EXPECT_EQ(16U, channel.getPacketLength());

	std::vector<uint8_t> recv = channel.getPacket();

	EXPECT_EQ(16U, recv.size());
	EXPECT_ARRAY_EQ(uint8_t, data, recv.data(), 16);

	// remove the packet form the list
	channel.nextPacket();

	ASSERT_TRUE(channel.hasPackets());
	EXPECT_EQ(8U, channel.getPacketLength());

	std::vector<uint8_t> recv2 = channel.getPacket();

	EXPECT_EQ(8U, recv2.size());
	EXPECT_ARRAY_EQ(uint8_t, data2, recv2.data(), 8);

	// remove the packet form the list
	channel.nextPacket();

	EXPECT_FALSE(channel.hasPackets());
}
