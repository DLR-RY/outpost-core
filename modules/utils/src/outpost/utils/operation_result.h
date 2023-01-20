/*
 * Copyright (c) 2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2022, Tobias Pfeffer (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_OPERATION_RESULT_H
#define OUTPOST_UTILS_OPERATION_RESULT_H

namespace outpost
{
namespace utils
{
enum class OperationResult
{
    success,
    invalid,
    outOfBounds,
    inUse
};

}  // namespace utils
}  // namespace outpost

#endif  // OUTPOST_UTILS_OPERATION_RESULT_H