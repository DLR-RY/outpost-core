/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#include "subscription.h"

cobc::smpc::Subscription* cobc::smpc::Subscription::listOfAllSubscriptions = 0;

cobc::smpc::Subscription::~Subscription()
{
    removeFromList(&Subscription::listOfAllSubscriptions, this);

    // TODO
    releaseAllSubscriptions();
    connectSubscriptionsToTopics();
}

void
cobc::smpc::Subscription::connectSubscriptionsToTopics()
{
    // Reset the lists in the topics
    TopicBase::clearSubscriptions();

    for (Subscription* it = Subscription::listOfAllSubscriptions;
            it != 0;
            it = it->getNext())
    {
        it->mNextTopicSubscription = it->mTopic->mSubscriptions;
        it->mTopic->mSubscriptions = it;
    }
}

void
cobc::smpc::Subscription::releaseAllSubscriptions()
{
    for (Subscription* it = Subscription::listOfAllSubscriptions;
            it != 0;
            it = it->getNext())
    {
        it->mNextTopicSubscription = 0;
    }

    TopicBase::clearSubscriptions();
}
