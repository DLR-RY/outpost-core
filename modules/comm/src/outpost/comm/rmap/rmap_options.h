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

#ifndef OUTPOST_COMM_RMAP_RMAP_OPTIONS_H_
#define OUTPOST_COMM_RMAP_RMAP_OPTIONS_H_

#include <stdint.h>

namespace outpost
{
namespace comm
{
struct RMapOptions
{
    constexpr RMapOptions() : mIncrementMode(true), mVerifyMode(true), mReplyMode(true){};
    constexpr RMapOptions(bool increment, bool verify, bool reply) :
        mIncrementMode(increment),
        mVerifyMode(verify),
        mReplyMode(reply){};

    bool mIncrementMode;
    bool mVerifyMode;
    bool mReplyMode;
};

}  // namespace comm
}  // namespace outpost

#endif /* MODULES_COMM_SRC_OUTPOST_COMM_RMAP_RMAP_OPTIONS_H_ */
