/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_HELPER_H
#define COBC_UTILS_HELPER_H

#if __cplusplus > 199711L

#else
// Provide an empty implementation when compiling without C++11 support.
static inline void
static_asssert(bool, const char*)
{
}
#endif

#endif // COBC_UTILS_HELPER_H
