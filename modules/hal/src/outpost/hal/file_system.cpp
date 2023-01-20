/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
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

#include "file_system.h"

namespace outpost
{
namespace hal
{
const FileSystem::Permission FileSystem::R(0x1);
const FileSystem::Permission FileSystem::W(0x2);
const FileSystem::Permission FileSystem::X(0x4);

const FileSystem::OpenMask FileSystem::READ(0x1);
const FileSystem::OpenMask FileSystem::WRITE(0x2);
const FileSystem::OpenMask FileSystem::EXECUTE(0x4);
const FileSystem::OpenMask FileSystem::APPEND(0x8);
const FileSystem::OpenMask FileSystem::CREATE(0x10);

}  // namespace hal
}  // namespace outpost
