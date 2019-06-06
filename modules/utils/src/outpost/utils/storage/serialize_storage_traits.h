/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_SERIALIZE_STORAGE_TRAITS_H
#define OUTPOST_UTILS_SERIALIZE_STORAGE_TRAITS_H

#include "serialize_traits.h"

#include <stdint.h>

namespace outpost
{
/**
 * Base class to define the storage type a by converting it to another type.
 *
 * It must be possible to convert between input type and storage type by
 * static_cast.
 *
 * Example:
 * ```
 * // Define type which should be stored
 * namespace system
 * {
 * enum class ControlMode
 * {
 *     automatic = 1,
 *     manual    = 2
 * };
 * }
 *
 * // Use uint8_t as storage type for ControlMode::Type.
 * namespace outpost
 * {
 * using namespace system;
 *
 * template<>
 * struct SerializeBigEndianTraits<ControlMode> :
 *          public SerializeStorageTraits<ControlMode, uint8_t>
 * {
 * };
 *
 * }
 *
 * // Prepare stream in which the data should be written.
 * std::array<uint8_t, 20> buffer;
 * Serialize stream(outpost::asSlice(buffer));
 *
 * // Stores the enum as a single byte.
 * stream.store<ControlMode>(ControlMode::automatic);
 * ```
 *
 * \author  Fabian Greif
 */
template <typename InputType,
          typename StorageType,
          typename Traits = SerializeBigEndianTraits<StorageType>>
struct SerializeStorageTraits
{
    static void
    store(uint8_t*& buffer, InputType value)
    {
        Traits::store(buffer, static_cast<StorageType>(value));
    }

    static InputType
    read(const uint8_t*& buffer)
    {
        StorageType value = Traits::read(buffer);
        return static_cast<InputType>(value);
    }

    static InputType
    peek(const uint8_t* const& buffer, size_t offset)
    {
        StorageType value = Traits::peek(buffer, offset);
        return static_cast<InputType>(value);
    }

    static inline size_t
    size()
    {
        return Traits::size();
    }
};

}  // namespace outpost

#endif
