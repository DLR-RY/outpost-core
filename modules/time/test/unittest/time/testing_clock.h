/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

    virtual
    ~TestingClock();

    virtual outpost::time::SpacecraftElapsedTime
    now() const;

    void
    setTime(outpost::time::SpacecraftElapsedTime);

private:
    outpost::time::SpacecraftElapsedTime currentTime;
};

}
}

#endif // UNITTEST_TIME_TESTING_CLOCK_H
