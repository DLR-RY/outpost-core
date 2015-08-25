/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include <cobc/time/duration.h>
#include <cobc/time/date.h>

#include <ostream>

::std::ostream&
operator<<(::std::ostream& os, const cobc::time::Duration& duration);

::std::ostream&
operator<<(::std::ostream& os, const cobc::time::Seconds& duration);

::std::ostream&
operator<<(::std::ostream& os, const cobc::time::Date& date);
