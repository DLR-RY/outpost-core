/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "l3test.h"

#include "channel.h"

static const struct luaL_Reg arraylib_f[] = {
    { "channel", l3test_channel_new },
    { NULL, NULL }
};

extern "C"
int
luaopen_l3test(lua_State* L)
{
    l3test_channel_open(L);

    luaL_newlib(L, arraylib_f);
    return 1;
}
