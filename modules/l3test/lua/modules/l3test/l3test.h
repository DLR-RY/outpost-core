/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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

#ifndef L3TEST_H
#define L3TEST_H

extern "C"
{
#include <lauxlib.h>

/**
 *
 *
 * \param L
 *      Lua State.
 *
 * \return
 */
int
luaopen_l3test(lua_State* L);
}

#endif // L3TEST_H
