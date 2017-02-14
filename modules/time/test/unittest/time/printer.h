/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#ifndef COBC_TIME_PRINTER_H
#define COBC_TIME_PRINTER_H

#include <outpost/time/duration.h>
#include <outpost/time/date.h>

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

}
}

#endif
