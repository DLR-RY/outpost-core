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

#include <outpost/coding/crc16.h>
#include "crc16.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdint.h>


using namespace outpost;

//static ptrdiff_t
//posrelat(ptrdiff_t pos, size_t len)
//{
//    /* relative string position: negative means back from end */
//    if (pos < 0) pos += (ptrdiff_t)len + 1;
//    return (pos >= 0) ? pos : 0;
//}

static Crc16Ccitt*
checkudata(lua_State* L)
{
    Crc16Ccitt** d = (Crc16Ccitt **) luaL_checkudata(L, 1, "dlr.crc16");
    luaL_argcheck(L, *d, 1, "crc object state has been destroyed");

    return *d;
}

/**
 * Returns a crc object.
 */
static int
l_crc16_function_new(lua_State* L)
{
    Crc16Ccitt** d = (Crc16Ccitt**) lua_newuserdata(L, sizeof(d));

    luaL_getmetatable(L, "dlr.crc16");
    lua_setmetatable(L, -2);

    *d = new Crc16Ccitt();
    luaL_argcheck(L, *d, 1, "Out of memory");

    return 1;
}

/**
 * Resets the CRC to it's initial state.
 *
 *   self = crc:reset()
 *
 * Returns the crc object.
 */
static int
l_crc16_reset(lua_State* L)
{
    Crc16Ccitt* d = checkudata(L);
    d->reset();

    return 1;
}

/**
 * Process a single byte.
 *
 *
 */
static int
l_crc16_update(lua_State* L)
{
    Crc16Ccitt* d = checkudata(L);

    int value = luaL_checkinteger(L, 2);
    if (value > 0xff || value < 0)
    {
        luaL_argcheck(L, false, 2, "Invalid value");
    }

    d->update(static_cast<uint8_t>(value));
    lua_settop(L, 1);
    return 1;
}

/**
 *
 * \param L
 *      Lua state
 * \param offset
 *      Offset of the argument on the stack
 * \param d
 *      CRC object
 */
static void
calculate(lua_State* L, int offset, Crc16Ccitt* d)
{
    int type = lua_type(L, offset);
    if (type == LUA_TTABLE)
    {
        int start = luaL_optinteger(L, offset + 1, 1);
        int end = luaL_optinteger(L, offset + 2, -1);

        // remove optional start and end parameters from the stack
        lua_settop(L, offset);

        // Make the end relative to the start of the table, not the end
        if (end < 0)
        {
            // Get table size
            lua_len(L, offset);
            size_t len = lua_tointeger(L, -1);
            lua_pop(L, 1);

            // 'end = -1' refers to the last entry, which is equal to 'len'.
            // Therefore adding one is required.
            end = len + end + 1;
        }

        for (int i = start; i <= end; ++i)
        {
            lua_rawgeti(L, offset, i);
            int value = lua_tointeger(L, -1);
            lua_pop(L, 1);

            if (value > 0xff || value < 0)
            {
                luaL_argcheck(L, false, offset, "Table contains invalid values");
            }
            d->update(value);
        }

    }
    else if (type == LUA_TSTRING)
    {
        size_t length;
        const char* str = lua_tolstring(L, offset, &length);

        for (size_t i = 0; i < length; ++i)
        {
            d->update(str[i]);
        }
    }
    else
    {
        luaL_argcheck(L, false, offset, "Invalid type");
    }
}

/**
 * Process a substring of bytes.
 *
 *   self = crc:update(bytes, [start, [, end]])
 *
 * Process substring of bytes from start..end.
 *
 * If \c end is absent, it defaults to -1, the end of the bytes.
 * If \c start is absent, it defaults to 1, the start of the bytes.
 *
 * \c bytes can either be an array table with each byte as a separate entry or
 * a string.
 *
 * Returns the crc object.
 */
static int
l_crc16_method_calculate(lua_State* L)
{
    Crc16Ccitt* d = checkudata(L);

    calculate(L, 2, d);

    lua_settop(L, 1);
    return 1;
}

/**
 * Get the current checksum value.
 *
 *   checksum = crc:get()
 *
 * Returns the current CRC value (it is possible to keep calling update()
 * after this).
 */
static int
l_crc16_get(lua_State* L)
{
    Crc16Ccitt* d = checkudata(L);

    lua_pushinteger(L, d->getValue());

    return 1;
}

/**
 * Calculate checksum in a single call.
 *
 * Example:
 *
 *   checksum = crc(bytes, start, end)
 *
 * is a short form for
 *
 *   checksum = crc:reset():update(bytes, start, end):get()
 *
 * \see crc:update()
 */
static int
l_crc16_call(lua_State* L)
{
    l_crc16_reset(L);
    l_crc16_update(L);

    return l_crc16_get(L);
}

static int
l_crc16_function_calculate(lua_State* L)
{
    Crc16Ccitt crc;
    calculate(L, 1, &crc);

    lua_pushinteger(L, crc.getValue());
    return 1;
}

static int
l_crc16_gc(lua_State* L)
{
    Crc16Ccitt** d = (Crc16Ccitt **) luaL_checkudata(L, 1, "dlr.crc16");

    delete *d;
    *d = NULL;

    return 0;
}

// ----------------------------------------------------------------------------
static const struct luaL_Reg lib_functions[] = {
	{ "new", l_crc16_function_new },
	{ "calculate", l_crc16_function_calculate },
	{ NULL, NULL }
};

static const struct luaL_Reg lib_methods[] = {
	{ "reset", l_crc16_reset },
	{ "update", l_crc16_update },
	{ "calculate", l_crc16_method_calculate },
	{ "get", l_crc16_get },
	{ "__call", l_crc16_call},
	{ "__gc", l_crc16_gc},

	{ NULL, NULL },
};

// ----------------------------------------------------------------------------
extern "C"
int
luaopen_crc16(lua_State* L)
{
	luaL_newmetatable(L, "dlr.crc16");

	// metatable.__index = metatable
	lua_pushvalue(L, -1);	// duplicate the metatable
	lua_setfield(L, -2, "__index");

	luaL_setfuncs(L, lib_methods, 0);
	luaL_newlib(L, lib_functions);
	return 1;
}

