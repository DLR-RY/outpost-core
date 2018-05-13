/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */

#include "lua_path.h"

using outpost::l3test::LuaPath;

std::string LuaPath::defaultPrefix(".");

void
LuaPath::setDefaultPathPrefix(std::string prefix)
{
    defaultPrefix = prefix;
}

std::string
LuaPath::getDefaultPathPrefix()
{
    return defaultPrefix;
}
