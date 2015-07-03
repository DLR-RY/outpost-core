/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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

#ifndef COBC_RTOS_FREERTOS_CLOCK_H
#define COBC_RTOS_FREERTOS_CLOCK_H

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
    virtual time::SpacecraftElapsedTimePoint
    now() const;
};

}
}

#endif // COBC_RTOS_FREERTOS_CLOCK_H
