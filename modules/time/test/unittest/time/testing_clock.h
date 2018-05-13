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

#ifndef UNITTEST_TIME_TESTING_CLOCK_H
#define UNITTEST_TIME_TESTING_CLOCK_H

#include <outpost/time/clock.h>

namespace unittest
{
namespace time
{
/**
 * Clock interface.
 *
 * \author  Fabian Greif
 */
class TestingClock : public outpost::time::Clock
{
public:
    TestingClock();

    virtual ~TestingClock();

    virtual outpost::time::SpacecraftElapsedTime
    now() const;

    void setTime(outpost::time::SpacecraftElapsedTime);

private:
    outpost::time::SpacecraftElapsedTime currentTime;
};

}  // namespace time
}  // namespace unittest

#endif  // UNITTEST_TIME_TESTING_CLOCK_H
