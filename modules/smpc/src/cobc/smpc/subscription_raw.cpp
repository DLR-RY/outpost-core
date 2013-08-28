/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "subscription_raw.h"

cobc::smpc::SubscriptionRaw * cobc::smpc::SubscriptionRaw::listOfAllSubscriptions = 0;

cobc::smpc::SubscriptionRaw::~SubscriptionRaw()
{
	removeFromList(&SubscriptionRaw::listOfAllSubscriptions, this);

	releaseAllSubscriptions();
	connectSubscriptionsToTopics();
}

void
cobc::smpc::SubscriptionRaw::connectSubscriptionsToTopics()
{
	TopicRaw::clearSubscriptions();

	for (SubscriptionRaw * it = listOfAllSubscriptions;
			it != 0;
			it = it->getNext())
	{
		it->nextTopicSubscription = it->topic->subscriptions;
		it->topic->subscriptions = it;
	}
}

void
cobc::smpc::SubscriptionRaw::releaseAllSubscriptions()
{
	for (SubscriptionRaw * it = listOfAllSubscriptions;
			it != 0;
			it = it->getNext())
	{
		it->nextTopicSubscription = 0;
	}

	TopicRaw::clearSubscriptions();
}
