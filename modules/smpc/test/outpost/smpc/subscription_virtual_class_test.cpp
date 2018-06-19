/*
 * Copyright (c) 2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2018, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/smpc/subscription.h>
#include <outpost/smpc/topic.h>

#include <unittest/harness.h>
#include <unittest/smpc/testing_subscription.h>

#include <stdint.h>

#include <cstring>

class ComponentBase : public outpost::smpc::Subscriber
{
public:
    virtual ~ComponentBase() = default;

    virtual void
    onReceiveData(const bool*)
    {
        mReceivedBase = true;
    }

    bool mReceivedBase = false;
};

class ComponentChild : public ComponentBase
{
public:
    virtual void
    onReceiveData(const bool*) override
    {
        mReceivedChild = true;
    }

    bool mReceivedChild = false;
};

class SubscriptionVirtualClassTest : public ::testing::Test
{
public:
    virtual void
    SetUp()
    {
        componentBase.mReceivedBase = false;
        componentChild.mReceivedBase = false;
        componentChild.mReceivedChild = false;
    }

    virtual void
    TearDown()
    {
        unittest::smpc::TestingSubscription::releaseAllSubscriptions();
    }

    ComponentBase componentBase;
    ComponentChild componentChild;

    outpost::smpc::Topic<const bool> topic;
};

TEST_F(SubscriptionVirtualClassTest, baseClassShouldReceiveData)
{
    outpost::smpc::Subscription subscription(topic, &componentBase, &ComponentBase::onReceiveData);
    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    topic.publish(false);
    EXPECT_TRUE(componentBase.mReceivedBase);

    unittest::smpc::TestingSubscription::releaseAllSubscriptions();
}

TEST_F(SubscriptionVirtualClassTest, childClassShouldReceiveData)
{
    outpost::smpc::Subscription subscription(topic, &componentChild, &ComponentChild::onReceiveData);
    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    topic.publish(false);

    EXPECT_FALSE(componentChild.mReceivedBase);
    EXPECT_TRUE(componentChild.mReceivedChild);

    unittest::smpc::TestingSubscription::releaseAllSubscriptions();
}

TEST_F(SubscriptionVirtualClassTest, childClassShouldReceiveDataThroughBase)
{
    outpost::smpc::Subscription subscription(topic, &componentChild, &ComponentBase::onReceiveData);
    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    topic.publish(false);

    EXPECT_FALSE(componentChild.mReceivedBase);
    EXPECT_TRUE(componentChild.mReceivedChild);

    unittest::smpc::TestingSubscription::releaseAllSubscriptions();
}
