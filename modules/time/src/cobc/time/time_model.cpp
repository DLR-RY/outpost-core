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
    return TimePoint(-1);
}
