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
