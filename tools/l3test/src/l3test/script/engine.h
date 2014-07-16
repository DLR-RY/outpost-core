
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
	execute(const char* string);

private:
	lua_State* L;

	std::list< std::pair<std::string, Channel::Ptr> > channels;
};
}
}

#endif // SCRIPT_ENGINE_H
