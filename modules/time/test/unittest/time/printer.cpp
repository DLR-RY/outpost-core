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

#include <iomanip>
#include "printer.h"

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
