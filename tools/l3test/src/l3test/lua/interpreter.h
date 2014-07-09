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

#ifndef LUA_INTERPRETER_H
#define LUA_INTERPRETER_H

#include "exception.h"
#include "state.h"
#include "table.h"

namespace lua
{
	class Interpreter
	{
	public:
		/**
		 * Create a new interpreter instance.
		 *
		 * This does not import the built-in libraries.
		 *
		 * @see	openLibraries()
		 */
		Interpreter();

		/**
		 * @brief	Load built-in libraries
		 */
		void
		openLibraries();

		void
		reset();

		/**
		 * Run a string of Lua.
		 *
		 * @param  code
		 *     Lua code to execute.
		 * @throw  lua::Error
		 *     Thrown for syntax errors and uncaught runtime errors.
		 */
		void
		doString(const char* code);

		/**
		 * Run a Lua script.
		 *
		 * @param  path
		 *     Path to file to execute.
		 * @throw  lua::Error
		 *     Thrown for syntax errors and uncaught runtime errors.
		 */
		void
		doFile(const char* path);

		/**
		 * Get global table.
		 *
		 * @return	Reference to the global environment
		 */
		Table&
		globals();

		/**
		 * Get global table.
		 *
		 * @return	Immutable reference to the global environment
		 */
		const Table&
		globals() const;

		/// Get global variable
		template<typename T, typename U>
		T get(U key) const
		{
			return _G->get<T>(key);
		}

		/// Set global variable
		template<typename T, typename U>
		void set(T key, U value)
		{
			_G->set(key, value);
		}

		inline State&
		getState()
		{
			return *this->L;
		}

	private:
		State* L;
		Table* _G;
	};
}

#endif // LUA_INTERPRETER_H
