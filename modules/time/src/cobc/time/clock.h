/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_TIME_CLOCK_H
#define COBC_TIME_CLOCK_H

#include "time_point.h"

namespace cobc
{
namespace time
{

/**
 * Clock interface.
 *
 * \author  Fabian Greif
 */
class Clock
{
public:
    Clock();

    virtual
    ~Clock();

    virtual TimePoint
    now() const = 0;
};

}
}

#endif // COBC_TIME_CLOCK_H
