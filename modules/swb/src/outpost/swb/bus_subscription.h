/*
 * Copyright (c) 2020-2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan-Gerd Mess (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_SWB_BUS_SUBSCRIPTION_H
#define OUTPOST_SWB_BUS_SUBSCRIPTION_H

#include "types.h"

#include <outpost/utils/container/list.h>

#include <stdint.h>

namespace outpost
{
namespace swb
{
/**
 * A BusSubscription can be used together with a SubscriptionFilter on a BusChannel to filter by
 * (masked) Message id.
 * \ingroup swb
 * \param IDType type of the message ID
 */
template <typename IDType>
class BusSubscription : public outpost::ListElement
{
public:
    /**
     * Constructor.
     * \param messageId Incoming Message are filtered based on this ID.
     * \param mask A bitmask can be set to ignore certain bits of the ID during matching.
     *             Example: A mask of all zeros lets all Message pass, whereas a mask of all ones
     *             (default) means exact matching of the given messageId.
     */
    explicit BusSubscription(IDType messageId, IDType mask = ~(IDType())) :
        mMessageId(messageId & mask), mMask(mask), mNumMatchedMessages(0U)
    {
    }

    /**
     * Destructor.
     */
    virtual ~BusSubscription() = default;

    /**
     * Matching function for a given ID
     * \return Returns true if the given id matches the subscription, false otherwise.
     */
    inline bool
    matches(IDType id) const
    {
        bool res = false;
        if ((id & mMask) == mMessageId)
        {
            res = true;
            mNumMatchedMessages++;
        }
        return res;
    }

    /**
     * Getter for the number of matched Message
     * \return Number of matched Message.
     */
    inline uint32_t
    getNumberOfMatchedMessages() const
    {
        return mNumMatchedMessages;
    }

private:
    IDType mMessageId;
    IDType mMask;

    mutable uint32_t mNumMatchedMessages;
};

}  // namespace swb
}  // namespace outpost

#endif /* OUTPOST_SWB_BUS_SUBSCRIPTION_H */
