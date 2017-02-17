/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_TIME_H
#define COBC_TIME_H

/**
 * \defgroup    time
 * \brief        Time management
 *
 * In the satellite there are different time representations. TM/TC use
 * CUC (CCSDS Unsegmented time Code) and UTC encoded as CDS (CCSDS Day
 * Segmented time code) while the satellite itself uses a local time
 * representation for calculations. This local time lacks for example a
 * leap-second correction but is very efficient for time calculations.
 *
 * The local time is simply the number of seconds from the start of the
 * satellite.
 *
 * outpost::TimeModel is responsible for converting between local Time and UTC,
 * while spp::Time is used to convert between CDS and UTC and CUC and
 * local time.
 */

#include "time/duration.h"
#include "time/time_point.h"
#include "time/time_epoch.h"

namespace outpost
{
    /**
     * \ingroup    time
     */
    namespace time
    {
    }
}

#endif // COBC_TIME_H
