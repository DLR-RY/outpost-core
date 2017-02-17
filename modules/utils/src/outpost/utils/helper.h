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
 * - 2014, Norbert Toth (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_HELPER_H
#define COBC_UTILS_HELPER_H

#if __cplusplus > 199711L
// Nothing to do here, C++11 has support for static_assert
#else
// HACK: Provide an empty implementation when compiling without C++11 support.
#define static_assert(test, msg)

#endif

#endif // COBC_UTILS_HELPER_H
