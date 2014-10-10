/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
 *
 * It is distributed under the terms of the GNU General Public License with
 * a linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
// ----------------------------------------------------------------------------

#include "topic.h"
#include "subscription.h"

#include <cobc/rtos/mutex_guard.h>

cobc::smpc::TopicBase* cobc::smpc::TopicBase::listOfAllTopics = 0;

cobc::smpc::TopicBase::TopicBase() :
    ImplicitList<TopicBase>(listOfAllTopics, this),
    subscriptions(0)
{
}

cobc::smpc::TopicBase::~TopicBase()
{
    removeFromList(&TopicBase::listOfAllTopics, this);
}

void
cobc::smpc::TopicBase::publishTypeUnsafe(void* message)
{
    rtos::MutexGuard lock(mutex);

    for (Subscription* topic = subscriptions;
            topic != 0;
            topic = topic->nextTopicSubscription)
    {
        topic->notify(message);
    }
}

void
cobc::smpc::TopicBase::clearSubscriptions()
{
    for (TopicBase* it = listOfAllTopics;
            it != 0;
            it = it->getNext())
    {
        it->subscriptions = 0;
    }
}
