/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Muhammad Bassam (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_COMM_RMAP_STATUS_H_
#define OUTPOST_COMM_RMAP_STATUS_H_

#include <stdint.h>

//#define DEBUG_EN

#ifdef DEBUG_EN
#include <stdio.h>
#define console_out   printf
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
    replyStatus(uint8_t status)
    {
        switch (status)
        {
            case 0x00:
                console_out("Successfully Executed (0x00)\n");
                break;
            case 0x01:
                console_out("General Error (0x01)\n");
                break;
            case 0x02:
                console_out("Unused RMAP Packet Type or Command Code (0x02)\n");
                break;
            case 0x03:
                console_out("Invalid Target Key (0x03)\n");
                break;
            case 0x04:
                console_out("Invalid Data CRC (0x04)\n");
                break;
            case 0x05:
                console_out("Early EOP (0x05)\n");
                break;
            case 0x06:
                console_out("Cargo Too Large (0x06)\n");
                break;
            case 0x07:
                console_out("EEP (0x07)\n");
                break;
            case 0x08:
                console_out("Reserved (0x08)\n");
                break;
            case 0x09:
                console_out("Verify Buffer Overrun (0x09)\n");
                break;
            case 0x0a:
                console_out("RMAP Command Not Implemented or Not Authorized (0x0a)\n");
                break;
            case 0x0b:
                console_out("Invalid Target Logical Address (0x0b)\n");
                break;
            default:
                console_out("Reserved (0x%02X\n", status);
                break;
        }
    }
};
}
}

#endif /* MODULES_HAL_PROTOCOL_RMAP_RMAP_STATUS_H_ */
