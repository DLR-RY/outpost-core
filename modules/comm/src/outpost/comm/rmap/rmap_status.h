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
 * - 2017, Muhammad Bassam (DLR RY-AVS)
 */

#ifndef OUTPOST_COMM_RMAP_STATUS_H_
#define OUTPOST_COMM_RMAP_STATUS_H_

#include <stdint.h>

//#define DEBUG_EN
#ifdef DEBUG_EN
#include <stdio.h>
#define console_out printf
#else
#define console_out(...)
#endif

namespace outpost
{
namespace comm
{
class RmapReplyStatus
{
public:
    enum ErrorStatusCodes
    {
        commandExecutedSuccessfully = 0,
        generalErrorCode = 1,
        unusedRmapPacketType = 2,
        invalidKey = 3,
        invalidDataCrc = 4,
        earlyEOP = 5,
        tooMuchData = 6,
        eep = 7,
        verifyBufferOverrun = 9,
        rmapCommandNotImplemented = 10,
        rmwDataLengthError = 11,
        invalidTargetLogicalAddress = 12,
        unknown = 255
    };

    static void
    replyStatus(uint8_t status);
};
}  // namespace comm
}  // namespace outpost

#endif /* MODULES_HAL_PROTOCOL_RMAP_RMAP_STATUS_H_ */
