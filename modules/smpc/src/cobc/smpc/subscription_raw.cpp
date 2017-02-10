/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#include "subscription_raw.h"

cobc::smpc::SubscriptionRaw* cobc::smpc::SubscriptionRaw::listOfAllSubscriptions = 0;

cobc::smpc::SubscriptionRaw::~SubscriptionRaw()
{
    removeFromList(&SubscriptionRaw::listOfAllSubscriptions, this);

    releaseAllSubscriptions();
    connectSubscriptionsToTopics();
}

void
cobc::smpc::SubscriptionRaw::connectSubscriptionsToTopics()
{
    TopicRaw::clearSubscriptions();

    for (SubscriptionRaw* it = listOfAllSubscriptions;
            it != 0;
            it = it->getNext())
    {
        it->mNextTopicSubscription = it->mTopic->mSubscriptions;
        it->mTopic->mSubscriptions = it;
    }
}

void
cobc::smpc::SubscriptionRaw::releaseAllSubscriptions()
{
    for (SubscriptionRaw* it = listOfAllSubscriptions;
            it != 0;
            it = it->getNext())
    {
        it->mNextTopicSubscription = 0;
    }

    TopicRaw::clearSubscriptions();
}
