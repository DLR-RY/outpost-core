/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * 
 * This file is part of libCOBC 0.4.
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

#include <stdint.h>

#include <cobc/smpc/topic.h>
#include <cobc/smpc/subscription.h>

#include <unittest/smpc/testing_subscription.h>

#include <cstring>
#include <unittest/harness.h>

struct Data
{
    uint32_t foo;
    uint16_t bar;
};

class Component : public cobc::smpc::Subscriber
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
    cobc::smpc::Topic<const Data> topic;
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
    cobc::smpc::Subscription* subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    cobc::smpc::Subscription* subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);

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
    cobc::smpc::Subscription* subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    cobc::smpc::Subscription* subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);
    cobc::smpc::Subscription* subscription2 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData2);
    cobc::smpc::Subscription* subscription3 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData3);

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
    cobc::smpc::Subscription* subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    cobc::smpc::Subscription* subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);
    cobc::smpc::Subscription* subscription2 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData2);
    cobc::smpc::Subscription* subscription3 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData3);

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
    cobc::smpc::Subscription* subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
    cobc::smpc::Subscription* subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);
    cobc::smpc::Subscription* subscription2 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData2);
    cobc::smpc::Subscription* subscription3 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData3);

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
