/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "printer.h"

#include <iomanip>

::std::ostream&
outpost::time::operator<<(::std::ostream& os, const outpost::time::Duration& duration)
{
    return os << duration.microseconds() << " us";
}

::std::ostream&
outpost::time::operator<<(::std::ostream& os, const outpost::time::Seconds& duration)
{
    return os << duration.microseconds() << " us";
}

::std::ostream&
outpost::time::operator<<(::std::ostream& os, const outpost::time::SpacecraftElapsedTime& time)
{
    return os << time.timeSinceEpoch().microseconds() << " us";
}

::std::ostream&
outpost::time::operator<<(::std::ostream& os, const outpost::time::Date& date)
{
    os << static_cast<int>(date.year) << "-";
    os << std::setfill('0') << std::setw(2) << static_cast<int>(date.month) << "-";
    os << std::setfill('0') << std::setw(2) << static_cast<int>(date.day) << "T";
    os << std::setfill('0') << std::setw(2) << static_cast<int>(date.hour) << "-";
    os << std::setfill('0') << std::setw(2) << static_cast<int>(date.minute) << "-";
    os << std::setfill('0') << std::setw(2) << static_cast<int>(date.second);

    return os;
}
