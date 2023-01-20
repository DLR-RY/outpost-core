/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2018, Fabian Greif (DLR RY-AVS)
 */

#include "topic.h"

#include "subscription.h"

#include <outpost/rtos/mutex_guard.h>

outpost::smpc::TopicBase* outpost::smpc::TopicBase::listOfAllTopics = nullptr;

outpost::smpc::TopicBase::TopicBase() :
    ImplicitList<TopicBase>(listOfAllTopics, this), mSubscriptions(nullptr)
{
}

outpost::smpc::TopicBase::~TopicBase()
{
    removeFromList(&TopicBase::listOfAllTopics, this);
}

void
outpost::smpc::TopicBase::publishTypeUnsafe(void* message) const
{
    rtos::MutexGuard lock(mMutex);

    for (Subscription* subscription = mSubscriptions; subscription != nullptr;
         subscription = subscription->mNextTopicSubscription)
    {
        subscription->execute(message);
    }
}

void
outpost::smpc::TopicBase::clearSubscriptions()
{
    for (TopicBase* it = listOfAllTopics; it != nullptr; it = it->getNext())
    {
        it->mSubscriptions = nullptr;
    }
}
