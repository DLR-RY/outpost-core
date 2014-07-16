/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "testing_clock.h"

using namespace unittest::time;

TestingClock::TestingClock() :
        currentTime()
{
}

TestingClock::~TestingClock()
{
}

cobc::time::TimePoint
TestingClock::now() const
{
    return currentTime;
}

void
TestingClock::setTime(cobc::time::TimePoint timePoint)
{
    currentTime = timePoint;
}
