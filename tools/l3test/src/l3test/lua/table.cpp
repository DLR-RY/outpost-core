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

#include <iostream>

#include "table.h"

// ----------------------------------------------------------------------------
lua::Table::Table() :
	L(), ref(LUA_NOREF)
{
}

lua::Table::Table(const State& L, int index) :
	L(L)
{
	if (!lua_istable(this->L, index)) {
		luaL_error(this->L, "no table at index '%f'", static_cast<lua_Number>(index));
	}
	lua_pushvalue(this->L, index);
	this->ref = luaL_ref(this->L, LUA_REGISTRYINDEX);
}

lua::Table::~Table()
{
	luaL_unref(L, LUA_REGISTRYINDEX, this->ref);
}

lua::Table::Table(const Table& other) :
	L(other.L), ref(other.copyReference())
{
}

const lua::Table&
lua::Table::operator = (const Table& other)
{
	this->L = other.L;
	this->ref = other.copyReference();

	return *this;
}

// ----------------------------------------------------------------------------
int
lua::Table::getSize() const
{
	this->pushReference();
	int n = lua_rawlen(this->L, -1);
	lua_pop(this->L, 1);

	return n;
}

// ----------------------------------------------------------------------------
void
lua::Table::pushReference() const
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, this->ref);
}

int
lua::Table::copyReference() const
{
	this->pushReference();

	// create a new reference for this table
	return luaL_ref(L, LUA_REGISTRYINDEX);
}
