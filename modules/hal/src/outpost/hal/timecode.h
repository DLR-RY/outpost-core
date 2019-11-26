/*
 * Copyright (c) 2013-2019, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_HAL_TIMECODE_H_
#define OUTPOST_HAL_TIMECODE_H_

#include <stdint.h>

namespace outpost
{
namespace hal
{
struct TimeCode
{
    // data contained in the corresponding lower bits.
    uint8_t mValue;    // 6-bits
    uint8_t mControl;  // 2-bits
};
}  // namespace hal
}  // namespace outpost

#endif /* MODULES_HAL_SRC_OUTPOST_HAL_TIMECODE_H_ */
