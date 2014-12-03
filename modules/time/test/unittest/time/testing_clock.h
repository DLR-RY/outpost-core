/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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

#ifndef UNITTEST_TIME_TESTING_CLOCK_H
#define UNITTEST_TIME_TESTING_CLOCK_H

#include <cobc/time/clock.h>

namespace unittest
{
namespace time
{

/**
 * Clock interface.
 *
 * \author  Fabian Greif
 */
class TestingClock : public cobc::time::Clock
{
public:
    TestingClock();

    virtual
    ~TestingClock();

    virtual cobc::time::TimePoint
    now() const;

    void
    setTime(cobc::time::TimePoint);

private:
    cobc::time::TimePoint currentTime;
};

}
}

#endif // UNITTEST_TIME_TESTING_CLOCK_H
