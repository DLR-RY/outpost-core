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

#include "subscription_raw.h"

outpost::smpc::SubscriptionRaw* outpost::smpc::SubscriptionRaw::listOfAllSubscriptions = 0;

outpost::smpc::SubscriptionRaw::~SubscriptionRaw()
{
    removeFromList(&SubscriptionRaw::listOfAllSubscriptions, this);

    releaseAllSubscriptions();
    connectSubscriptionsToTopics();
}

void
outpost::smpc::SubscriptionRaw::connectSubscriptionsToTopics()
{
    TopicRaw::clearSubscriptions();

    for (SubscriptionRaw* it = listOfAllSubscriptions; it != 0; it = it->getNext())
    {
        it->mNextTopicSubscription = it->mTopic->mSubscriptions;
        it->mTopic->mSubscriptions = it;
    }
}

void
outpost::smpc::SubscriptionRaw::releaseAllSubscriptions()
{
    for (SubscriptionRaw* it = listOfAllSubscriptions; it != 0; it = it->getNext())
    {
        it->mNextTopicSubscription = 0;
    }

    TopicRaw::clearSubscriptions();
}
