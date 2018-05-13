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

#include "topic_raw.h"

#include "subscription_raw.h"

#include <outpost/rtos/mutex_guard.h>

outpost::smpc::TopicRaw* outpost::smpc::TopicRaw::listOfAllTopics = 0;

outpost::smpc::TopicRaw::TopicRaw() :
    ImplicitList<TopicRaw>(listOfAllTopics, this),
    mSubscriptions(0)
{
}

outpost::smpc::TopicRaw::~TopicRaw()
{
    removeFromList(&TopicRaw::listOfAllTopics, this);
}

void
outpost::smpc::TopicRaw::publish(const void* message, size_t length)
{
    rtos::MutexGuard lock(mMutex);

    for (SubscriptionRaw* subscription = mSubscriptions; subscription != 0;
         subscription = subscription->mNextTopicSubscription)
    {
        subscription->execute(message, length);
    }
}

void
outpost::smpc::TopicRaw::clearSubscriptions()
{
    for (TopicRaw* it = listOfAllTopics; it != 0; it = it->getNext())
    {
        it->mSubscriptions = 0;
    }
}
