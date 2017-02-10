/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#include "configurable_event_listener.h"

unittest::ConfigurableEventListener*
unittest::registerConfigurableEventListener()
{
    // Remove the default listener
    testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
    auto defaultPrinter = listeners.Release(listeners.default_result_printer());

    // Add our listener, by default everything is off, like:
    // [==========] Running 149 tests from 53 test cases.
    // [==========] 149 tests from 53 test cases ran. (1 ms total)
    // [ PASSED ] 149 tests.
    ConfigurableEventListener* listener = new ConfigurableEventListener(defaultPrinter);
    listeners.Append(listener);

    return listener;
}
