/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#include "topic.h"
#include "subscription.h"

#include <outpost/rtos/mutex_guard.h>

outpost::smpc::TopicBase* outpost::smpc::TopicBase::listOfAllTopics = 0;

outpost::smpc::TopicBase::TopicBase() :
    ImplicitList<TopicBase>(listOfAllTopics, this),
    mSubscriptions(0)
{
}

outpost::smpc::TopicBase::~TopicBase()
{
    removeFromList(&TopicBase::listOfAllTopics, this);
}

void
outpost::smpc::TopicBase::publishTypeUnsafe(void* message)
{
    rtos::MutexGuard lock(mMutex);

    for (Subscription* subscription = mSubscriptions;
            subscription != 0;
            subscription = subscription->mNextTopicSubscription)
    {
        subscription->notify(message);
    }
}

void
outpost::smpc::TopicBase::clearSubscriptions()
{
    for (TopicBase* it = listOfAllTopics;
            it != 0;
            it = it->getNext())
    {
        it->mSubscriptions = 0;
    }
}
