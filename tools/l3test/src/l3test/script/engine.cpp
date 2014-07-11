
#include "engine.h"

#include <sstream>

#include <lua/module/l3test/channel.h>
#include <lua/exception.h>
//#include <lua/stackdump.h> // for debugging

using namespace l3test::script;

namespace
{
    int atpanic(lua_State* L)
    {
        throw lua::Exception(lua_tostring(L, -1));
    }
}

Engine::Engine() :
        L(luaL_newstate())
{
    luaL_openlibs(L);
    lua_atpanic(L, atpanic);

    l3test_channel_open(L);
}

Engine::~Engine()
{
    lua_close(L);
}

static void
setPath(lua_State* L, const char* field, const char* path)
{
    lua_getglobal(L, "package");
    //lua_getfield(L, -1, field);

    //std::string cur_path = lua_tostring(L, -1);
    //lua_pop(L, 1);

    //cur_path.append(";");
    //cur_path.append(path);

    std::string cur_path = path;

    // Set the new path value
    lua_pushstring(L, cur_path.c_str());
    lua_setfield(L, -2, field);

    // Remove the package table
    lua_pop(L, 1);
}

void
Engine::setLuaPath(const char* path)
{
    setPath(L, "path", path);
}

void
Engine::setLuaCPath(const char* path)
{
    setPath(L, "cpath", path);
}

static std::vector<std::string> &
split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
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
Engine::registerChannel(Channel::Ptr channel, const char * name)
{
	for (auto c : channels)
	{
		if (c.first.compare(name) == 0) {
			// Name already registered
			return false;
		}
	}

	auto strName = std::string(name);
	auto nameElements = split(strName, '.');

	channels.push_back(std::make_pair(strName, channel));

	if (nameElements.size() == 1) {
	    l3test_channel_register(L, channel);
	    lua_setglobal(L, name);
	}
	else {
	    const char * globalTableName = nameElements[0].c_str();
	    lua_getglobal(L, globalTableName);
	    if (lua_isnil(L, -1)) {
	        lua_pop(L, 1);

	        // no global table exists, create one
            lua_newtable(L);
            lua_pushvalue(L, -1);   // duplicate table
            lua_setglobal(L, globalTableName);
	    }

	    for (size_t i = 1; i < nameElements.size() - 1; ++i) {
	        lua_getfield(L, -1, nameElements[i].c_str());
	        if (lua_isnil(L, -1)) {
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
Engine::execute(const char* string)
{
	int error = luaL_dostring(L, string);
    if (error) {
        lua_error(L);
    }
	return false;
}
