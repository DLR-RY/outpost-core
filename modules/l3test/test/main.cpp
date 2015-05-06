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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <unittest/configurable_event_listener.h>

#include <unittest/l3test/lua_path.h>

int
main(int argc, char** argv)
{
    // InitGoogleMock initializes Google test, too.
    ::testing::InitGoogleMock(&argc, argv);

    // Using the default listener from cobc/utils/unittest for customized gtest output
    unittest::registerConfigurableEventListener();

    cobc::l3test::LuaPath::setDefaultPathPrefix("./");

    return RUN_ALL_TESTS();
}
