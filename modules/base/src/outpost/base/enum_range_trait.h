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
/**
 * Trait class for enums that require a range check.
 *
 * \author  Jan-Gerd Mess
 */
template <typename T>
struct EnumRangeTrait
{
    /**
     * Return the minimum integer value allowed for the underlying enum.
     */
    static constexpr T
    min();

    /**
     * Return the maximum integer value allowed for the underlying enum.
     */
    static constexpr T
    max();

    /**
     * Check the given enum value for validity.
     */
    static constexpr bool
    isValid(const T& t);
};

/**
 * Trivial check for enum validity.
 * \author  Jan-Gerd Mess
 */
template<typename T>
inline constexpr bool
isValidEnum(const T& t)
{
	return t >= EnumRangeTrait<T>::min() && t <= EnumRangeTrait<T>::max();
}

}  // namespace outpost

#endif
