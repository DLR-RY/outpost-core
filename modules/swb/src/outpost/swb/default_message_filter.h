/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_SWB_DEFAULT_MESSAGE_FILTER_H_
#define OUTPOST_SWB_DEFAULT_MESSAGE_FILTER_H_

#include "bus_subscription.h"

#include <outpost/base/slice.h>
#include <outpost/swb/types.h>
#include <outpost/utils/container/list.h>

#include <stdint.h>

namespace outpost
{
namespace swb
{
/**
 * Base class for all filters. Demands a filter function to be implemented by its subclasses.
 * \ingroup swb
 * \param IDType type of the message ID
 */
template <typename IDType>
class MessageFilter
{
public:
    /**
     * Constructor.
     */
    MessageFilter() = default;

    /**
     * Destructor.
     */
    virtual ~MessageFilter() = default;

    /**
     * Operator to match a given Message against the filter.
     * \param m Message to be matched.
     * \return Returns true if the message is accepted, false otherwise.
     */
    bool
    operator()(const Message<IDType>& m) const
    {
        return filter(m.id, m.buffer.asSlice());
    }

    /**
     * Operator to match a given id and message data against the filter.
     * \param id Message id to be matched.
     * \param data Message data to be matched.
     * \return Returns true if the message is accepted, false otherwise.
     */
    bool
    operator()(const IDType& id, const outpost::Slice<const uint8_t>& data) const
    {
        return filter(id, data);
    }

protected:
    /**
     * Filters a message by id and data. Must be implemented by instantiable subclasses.
     * \param id Id of the Message to be checked.
     * \param data Slice containing the data of the Message to be checked.
     * \return Returns true if the message is accepted, false otherwise.
     */
    virtual bool
    filter(const IDType& id, const outpost::Slice<const uint8_t>& data) const = 0;
};
/**
 * Filters out nothing
 * \ingroup swb
 */
template <typename IDType>
class FilterNone : public MessageFilter<IDType>
{
public:
    FilterNone() = default;
    virtual ~FilterNone() = default;

protected:
    bool
    filter(const IDType&, const outpost::Slice<const uint8_t>&) const override
    {
        return true;
    }
};

/**
 * Uses Subscriptions for Filtering, requires integer IDType
 * \ingroup swb
 */
template <typename IDType>
class SubscriptionFilter : public MessageFilter<IDType>
{
public:
    SubscriptionFilter() = default;
    virtual ~SubscriptionFilter() = default;

    void
    registerSubscription(BusSubscription<IDType>& subscription)
    {
        mSubscriptions.append(&subscription);
        mSubscriptionCount++;
    }

    size_t
    getNumberOfSubscriptions() const
    {
        return mSubscriptionCount;
    }

protected:
    bool
    filter(const IDType& id, const outpost::Slice<const uint8_t>&) const override
    {
        for (auto sub = mSubscriptions.begin(); sub != mSubscriptions.end(); ++sub)
        {
            if (sub->matches(id))
            {
                return true;
            }
        }
        return false;
    }

private:
    outpost::List<BusSubscription<IDType>> mSubscriptions;
    size_t mSubscriptionCount = 0;
};

/**
 * Only allows IDs within range [min, max], requires IDType to have boolean <= and >= operators.
 * \ingroup swb
 */
template <typename IDType>
class RangeFilter : public MessageFilter<IDType>
{
public:
    RangeFilter() = default;
    virtual ~RangeFilter() = default;

    /**
     * Sets the range of Message IDs to be accepted.
     * \param min Minimum value to be accepted.
     * \param max Maximum value to be accepted.
     */
    void
    setRange(IDType min, IDType max)
    {
        mMin = min;
        mMax = max;
    }

protected:
    bool
    filter(const IDType& id, const outpost::Slice<const uint8_t>&) const override
    {
        return id >= mMin && id <= mMax;
    }

private:
    IDType mMin = IDType();
    IDType mMax = IDType();
};

}  // namespace swb
}  // namespace outpost

#endif
