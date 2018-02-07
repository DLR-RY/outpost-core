/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "testing_clock.h"

using namespace unittest::time;

TestingClock::TestingClock() : currentTime(outpost::time::SpacecraftElapsedTime::startOfEpoch())
{
}

TestingClock::~TestingClock()
{
}

outpost::time::SpacecraftElapsedTime
TestingClock::now() const
{
    return currentTime;
}

void
TestingClock::setTime(outpost::time::SpacecraftElapsedTime timePoint)
{
    currentTime = timePoint;
}
