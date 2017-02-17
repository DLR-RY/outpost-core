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
// ----------------------------------------------------------------------------

#ifndef COBC_L3TEST_LUA_PATH_H
#define COBC_L3TEST_LUA_PATH_H

#include <string>

namespace outpost
{
namespace l3test
{

/**
 * Global setting for the Lua path prefix used during unit-tests.
 *
 * Must be applied to the Lua engine by hand:
 * \code
 * Engine engine;
 * engine.appendDefaultLuaPath(outpost::l3test::LuaPath::getDefaultPathPrefix());
 * \endcode
 */
class LuaPath
{
public:
    static void
    setDefaultPathPrefix(std::string prefix);

    static std::string
    getDefaultPathPrefix();

private:
    static std::string defaultPrefix;
};

}
}

#endif // COBC_L3TEST_LUA_PATH_H
