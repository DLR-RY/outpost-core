/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_RETMS_CLOCK_H
#define COBC_RTOS_RETMS_CLOCK_H

#include <cobc/time/time_point.h>
#include <cobc/time/clock.h>

namespace cobc
{
namespace rtos
{

/**
 *
 * \ingroup    rtos
 */
class SystemClock : public time::Clock
{
public:
    virtual time::TimePoint
    now() const;
};

}
}

#endif // COBC_RTOS_RETMS_CLOCK_H
