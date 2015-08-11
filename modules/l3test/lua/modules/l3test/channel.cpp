/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include "channel.h"

#include <cstdlib>
#include <cstdio>

#include <limits.h>
#include <stdint.h>

using namespace l3test::script;

/**
 * Create a new channel object.
 */
int
l3test_channel_new(lua_State* L)
{
	void* ptr = lua_newuserdata(L, sizeof(Channel::Ptr));
	new (ptr) Channel::Ptr(new Channel());

	luaL_getmetatable(L, "dlr.l3test.channel");
	lua_setmetatable(L, -2);

	return 1;
}

static inline Channel*
getChannel(lua_State* L)
{
    Channel::Ptr* ptr = reinterpret_cast<Channel::Ptr *>(luaL_checkudata(L, 1, "dlr.l3test.channel"));
    return ptr->get();
}

static int
l_destroy(lua_State* L)
{
    Channel::Ptr* c = reinterpret_cast<Channel::Ptr *>(luaL_checkudata(L, 1, "dlr.l3test.channel"));

    typedef Channel::Ptr ChannelPtr;
    c->~ChannelPtr();
    //c->Channel::Ptr::~Ptr();

    return 1;
}

// ---------------------------------------------------------------------------
/**
 * Create a string representation.
 */
static int
l_string_representation(lua_State* L)
{
	Channel* c = getChannel(L);
	lua_pushfstring(L, "l3test.channel(%d)", c->getNumberOfPackets());

	return 1;
}

// ---------------------------------------------------------------------------
static int
l_append(lua_State* L)
{
    Channel* c = getChannel(L);

    if (lua_istable(L, 2))
    {
        lua_len(L, 2);
        int length = lua_tointeger(L, -1);
        lua_pop(L, 1);

        uint8_t* a = reinterpret_cast<uint8_t *>(alloca(length));
        for (int i = 0; i < length; ++i)
        {
            lua_rawgeti(L, 2, i+1);
            a[i] = lua_tointeger(L, -1);
            lua_pop(L, 1);
        }

        c->append(a, length);
    }
    else if (lua_isnumber(L, 2))
    {
        // anything bigger than a byte is cropped
        uint8_t number = static_cast<uint8_t>(lua_tointeger(L, 2));
        c->append(&number, 1);
    }
    else
    {
        return luaL_argerror(L, 2, "Argument type must be a table or an integer");
    }

    return 0;
}

static int
l_finish_packet(lua_State* L)
{
    Channel* c = getChannel(L);
    c->finishPacket();

    return 0;
}

static int
l_send(lua_State* L)
{
    l_append(L);
    l_finish_packet(L);

    return 0;
}

// ---------------------------------------------------------------------------
static int
l_is_packet_available(lua_State* L)
{
    Channel* c = getChannel(L);
    lua_pushboolean(L, c->hasPacket() ? 1 : 0);

    return 1;
}

static int
l_number_of_packets(lua_State* L)
{
    Channel* c = getChannel(L);
    lua_pushinteger(L, c->getNumberOfPackets());

    return 1;
}

/*
 * Returns an array with an entry per byte.
 */
static int
l_get_packet(lua_State* L)
{
    Channel* c = getChannel(L);

    if (!c->hasPacket())
    {
        luaL_error(L, "No packet available");
    }

    // create a new table with the matching number of entries.
    int length = c->getPacketLength();
    lua_createtable(L, length, 0);

    // convert vector into a lua table with one entry per byte
    std::vector<uint8_t> packet = c->getPacket();
    for (int i = 0; i < length; ++i)
    {
        lua_pushinteger(L, packet[i]);
        lua_rawseti(L, -2, i+1);
    }

    return 1;
}

static int
l_next_packet(lua_State* L)
{
    Channel* c = getChannel(L);
    c->nextPacket();

    return 0;
}

// ---------------------------------------------------------------------------
static const struct luaL_Reg arraylib_m[] = {
	{ "__gc", l_destroy },
	{ "__tostring", l_string_representation },

	{ "append", l_append },
	{ "finishPacket", l_finish_packet },
	{ "send", l_send },

	{ "hasPacket", l_is_packet_available },
    { "getNumberOfPackets", l_number_of_packets },

    { "get", l_get_packet },
    { "next", l_next_packet },

	{ NULL, NULL },
};

void
l3test_channel_open(lua_State* L)
{
    luaL_newmetatable(L, "dlr.l3test.channel");

    // metatable.__index = metatable
    lua_pushvalue(L, -1);   // duplicate the metatable
    lua_setfield(L, -2, "__index");

    luaL_setfuncs(L, arraylib_m, 0);
    lua_pop(L, 1);
}

void
l3test_channel_register(lua_State* L, l3test::script::Channel::Ptr channel)
{
    void* ptr = lua_newuserdata(L, sizeof(Channel::Ptr));
    new (ptr) Channel::Ptr(channel);

    luaL_getmetatable(L, "dlr.l3test.channel");
    lua_setmetatable(L, -2);
}
