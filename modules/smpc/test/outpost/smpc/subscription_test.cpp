/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include <stdint.h>

#include <outpost/smpc/topic.h>
#include <outpost/smpc/subscription.h>

#include <unittest/smpc/testing_subscription.h>

#include <cstring>
#include <unittest/harness.h>

struct Data
{
    uint32_t foo;
    uint16_t bar;
};

class Component : public outpost::smpc::Subscriber
{
public:
    Component() :
        received()
    {
        reset();
    }

    void
    onReceiveData0(const Data*)
    {
        received[0] = true;
    }

    void
    onReceiveData1(const Data*)
    {
        received[1] = true;
    }

    void
    onReceiveData2(const Data*)
    {
        received[2] = true;
    }

    void
    onReceiveData3(const Data*)
    {
        received[3] = true;
    }

    void
    reset()
    {
        for (int i = 0; i < 4; ++i)
        {
            received[i] = false;
        }
    }

    bool received[4];
};

class SubscriptionTest : public ::testing::Test
{
public:
    SubscriptionTest() :
        data({ 0x12345678, 0x9876 })
    {
    }

    virtual void SetUp()
    {
        component.reset();
    }

    virtual void TearDown()
    {
        unittest::smpc::TestingSubscription::releaseAllSubscriptions();
    }

    Component component;
    Data data;
    outpost::smpc::Topic<const Data> topic;
};

TEST_F(SubscriptionTest, receiveNone)
{
    topic.publish(data);
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_FALSE(component.received[i]);
    }

    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    topic.publish(data);
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_FALSE(component.received[i]);
    }
}

TEST_F(SubscriptionTest, receiveTwo)
{
    outpost::smpc::Subscription* subscription0 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    outpost::smpc::Subscription* subscription1 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData1);

    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    topic.publish(data);
    EXPECT_TRUE(component.received[0]);
    EXPECT_TRUE(component.received[1]);
    EXPECT_FALSE(component.received[2]);
    EXPECT_FALSE(component.received[3]);

    delete subscription0;
    delete subscription1;
}

TEST_F(SubscriptionTest, receiveFour)
{
    outpost::smpc::Subscription* subscription0 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    outpost::smpc::Subscription* subscription1 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData1);
    outpost::smpc::Subscription* subscription2 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData2);
    outpost::smpc::Subscription* subscription3 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData3);

    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    topic.publish(data);
    EXPECT_TRUE(component.received[0]);
    EXPECT_TRUE(component.received[1]);
    EXPECT_TRUE(component.received[2]);
    EXPECT_TRUE(component.received[3]);

    delete subscription0;
    delete subscription1;
    delete subscription2;
    delete subscription3;
}

TEST_F(SubscriptionTest, receiveFourWithDelete)
{
    outpost::smpc::Subscription* subscription0 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    outpost::smpc::Subscription* subscription1 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData1);
    outpost::smpc::Subscription* subscription2 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData2);
    outpost::smpc::Subscription* subscription3 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData3);

    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    delete subscription1;

    topic.publish(data);
    EXPECT_TRUE(component.received[0]);
    EXPECT_FALSE(component.received[1]);
    EXPECT_TRUE(component.received[2]);
    EXPECT_TRUE(component.received[3]);

    delete subscription0;
    delete subscription2;
    delete subscription3;
}

TEST_F(SubscriptionTest, receiveFourWithDelete2)
{
    outpost::smpc::Subscription* subscription0 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    outpost::smpc::Subscription* subscription1 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData1);
    outpost::smpc::Subscription* subscription2 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData2);
    outpost::smpc::Subscription* subscription3 = new outpost::smpc::Subscription(topic, &component, &Component::onReceiveData3);

    unittest::smpc::TestingSubscription::connectSubscriptionsToTopics();

    delete subscription0;
    delete subscription3;

    topic.publish(data);
    EXPECT_FALSE(component.received[0]);
    EXPECT_TRUE(component.received[1]);
    EXPECT_TRUE(component.received[2]);
    EXPECT_FALSE(component.received[3]);

    delete subscription1;
    delete subscription2;
}
