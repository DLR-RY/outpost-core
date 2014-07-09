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

#ifndef LUA_STACK_H
#define LUA_STACK_H

#include <string>
#include <stdint.h>

extern "C"
{
#include <lua.h>
}

#include "state.h"
#include "types.h"

namespace lua
{
	class Table;

	// from Lua
	void getValue(State& L, int index, Table& r);
	void getValue(State& L, int index, int32_t& r);
	void getValue(State& L, int index, int64_t& r);
	void getValue(State& L, int index, lua_Number& r);
	void getValue(State& L, int index, float& r);
	void getValue(State& L, int index, bool& r);
	void getValue(State& L, int index, const char*& r);
	void getValue(State& L, int index, const char*& r, size_t& len);
	void getValue(State& L, int index, std::string& r);

	template<typename T>
	inline void getArg(State& L, int narg, T& r)
	{
		int type = typeOf<T>(L);
		int t = lua_type(L, narg);
		if (t != type) {
			const char *msg = lua_pushfstring(L, "%s expected, got %s",
			                                  lua_typename(L, type),
			                                  luaL_typename(L, narg));
			luaL_argerror(L, narg, msg);
		}
		getValue(L, narg, r);
	}

	// to Lua
	void pushValue(State& L, int32_t i);
	void pushValue(State& L, int64_t n);
	void pushValue(State& L, lua_Number n);
	void pushValue(State& L, float i);
	void pushValue(State& L, bool b);
	void pushValue(State& L, const char* s);
	void pushValue(State& L, const char* s, size_t len);
	void pushValue(State& L, const std::string& str);
}

#endif // LUA_STACK_H
