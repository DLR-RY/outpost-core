/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
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

private:
    // disable copy constructor
    Clock(const Clock&);

    // disable assignment operator
    Clock&
    operator=(const Clock&);
};

}
}

#endif // COBC_TIME_CLOCK_H
