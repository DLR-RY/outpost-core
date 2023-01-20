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

#ifndef OUTPOST_COMM_RMAP_RMAP_RESULT_H_
#define OUTPOST_COMM_RMAP_RMAP_RESULT_H_

#include "rmap_status.h"

namespace outpost
{
namespace comm
{
class RmapInitiator;

class RmapResult
{
    friend class RmapInitiator;

public:
    enum class Code
    {
        unknown,             // not yet set
        timeout,             // request was not finished in time
        invalidReply,        // remote replied with invalid package
        executionFailed,     // remote indicate execution failure, check status
        noFreeTransactions,  // all transactions are currently in use
        invalidParameters,   // functions parameters will never result in a successful request
        sendFailed,          // sending the command already failed
        replyTooShort,  // a read command read less data than requested, this may be valid behavior
                        // have to be handled by caller who knows more about the target
        lockingError,
        success  // command executed as expected
    };

    /**
     * Returns the result of the command
     */
    Code
    getResult() const
    {
        return mResult;
    }

    /**
     * return the number of bytes read, only valid if result is "success" or "replyTooShort"
     */
    uint32_t
    getReadBytes() const
    {
        return mReadbytes;
    }

    /**
     * Returns the errorcode send with the reply only valid if result is "executionFailed",
     * "success" or "replyTooShort" Also not valid if a write was requested with no reply mode
     */
    RmapReplyStatus::ErrorStatusCodes
    getReplyErrorCode() const
    {
        return mErrorCode;
    }

    /**
     * Intentional automatic cast to bool, will be true in all cases the old functions returned true
     * and false otherwise.
     */
    operator bool() const
    {
        return mResult == Code::success;
    }

private:
    Code mResult = Code::unknown;
    RmapReplyStatus::ErrorStatusCodes mErrorCode = RmapReplyStatus::ErrorStatusCodes::unknown;
    uint32_t mReadbytes = 0;
};

}  // namespace comm
}  // namespace outpost

#endif /* MODULES_COMM_SRC_OUTPOST_COMM_RMAP_RMAP_RESULT_H_ */
