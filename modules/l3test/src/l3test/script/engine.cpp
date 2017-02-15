/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#include "engine.h"

#include <sstream>
#include <array>

#include <modules/l3test/channel.h>
#include <lua/exception.h>

using namespace l3test::script;

const std::string Engine::defaultPath[] =
{
    // all strings here except for the last have to end with the Lua path
    // separator (semicolon).
    std::string("lua/src/?.lua;"),
    std::string("lua/src/?/init.lua;"),
    std::string("ext/?/init.lua;"),
    std::string("ext/?.lua"),
};

const std::string Engine::defaultCPath[] =
{
    "../../bin/lua/?.so",
};

namespace
{
int atpanic(lua_State* L)
{
    throw lua::Exception(lua_tostring(L, -1));
}
}

// ----------------------------------------------------------------------------
static void
setPath(lua_State* L, const char* field, const char* path)
{
    lua_getglobal(L, "package");

    std::string cur_path = path;

    // Set the new path value
    lua_pushstring(L, cur_path.c_str());
    lua_setfield(L, -2, field);

    // Remove the package table
    lua_pop(L, 1);
}

static void
appendPath(lua_State* L, const char* field, const std::string path)
{
    lua_getglobal(L, "package");
    lua_getfield(L, -1, field);

    std::string currentPath = lua_tostring(L, -1);
    lua_pop(L, 1);

    currentPath.append(";");
    currentPath.append(path);

    // Set the new path value
    lua_pushstring(L, currentPath.c_str());
    lua_setfield(L, -2, field);

    // Remove the package table
    lua_pop(L, 1);
}

// ----------------------------------------------------------------------------
Engine::Engine() :
        L(luaL_newstate())
{
    luaL_openlibs(L);
    lua_atpanic(L, atpanic);

    l3test_channel_open(L);

    // clear the include path
    setPath(L, "path", "./?.lua");
    setPath(L, "cpath", "./?.so");
}

Engine::~Engine()
{
    lua_close(L);
}

// ----------------------------------------------------------------------------
void
Engine::appendDefaultLuaPath(std::string prefix)
{
    // The prefix path has to end with a slash.
    if (prefix.back() != '/')
    {
        prefix.append("/");
    }

    std::string path;
    for (auto p : defaultPath)
    {
        path.append(prefix);
        path.append(p);
    }
    appendPath(L, "path", path);

    std::string cPath;
    for (auto p : defaultCPath)
    {
        cPath.append(prefix);
        cPath.append(p);
    }
    appendPath(L, "cpath", cPath);
}

void
Engine::setLuaPath(const char* path)
{
    setPath(L, "path", path);
}

void
Engine::appendLuaPath(std::string path)
{
    appendPath(L, "path", path);
}

void
Engine::setLuaCPath(const char* path)
{
    setPath(L, "cpath", path);
}

void
Engine::appendLuaCPath(std::string path)
{
    appendPath(L, "cpath", path);
}

static std::vector<std::string>&
split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


static std::vector<std::string>
split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

bool
Engine::registerChannel(Channel::Ptr channel, const char* name)
{
	for (auto c : channels)
	{
		if (c.first.compare(name) == 0)
		{
			// Name already registered
			return false;
		}
	}

	auto strName = std::string(name);
	auto nameElements = split(strName, '.');

	channels.push_back(std::make_pair(strName, channel));

	if (nameElements.size() == 1)
	{
	    l3test_channel_register(L, channel);
	    lua_setglobal(L, name);
	}
	else
	{
	    const char* globalTableName = nameElements[0].c_str();
	    lua_getglobal(L, globalTableName);
	    if (lua_isnil(L, -1))
	    {
	        lua_pop(L, 1);

	        // no global table exists, create one
            lua_newtable(L);
            lua_pushvalue(L, -1);   // duplicate table
            lua_setglobal(L, globalTableName);
	    }

	    for (size_t i = 1; i < nameElements.size() - 1; ++i)
	    {
	        lua_getfield(L, -1, nameElements[i].c_str());
	        if (lua_isnil(L, -1))
	        {
	            lua_pop(L, 1);

                // entry does not exists, create it
                lua_newtable(L);
                lua_pushvalue(L, -1);   // duplicate table
                lua_setfield(L, -3, nameElements[i].c_str());
	        }
	    }

	    l3test_channel_register(L, channel);
	    lua_setfield(L, -2, nameElements[nameElements.size() - 1].c_str());

	    // clean up the stack
	    lua_pop(L, nameElements.size() - 1);
	}

	return true;
}

bool
Engine::execute(std::string code)
{
	int error = luaL_dostring(L, code.c_str());
    if (error)
    {
        lua_error(L);
    }
	return false;
}
