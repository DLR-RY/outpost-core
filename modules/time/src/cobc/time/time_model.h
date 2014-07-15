/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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

private:
    // disable unneeded implicitly member functions
    TimeModel();

    ~TimeModel();

    TimeModel(const TimeModel&);

    TimeModel&
    operator =(const TimeModel&);
};

}
}

#endif // COBC_TIME_TIME_MODEL_H
