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

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "interpreter.h"

// ----------------------------------------------------------------------------
lua::Interpreter::Interpreter() :
	L(0), _G(0)
{
	L = new State();

	lua_pushglobaltable(*L);
	_G = new Table(*L, -1);
	lua_pop(*L, 1);
}

void
lua::Interpreter::openLibraries()
{
	luaL_openlibs(*L);
}

void
lua::Interpreter::reset()
{
	delete _G;
	delete L;

	L = new State();
	lua_pushglobaltable(*L);
	_G = new Table(*L, -1);
	lua_pop(*L, 1);

	this->openLibraries();
}

// ----------------------------------------------------------------------------
void
lua::Interpreter::doString(const char* code)
{
	int error = luaL_dostring(*L, code);

	if (error) {
		lua_error(*L);
	}
}

void
lua::Interpreter::doFile(const char* path)
{
	int error = luaL_dofile(*L, path);

	if (error) {
		lua_error(*L);
	}
}

// ----------------------------------------------------------------------------
lua::Table&
lua::Interpreter::globals()
{
	return *_G;
}

const lua::Table&
lua::Interpreter::globals() const
{
	return *_G;
}
