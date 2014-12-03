/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.3-pre.
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
#include <unittest/configurable_event_listener.h>

int
main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    unittest::registerConfigurableEventListener();

    return RUN_ALL_TESTS();
}
