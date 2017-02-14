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

    for (SubscriptionRaw* topic = mSubscriptions;
            topic != 0;
            topic = topic->mNextTopicSubscription)
    {
        topic->notify(message, length);
    }
}

void
outpost::smpc::TopicRaw::clearSubscriptions()
{
    for (TopicRaw* it = listOfAllTopics;
            it != 0;
            it = it->getNext())
    {
        it->mSubscriptions = 0;
    }
}
