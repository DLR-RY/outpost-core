/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unittest/configurable_event_listener.h>
#include <unittest/l3test/lua_path.h>

int
main(int argc, char** argv)
{
    // InitGoogleMock initializes Google test, too.
    ::testing::InitGoogleMock(&argc, argv);

    // Using the default listener from outpost/utils/unittest for customized gtest output
    unittest::registerConfigurableEventListener();

    outpost::l3test::LuaPath::setDefaultPathPrefix("./");

    return RUN_ALL_TESTS();
}
