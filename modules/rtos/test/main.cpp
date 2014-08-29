/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
