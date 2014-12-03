/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#ifndef COBC_TIME_TIME_MODEL_H
#define COBC_TIME_TIME_MODEL_H

#include "time_point.h"

namespace cobc
{
namespace time
{

/**
 * Time model used to convert between different time representations.
 *
 * \ingroup time
 * \author  Fabian Greif
 */
class TimeModel
{
public:
    static TimePoint
    endOfEpoch();

    static TimePoint
    startOfEpoch();

private:
    // disable unneeded implicitly member functions
    TimeModel();

    ~TimeModel();

    TimeModel(const TimeModel&);

    TimeModel&
    operator=(const TimeModel&);
};

}
}

#endif // COBC_TIME_TIME_MODEL_H
