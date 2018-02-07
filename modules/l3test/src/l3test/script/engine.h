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
// ----------------------------------------------------------------------------

#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include "channel.h"

#include <list>
#include <string>

extern "C"
{
#include <lua.h>
}

namespace l3test
{
namespace script
{
/**
 * Script execution engine.
 *
 * Controls the Lua execution environment and provides means of communication
 * between Lua and C++.
 *
 * \author  Fabian Greif
 */
class Engine
{
public:
    Engine();

    ~Engine();

    /**
     * Set the default path for
     *
     * \param prefix
     *     Prefix to the default Lua and Lua C path. The prefix should point
     *     to the l3test folder.
     */
    void
    appendDefaultLuaPath(std::string prefix);

    void
    setLuaPath(const char* path);

    void
    appendLuaPath(std::string path);

    void
    setLuaCPath(const char* path);

    void
    appendLuaCPath(std::string path);

    /**
     * Register channel for communication between Lua and C++.
     *
     * \param channel
     *     Channel to register.
     * \param name
     *     Unique name of the channel.
     *
     * \retval true  Channel could be registered.
     * \retval false Channel was not registered due to a naming clash.
     */
    bool
    registerChannel(Channel::Ptr channel, const char* name);

    bool
    execute(std::string code);

private:
    static const std::string defaultPath[];
    static const std::string defaultCPath[];

    lua_State* L;

    std::list<std::pair<std::string, Channel::Ptr>> channels;
};
}  // namespace script
}  // namespace l3test

#endif  // SCRIPT_ENGINE_H
