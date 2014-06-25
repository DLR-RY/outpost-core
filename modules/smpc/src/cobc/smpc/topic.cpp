/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "topic.h"
#include "subscription.h"

#include <cobc/rtos/mutex_guard.h>

cobc::smpc::TopicBase * cobc::smpc::TopicBase::listOfAllTopics = 0;

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
cobc::smpc::TopicBase::publish(void * message)
{
    rtos::MutexGuard lock(mutex);

    for (Subscription * topic = subscriptions;
            topic != 0;
            topic = topic->nextTopicSubscription)
    {
        topic->notify(message);
    }
}

void
cobc::smpc::TopicBase::clearSubscriptions()
{
    for (TopicBase * it = listOfAllTopics;
            it != 0;
            it = it->getNext())
    {
        it->subscriptions = 0;
    }
}
