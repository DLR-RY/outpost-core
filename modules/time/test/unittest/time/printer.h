/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_TIME_PRINTER_H
#define OUTPOST_TIME_PRINTER_H

#include <outpost/time/date.h>
#include <outpost/time/duration.h>

#include <ostream>

namespace outpost
{
namespace time
{
::std::ostream&
operator<<(::std::ostream& os, const outpost::time::Duration& duration);

::std::ostream&
operator<<(::std::ostream& os, const outpost::time::Seconds& duration);

::std::ostream&
operator<<(::std::ostream& os, const outpost::time::SpacecraftElapsedTime& time);

::std::ostream&
operator<<(::std::ostream& os, const outpost::time::Date& date);

}  // namespace time
}  // namespace outpost

#endif
