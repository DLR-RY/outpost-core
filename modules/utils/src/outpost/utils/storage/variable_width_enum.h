/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 * All rights reserved.
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_STORAGE_VARIABLE_WIDTH_ENUM_H_
#define OUTPOST_UTILS_STORAGE_VARIABLE_WIDTH_ENUM_H_

#include "variable_width_integer.h"

#include <type_traits>

namespace outpost
{
namespace utils
{
template <typename T, size_t bits>
class Enumeration;

/**
 * Used to check whether a type is a variable width enumeration
 */
class EnumerationBase
{
    template <typename T, size_t bits>
    friend class Enumeration;

private:
    /*
     * Only friends shall be childs
     */
    EnumerationBase() = default;
};

/**
 * A class to store a enum value within a limit amount of bits
 * User must ensure that the value fits the bit limit
 *
 * @param T the enumeration it shall encapsulate
 * @param bits number of bits, must be between 1 - 64
 */
template <typename T, size_t bits = 32>
class Enumeration : private EnumerationBase
{
public:
    /**
     * For the cases you just need a object of the type
     */
    Enumeration() = default;

    /**
     * Generates from a enum
     * @param data The enum to load
     */
    // cppcheck-suppress noExplicitConstructor
    Enumeration(const T data) : mData(0)
    {
        static_assert(std::is_enum<T>::value,
                      "outpost::utils::enumeration construction source not an enum");
        mData = static_cast<uint64_t>(data);
    }

    /**
     * Create from raw data
     */
    explicit Enumeration(const UInteger<bits>& data) : mData(data)
    {
    }

    /**
     * @return the bit width of this type
     */
    static constexpr size_t
    getBits()
    {
        return bits;
    }

    /**
     * Sets from a enum
     * @param data The enum to load
     */
    Enumeration
    operator=(const T data)
    {
        mData = static_cast<typename UInteger<bits>::PrimitiveType>(data);
        return *this;
    }

    bool
    operator==(const Enumeration& other) const
    {
        return mData == other.mData;
    }

    bool
    operator!=(const Enumeration& other) const
    {
        return mData != other.mData;
    }

    explicit operator UInteger<bits>() const
    {
        return mData;
    }

    /**
     * Transforms into a given enum, explicit so that no unexpected translation occurs
     */
    explicit operator T() const
    {
        return static_cast<T>(static_cast<typename UInteger<bits>::PrimitiveType>(mData));
    }

private:
    UInteger<bits> mData;
};

/**
 * Self defined type trait to check whether a type is a variable width enumeration
 */
template <typename T>
using is_variable_width_enumeration = std::is_base_of<EnumerationBase, T>;

}  // namespace utils
}  // namespace outpost

#endif
