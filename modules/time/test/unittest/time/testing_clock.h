/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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
