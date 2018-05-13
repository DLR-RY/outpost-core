/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef LUA_L3TEST_CHANNEL_H
#define LUA_L3TEST_CHANNEL_H

#include <memory>

#include <l3test/script/channel.h>

#include <lua.hpp>

// Lua functions
/**
 * Creates a new channel object.
 *
 * The lifetime of the channel is determined through a shared pointer, therefore
 * the channel can be shared with C++ without the risk of having an invalid
 * pointer to the channel because the Lua object was garbage collected.
 *
 * See l3test_channel_register() on how to create a channel between Lua
 * and C++.
 *
 * \param L
 * \return
 */
int
l3test_channel_new(lua_State* L);

// ---------------------------------------------------------------------------
/**
 * Opens the l3test.channel library
 *
 * \param L
 *      Lua state
 */
void
l3test_channel_open(lua_State* L);

/**
 * Pushes a channel on the current Lua stack.
 *
 * \param L
 *      Lua state.
 * \param channel
 *      Channel which should be used from within C++.
 */
void
l3test_channel_register(lua_State* L, l3test::script::Channel::Ptr channel);

#endif // LUA_L3TEST_CHANNEL_H
