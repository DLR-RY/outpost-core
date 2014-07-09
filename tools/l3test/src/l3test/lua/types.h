/*
 * Copyright (c) 2010, Fabian Greif
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of the copyright holder nor the
 *   names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LUA_TYPES_H
#define LUA_TYPES_H

#include <sstream>
#include <stdint.h>

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
}

namespace lua
{
	enum class Type : int
	{
		NONE = LUA_TNONE,
		STRING = LUA_TSTRING,
		NUMBER = LUA_TNUMBER,
		THREAD = LUA_TTHREAD,
		BOOLEAN = LUA_TBOOLEAN,
		FUNCTION = LUA_TFUNCTION,
		USERDATA = LUA_TUSERDATA,
		LIGHTUSERDATA = LUA_TLIGHTUSERDATA,
		TABLE = LUA_TTABLE
	};

	inline void
	assertType(lua_State* L, int index, Type expectedType)
	{
		int t = lua_type(L, index);
		if (t != static_cast<int>(expectedType)) {
			luaL_error(L, "expected %s, got %s",
			           lua_typename(L, static_cast<int>(expectedType)),
			           lua_typename(L, t));
		}
	}

	template<typename T>
	inline Type
	typeOf(lua_State*)
	{
		return Type::NONE;
	}

	class Table;

	template<>
	inline Type
	typeOf<Table>(lua_State*)
	{
		return Type::TABLE;
	}

	template<>
	inline Type
	typeOf<bool>(lua_State*)
	{
		return Type::BOOLEAN;
	}

	template<>
	inline Type
	typeOf<const char*>(lua_State*)
	{
		return Type::STRING;
	}

	template<>
	inline Type
	typeOf<std::string>(lua_State*)
	{
		return Type::STRING;
	}

	template<>
	inline Type
	typeOf<float>(lua_State*)
	{
		return Type::NUMBER;
	}

	template<>
	inline Type
	typeOf<lua_Number>(lua_State*)
	{
		return Type::NUMBER;
	}

	template<>
	inline Type
	typeOf<int32_t>(lua_State*)
	{
		return Type::NUMBER;
	}

	template<>
	inline Type
	typeOf<int64_t>(lua_State*)
	{
		return Type::NUMBER;
	}
}

#include "table.h"

#endif	// LUA_TYPES_H
