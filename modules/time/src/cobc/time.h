/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
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
 * cobc::TimeModel is responsible for converting between local Time and UTC,
 * while spp::Time is used to convert between CDS and UTC and CUC and
 * local time.
 */

#include "time/duration.h"
#include "time/time_point.h"
#include "time/time_epoch.h"

namespace cobc
{
    /**
     * \ingroup    time
     */
    namespace time
    {
    }
}

#endif // COBC_TIME_H
