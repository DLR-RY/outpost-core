/*
 * Copyright (c) 2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef OUTPOST_BASE_ENUM_TRAIT_H_
#define OUTPOST_BASE_ENUM_TRAIT_H_

namespace outpost
{
template <typename T>
struct EnumTrait
{
    static constexpr T
    min() = 0;
    static constexpr T
    max() = 0;

    static bool
    isValid(const T& t) = 0;
};

}  // namespace outpost

#endif /* EXT_OUTPOST_CORE_MODULES_BASE_SRC_OUTPOST_BASE_ENUM_TRAIT_H_ */
