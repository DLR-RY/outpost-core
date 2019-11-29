/*
 * rmap_result.h
 *
 *  Created on: 29.11.2019
 *      Author: user
 */

#ifndef OUTPOST_COMM_RMAP_RMAP_RESULT_H_
#define OUTPOST_COMM_RMAP_RMAP_RESULT_H_

#include "rmap_status.h"

namespace outpost
{
namespace comm
{
class RmapInitiator;

enum class RmapResultType
{
    unknown,             // not yet set
    timeout,             // request was not finished in time
    invalidReply,        // remote replied with invalid package
    executionFailed,     // remote indicate execution failure, check status
    noFreeTransactions,  // all transactions are currently in use
    invalidParameters,   // functions parameters will never result in a successful request
    sendFailed,          // sending the command already failed
    replyTooShort,       // a read command read less data than requested, this may be valid behavior
                         // have to be handled by caller who knows more about the target
    success              // command executed as expected
};

class RmapResult
{
    friend class RmapInitiator;

public:
    /**
     * Returns the result of the command
     */
    RmapResultType
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
        return mResult == RmapResultType::success;
    }

private:
    RmapResultType mResult = RmapResultType::unknown;
    RmapReplyStatus::ErrorStatusCodes mErrorCode = RmapReplyStatus::ErrorStatusCodes::unknown;
    uint32_t mReadbytes = 0;
};

}  // namespace comm
}  // namespace outpost

#endif /* MODULES_COMM_SRC_OUTPOST_COMM_RMAP_RMAP_RESULT_H_ */
