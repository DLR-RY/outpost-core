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

    virtual ~TestingClock() = default;

    virtual outpost::time::SpacecraftElapsedTime
    now() const;

    /**
     * Set the current time to an absolute value.
     */
    void
    setTime(outpost::time::SpacecraftElapsedTime time);

    /**
     * Increment the current time by the given duration.
     *
     * The duration can be negative.
     */
    void
    incrementBy(outpost::time::Duration time);

private:
    outpost::time::SpacecraftElapsedTime currentTime;
};

}  // namespace time
}  // namespace unittest

#endif  // UNITTEST_TIME_TESTING_CLOCK_H
