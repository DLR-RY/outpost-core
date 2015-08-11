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

#ifndef COBC_L3TEST_LUA_PATH_H
#define COBC_L3TEST_LUA_PATH_H

#include <string>

namespace cobc
{
namespace l3test
{

/**
 * Global setting for the Lua path prefix used during unit-tests.
 *
 * Must be applied to the Lua engine by hand:
 * \code
 * Engine engine;
 * engine.appendDefaultLuaPath(cobc::l3test::LuaPath::getDefaultPathPrefix());
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
