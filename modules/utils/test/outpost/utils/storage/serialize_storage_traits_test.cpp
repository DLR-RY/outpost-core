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
 * - 2019, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/utils/storage/serialize.h>
#include <outpost/utils/storage/serialize_storage_traits.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace outpost;

namespace test
{
enum class ControlMode : uint8_t
{
    automatic = 1,
    manual = 2
};
}

namespace outpost
{
using namespace test;

// Use uint8_t as storage type for `ControlMode`
template <>
struct SerializeBigEndianTraits<ControlMode> : public SerializeStorageTraits<ControlMode, uint8_t>
{
};
}  // namespace outpost

TEST(SerializeStorageTraitsTest, store)
{
    // Prepare stream in which the data should be written.
    std::array<uint8_t, 20> buffer;
    Serialize stream(outpost::asSlice(buffer));

    // Stores the enum as a single byte.
    stream.store<ControlMode>(ControlMode::manual);

    EXPECT_EQ(1, stream.getPosition());
    EXPECT_EQ(2, buffer[0]);
}
