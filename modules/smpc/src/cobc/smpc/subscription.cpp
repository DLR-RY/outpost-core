/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#include "subscription.h"

cobc::smpc::Subscription * cobc::smpc::Subscription::listOfAllSubscriptions = 0;

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

    for (Subscription * it = Subscription::listOfAllSubscriptions;
            it != 0;
            it = it->getNext())
    {
        it->nextTopicSubscription = it->topic->subscriptions;
        it->topic->subscriptions = it;
    }
}

void
cobc::smpc::Subscription::releaseAllSubscriptions()
{
    for (Subscription * it = Subscription::listOfAllSubscriptions;
            it != 0;
            it = it->getNext())
    {
        it->nextTopicSubscription = 0;
    }

    TopicBase::clearSubscriptions();
}
