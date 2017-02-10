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

#include "testing_clock.h"

using namespace unittest::time;

TestingClock::TestingClock() :
	currentTime(cobc::time::SpacecraftElapsedTime::startOfEpoch())
{
}

TestingClock::~TestingClock()
{
}

cobc::time::SpacecraftElapsedTime
TestingClock::now() const
{
    return currentTime;
}

void
TestingClock::setTime(cobc::time::SpacecraftElapsedTime timePoint)
{
    currentTime = timePoint;
}
