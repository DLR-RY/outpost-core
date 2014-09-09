/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "time_model.h"

cobc::time::TimePoint
cobc::time::TimeModel::endOfEpoch()
{
    //FIXME: do something more sophisticated to determine the upper limit
    return TimePoint(4611686018427387903);
}

cobc::time::TimePoint
cobc::time::TimeModel::startOfEpoch()
{
    return TimePoint(0);
}
