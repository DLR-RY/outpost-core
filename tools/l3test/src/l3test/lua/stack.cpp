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

#include <string>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "state.h"
#include "stack.h"

// ----------------------------------------------------------------------------
// from Lua
void
lua::getValue(State& L, int index, Table& r)
{
	r = Table(L, index);
}

void
lua::getValue(State& L, int index, int32_t& r)
{
	r = lua_tointeger(L, index);
}

void
lua::getValue(State& L, int index, int64_t& r)
{
	r = lua_tointeger(L, index);
}

void
lua::getValue(State& L, int index, lua_Number& r)
{
	r = lua_tonumber(L, index);
}

void
lua::getValue(State& L, int index, float& r)
{
	r = lua_tonumber(L, index);
}

void
lua::getValue(State& L, int index, bool& r)
{
	r = lua_toboolean(L, index);
}

void
lua::getValue(State& L, int index, const char*& r)
{
	r = lua_tostring(L, index);
}

void
lua::getValue(State& L, int index, const char*& r, size_t& len)
{
	r = lua_tolstring(L, index, &len);
}

void
lua::getValue(State& L, int index, std::string& r)
{
	size_t len;
	const char* s = lua_tolstring(L, index, &len);
	r = std::string(s, len);
}

// ----------------------------------------------------------------------------
// to Lua
void
lua::pushValue(State& L, int32_t i)
{
	lua_pushinteger(L, i);
}

void
lua::pushValue(State& L, int64_t i)
{
	lua_pushinteger(L, i);
}

void
lua::pushValue(State& L, lua_Number n)
{
	lua_pushnumber(L, n);
}

void
lua::pushValue(State& L, float n)
{
	lua_pushnumber(L, n);
}

void
lua::pushValue(State& L, bool b)
{
	lua_pushboolean(L, b);
}

void
lua::pushValue(State& L, const char* s)
{
	lua_pushstring(L, s);
}

void
lua::pushValue(State& L, const char* s, size_t len)
{
	lua_pushlstring(L, s, len);
}

void
lua::pushValue(State& L, const std::string& str)
{
	lua_pushlstring(L, str.c_str(), str.length());
}
