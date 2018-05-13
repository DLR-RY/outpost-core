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
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#include "subscription.h"

outpost::smpc::Subscription* outpost::smpc::Subscription::listOfAllSubscriptions = 0;

outpost::smpc::Subscription::~Subscription()
{
    removeFromList(&Subscription::listOfAllSubscriptions, this);

    // TODO
    releaseAllSubscriptions();
    connectSubscriptionsToTopics();
}

void
outpost::smpc::Subscription::connectSubscriptionsToTopics()
{
    // Reset the lists in the topics
    TopicBase::clearSubscriptions();

    for (Subscription* it = Subscription::listOfAllSubscriptions; it != 0; it = it->getNext())
    {
        it->mNextTopicSubscription = it->mTopic->mSubscriptions;
        it->mTopic->mSubscriptions = it;
    }
}

void
outpost::smpc::Subscription::releaseAllSubscriptions()
{
    for (Subscription* it = Subscription::listOfAllSubscriptions; it != 0; it = it->getNext())
    {
        it->mNextTopicSubscription = 0;
    }

    TopicBase::clearSubscriptions();
}
