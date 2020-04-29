/*
 * Copyright (c) 2014-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2018, Fabian Greif (DLR RY-AVS)
 */

#ifndef UNITTEST_CONFIGURABLE_EVENT_LISTENER_H
#define UNITTEST_CONFIGURABLE_EVENT_LISTENER_H

#include <gtest/gtest.h>

#include <stdio.h>

// exported from gtest.cc to provide colored output
namespace testing
{
namespace internal
{
enum GTestColor
{
    COLOR_DEFAULT,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW
};

void
ColoredPrintf(GTestColor color, const char* fmt, ...);
}  // namespace internal
}  // namespace testing

namespace unittest
{
class ConfigurableEventListener : public testing::TestEventListener
{
public:
    inline explicit ConfigurableEventListener(TestEventListener* theEventListener) :
        showTestCases(false),
        showTestNames(false),
        showSuccesses(false),
        showInlineFailures(true),
        showEnvironment(false),
        eventListener(theEventListener)
    {
    }

    virtual ~ConfigurableEventListener();

    virtual void
    OnTestProgramStart(const testing::UnitTest& unit_test)
    {
        eventListener->OnTestProgramStart(unit_test);
    }

    virtual void
    OnTestIterationStart(const testing::UnitTest& unit_test, int iteration)
    {
        eventListener->OnTestIterationStart(unit_test, iteration);
        printf("\n");
    }

    virtual void
    OnEnvironmentsSetUpStart(const testing::UnitTest& unit_test)
    {
        if (showEnvironment)
        {
            eventListener->OnEnvironmentsSetUpStart(unit_test);
        }
    }

    virtual void
    OnEnvironmentsSetUpEnd(const testing::UnitTest& unit_test)
    {
        if (showEnvironment)
        {
            eventListener->OnEnvironmentsSetUpEnd(unit_test);
        }
    }

    virtual void
    OnTestCaseStart(const testing::TestCase& test_case)
    {
        if (showTestCases)
        {
            eventListener->OnTestCaseStart(test_case);
        }
    }

    virtual void
    OnTestStart(const testing::TestInfo& test_info)
    {
        if (showTestNames)
        {
            eventListener->OnTestStart(test_info);
        }
    }

    virtual void
    OnTestPartResult(const testing::TestPartResult& result)
    {
        eventListener->OnTestPartResult(result);
    }

    virtual void
    OnTestEnd(const testing::TestInfo& test_info);

    virtual void
    OnTestCaseEnd(const testing::TestCase& test_case)
    {
        if (showTestCases)
        {
            eventListener->OnTestCaseEnd(test_case);
        }
    }

    virtual void
    OnEnvironmentsTearDownStart(const testing::UnitTest& unit_test)
    {
        if (showEnvironment)
        {
            eventListener->OnEnvironmentsTearDownStart(unit_test);
        }
    }

    virtual void
    OnEnvironmentsTearDownEnd(const testing::UnitTest& unit_test)
    {
        if (showEnvironment)
        {
            eventListener->OnEnvironmentsTearDownEnd(unit_test);
        }
    }

    virtual void
    OnTestIterationEnd(const testing::UnitTest& unit_test, int iteration)
    {
        eventListener->OnTestIterationEnd(unit_test, iteration);
    }

    virtual void
    OnTestProgramEnd(const testing::UnitTest& unit_test)
    {
        eventListener->OnTestProgramEnd(unit_test);
    }

    /**
     * Show the names of each test case.
     */
    bool showTestCases;
    /**
     * Show the names of each test.
     */
    bool showTestNames;
    /**
     * Show each success.
     */
    bool showSuccesses;
    /**
     * Show each failure as it occurs. You will also see it at the bottom after the full
     * suite is run.
     */
    bool showInlineFailures;
    /**
     * Show the setup of the global environment.
     */
    bool showEnvironment;

protected:
    TestEventListener* eventListener;
};

ConfigurableEventListener*
registerConfigurableEventListener(int argc = 0, char** argv = nullptr);

}  // namespace unittest

#endif  // UNITTEST_CONFIGURABLE_EVENT_LISTENER_H
