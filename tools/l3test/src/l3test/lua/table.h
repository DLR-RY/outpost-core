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

#ifndef LUA_TABLE_H
#define LUA_TABLE_H

extern "C"
{
#include <lua.h>
}

#include "state.h"
#include "types.h"
#include "stack.h"

namespace lua
{
	class Interpreter;

	template<typename T>
	inline void
	checkType(lua_State* L, int index)
	{
		assertType(L, index, typeOf<T>(L));
	}

	class Table
	{
	public:
		/// Create a reference to a table on the stack
		Table(const State& L, int index = -1);

		~Table();

		/// Copy constructor
		Table(const Table& other);

		const Table&
		operator = (const Table& other);

		/**
		 * Look up variable in table.
		 *
		 * \tparam T
		 *     Type of the variable.
		 * \tparam U
		 *     Type of the key (e.g. string or number).
		 * \param  key
		 *     Key under which the variable is stored.
		 * \return Value of the variable.
		 *
		 * \throw  lua::Error
		 *     Thrown if T or U are not convertible to Lua types (see
		 *     table::index).
		 */
		template <typename T, typename U>
		T
		get(U key) const
		{
			this->pushReference();

			pushValue(this->L, key);
			lua_gettable(this->L, -2);

			checkType<T>(this->L, -1);

			T t;
			getValue(this->L, -1, t);

			lua_pop(this->L, 2);
			return t;
		}

		/**
		 * Set variable in table.
		 *
		 * \param	key		variable key
		 * \param	value	variable value
		 *
		 * \throw	lua::Error 	thrown if T or U are not convertible to
		 * 						Lua types (see table::index)
		 */
		template<typename T, typename U>
		void set(T key, U value)
		{
			this->pushReference();

			pushValue(this->L, key);
			pushValue(this->L, value);

			lua_settable(this->L, -3);
			lua_pop(this->L, 1);
		}

		/// Get size of the table (only numerical indices)
		int
		getSize() const;

	protected:
		Table();

	private:
		/// Load the reference for this table
		void
		pushReference() const;

		int
		copyReference() const;

		mutable State L;
		int ref;
	};
}

#endif // LUA_TABLE_H
