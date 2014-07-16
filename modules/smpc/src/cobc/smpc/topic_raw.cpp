/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "topic_raw.h"
#include "subscription_raw.h"

#include <cobc/rtos/mutex_guard.h>

cobc::smpc::TopicRaw* cobc::smpc::TopicRaw::listOfAllTopics = 0;

cobc::smpc::TopicRaw::TopicRaw() :
    ImplicitList<TopicRaw>(listOfAllTopics, this),
    subscriptions(0)
{
}

cobc::smpc::TopicRaw::~TopicRaw()
{
    removeFromList(&TopicRaw::listOfAllTopics, this);
}

void
cobc::smpc::TopicRaw::publish(const void* message, size_t length)
{
    rtos::MutexGuard lock(mutex);

    for (SubscriptionRaw* topic = subscriptions;
            topic != 0;
            topic = topic->nextTopicSubscription)
    {
        topic->notify(message, length);
    }
}

void
cobc::smpc::TopicRaw::clearSubscriptions()
{
    for (TopicRaw* it = listOfAllTopics;
            it != 0;
            it = it->getNext())
    {
        it->subscriptions = 0;
    }
}
