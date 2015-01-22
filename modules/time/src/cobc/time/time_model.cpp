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

#include "time_model.h"

#include <cobc/utils/limits.h>

using cobc::time::TimeModel;

cobc::time::TimePoint
TimeModel::endOfEpoch()
{
    return TimePoint(cobc::Limits<TimePoint::Type>::max / 2 - 1);
}

cobc::time::TimePoint
TimeModel::startOfEpoch()
{
    return TimePoint(0);
}
