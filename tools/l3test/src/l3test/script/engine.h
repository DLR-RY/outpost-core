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

#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include <list>
#include <string>

#include "channel.h"

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
	setLuaCPath(const char* path);

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

	std::list< std::pair<std::string, Channel::Ptr> > channels;
};
}
}

#endif // SCRIPT_ENGINE_H
