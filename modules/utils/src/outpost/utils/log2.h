/*
 * Copyright (c) 2015-2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_LOG2_H_
#define OUTPOST_UTILS_LOG2_H_

#include <assert.h>
#include <stddef.h>

namespace outpost
{
constexpr size_t
Log2(size_t n)
{
    return ((n < 2) ? 0 : 1 + Log2(n / 2));
}

}  // namespace outpost

#endif /* OUTPOST_UTILS_LOG2_H_ */
