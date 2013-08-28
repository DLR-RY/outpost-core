/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "topic.h"
#include "subscription.h"

cobc::smpc::TopicBase * cobc::smpc::TopicBase::listOfAllTopics = 0;

cobc::smpc::TopicBase::TopicBase() :
	List<TopicBase>(listOfAllTopics, this),
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

// LCOV_EXCL_START
#ifdef UNITTEST
#include <cstdio>

void
cobc::smpc::TopicBase::dumpConnectedSubscriptions()
{
	printf("topic %p\n", reinterpret_cast<void *>(this));

	for (Subscription * topic = subscriptions;
			topic != 0;
			topic = topic->nextTopicSubscription)
	{
		printf("- %p\n", reinterpret_cast<void *>(topic));
	}
}
#endif
// LCOV_EXCL_STOP
// LCOV_EXCL_END
