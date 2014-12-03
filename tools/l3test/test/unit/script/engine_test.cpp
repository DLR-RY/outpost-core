/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.3-pre.
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

#include <l3test/script/engine.h>

using namespace l3test::script;

/*
 * Check the default values of the object.
 */
TEST(EngineTest, creation)
{
	Engine engine;
}

/*
 * Channels can be registered under multiple names.
 */
TEST(EngineTest, channelRegistration)
{
	Engine engine;
	Channel::Ptr channel(new Channel);

	EXPECT_TRUE(engine.registerChannel(channel, "test"));
	EXPECT_TRUE(engine.registerChannel(channel, "test2"));
}

/*
 * Registered channels must have a unique name.
 */
TEST(EngineTest, channelRegistrationClash)
{
	Engine engine;
	Channel::Ptr channel(new Channel);
	Channel::Ptr channel2(new Channel);

	EXPECT_TRUE(engine.registerChannel(channel, "test"));
	EXPECT_FALSE(engine.registerChannel(channel2, "test"));
}

/*
 * Registered channels should appear in the Lua code.
 *
 * It the channel is not available the test fails with an uncaught exception.
 */
TEST(EngineTest, channelRegistrationLua)
{
    Engine engine;
    Channel::Ptr channel(new Channel);

    engine.registerChannel(channel, "tm");

    engine.execute(R"--(
tm:append(1)
)--");
}

/*
 * It should be possible to register channels to a deeper level.
 *
 * Levels/tables are separated by dots as they will be in the Lua code.
 */
TEST(EngineTest, channelRegistrationLuaNested)
{
    Engine engine;
    Channel::Ptr channel(new Channel);

    engine.registerChannel(channel, "output.tm.essential");

    engine.execute(R"--(
output.tm.essential:append(1)
)--");
}

/*
 * It should be possible to register channels to a similar (deeper) level.
 *
 * Tables are shared when creating two channels with a shared base name.
 */
TEST(EngineTest, channelRegistrationLuaSharedTables)
{
    Engine engine;
    Channel::Ptr channel(new Channel);
    Channel::Ptr channel2(new Channel);

    engine.registerChannel(channel, "output.tm.essential");
    engine.registerChannel(channel2, "output.tm.extended");

    engine.execute(R"--(
output.tm.essential:append(1)
output.tm.extended:append(1)
)--");
}

/*
 * Send data from Lua through a channel to the C++ side.
 */
TEST(EngineTest, sendDataFromLua)
{
	Engine engine;
	Channel::Ptr channel(new Channel);

	engine.registerChannel(channel, "tm");

	engine.execute(R"--(
data = { 1, 2, 3, 4, 5, 6 }
tm:send(data)
)--");

	ASSERT_TRUE(channel->hasPacket());
	EXPECT_EQ(6U, channel->getPacketLength());

	auto packet = channel->getPacket();

	uint8_t data[6] = { 1, 2, 3, 4, 5, 6 };
	EXPECT_ARRAY_EQ(uint8_t, data, packet.data(), 6);
}

/*
 * Receive data in the Lua environment.
 *
 * Failed assert statements inside Lua are reported as uncaught exceptions
 * in googletest.
 */
TEST(EngineTest, sendDataToLua)
{
    Engine engine;
    Channel::Ptr channel(new Channel);

    engine.registerChannel(channel, "tc");

    uint8_t data[6] = { 1, 2, 6, 5, 4, 3 };

    channel->append(data, sizeof(data));
    channel->finishPacket();

    engine.execute(R"--(
assert(tc:getNumberOfPackets() == 1, "number of packets wrong")

packet = tc:get()
tc:next()

assert(#packet == 6, "wrong packet size")
assert(packet[1], 1)
assert(packet[2], 2)
assert(packet[3], 6)
assert(packet[4], 5)
assert(packet[5], 4)
assert(packet[6], 3)
)--");
}
