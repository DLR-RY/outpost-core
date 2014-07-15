
#include <stdint.h>

#include <cobc/smpc/topic_raw.h>
#include <cobc/smpc/subscription_raw.h>

#include <unittest/smpc/testing_subscription_raw.h>

#include <cstring>
#include <unittest/harness.h>

using namespace cobc::smpc;

struct Data
{
    uint32_t foo;
    uint16_t bar;
};

static TopicRaw topic;

class Component : public Subscriber
{
public:
    Component() :
        received()
    {
        reset();
    }

    void
    onReceiveData0(const void *, size_t) {
        received[0] = true;
    }

    void
    onReceiveData1(const void *, size_t) {
        received[1] = true;
    }

    void
    onReceiveData2(const void *, size_t) {
        received[2] = true;
    }

    void
    onReceiveData3(const void *, size_t) {
        received[3] = true;
    }

    void
    reset()
    {
        for (int i = 0; i < 4; ++i) {
            received[i] = false;
        }
    }

    bool received[4];
};

class SubscriptionRawTest : public ::testing::Test
{
public:
    SubscriptionRawTest() :
        data({ 0x12345678, 0x9876 })
    {
    }

    virtual void SetUp()
    {
        component.reset();
    }

    virtual void TearDown()
    {
        unittest::smpc::TestingSubscriptionRaw::releaseAllSubscriptions();
    }

    Component component;
    Data data;
};

TEST_F(SubscriptionRawTest, receiveNone)
{
    topic.publish(&data, sizeof(data));
    for (int i = 0; i < 4; ++i) {
        EXPECT_FALSE(component.received[i]);
    }

    unittest::smpc::TestingSubscriptionRaw::connectSubscriptionsToTopics();

    topic.publish(&data, sizeof(data));
    for (int i = 0; i < 4; ++i) {
        EXPECT_FALSE(component.received[i]);
    }
}

TEST_F(SubscriptionRawTest, receiveTwo)
{
    SubscriptionRaw * subscription0 = new SubscriptionRaw(topic, &component, &Component::onReceiveData0);
    SubscriptionRaw * subscription1 = new SubscriptionRaw(topic, &component, &Component::onReceiveData1);

    unittest::smpc::TestingSubscriptionRaw::connectSubscriptionsToTopics();

    topic.publish(&data, sizeof(data));
    EXPECT_TRUE(component.received[0]);
    EXPECT_TRUE(component.received[1]);
    EXPECT_FALSE(component.received[2]);
    EXPECT_FALSE(component.received[3]);

    delete subscription0;
    delete subscription1;
}

TEST_F(SubscriptionRawTest, receiveFour)
{
    SubscriptionRaw * subscription0 = new SubscriptionRaw(topic, &component, &Component::onReceiveData0);
    SubscriptionRaw * subscription1 = new SubscriptionRaw(topic, &component, &Component::onReceiveData1);
    SubscriptionRaw * subscription2 = new SubscriptionRaw(topic, &component, &Component::onReceiveData2);
    SubscriptionRaw * subscription3 = new SubscriptionRaw(topic, &component, &Component::onReceiveData3);

    unittest::smpc::TestingSubscriptionRaw::connectSubscriptionsToTopics();

    topic.publish(&data, sizeof(data));
    EXPECT_TRUE(component.received[0]);
    EXPECT_TRUE(component.received[1]);
    EXPECT_TRUE(component.received[2]);
    EXPECT_TRUE(component.received[3]);

    delete subscription0;
    delete subscription1;
    delete subscription2;
    delete subscription3;
}

TEST_F(SubscriptionRawTest, receiveFourWithDelete)
{
    SubscriptionRaw * subscription0 = new SubscriptionRaw(topic, &component, &Component::onReceiveData0);
    SubscriptionRaw * subscription1 = new SubscriptionRaw(topic, &component, &Component::onReceiveData1);
    SubscriptionRaw * subscription2 = new SubscriptionRaw(topic, &component, &Component::onReceiveData2);
    SubscriptionRaw * subscription3 = new SubscriptionRaw(topic, &component, &Component::onReceiveData3);

    unittest::smpc::TestingSubscriptionRaw::connectSubscriptionsToTopics();

    delete subscription1;

    topic.publish(&data, sizeof(data));
    EXPECT_TRUE(component.received[0]);
    EXPECT_FALSE(component.received[1]);
    EXPECT_TRUE(component.received[2]);
    EXPECT_TRUE(component.received[3]);

    delete subscription0;
    delete subscription2;
    delete subscription3;
}

TEST_F(SubscriptionRawTest, receiveFourWithDelete2)
{
    SubscriptionRaw * subscription0 = new SubscriptionRaw(topic, &component, &Component::onReceiveData0);
    SubscriptionRaw * subscription1 = new SubscriptionRaw(topic, &component, &Component::onReceiveData1);
    SubscriptionRaw * subscription2 = new SubscriptionRaw(topic, &component, &Component::onReceiveData2);
    SubscriptionRaw * subscription3 = new SubscriptionRaw(topic, &component, &Component::onReceiveData3);

    unittest::smpc::TestingSubscriptionRaw::connectSubscriptionsToTopics();

    delete subscription0;
    delete subscription3;

    topic.publish(&data, sizeof(data));
    EXPECT_FALSE(component.received[0]);
    EXPECT_TRUE(component.received[1]);
    EXPECT_TRUE(component.received[2]);
    EXPECT_FALSE(component.received[3]);

    delete subscription1;
    delete subscription2;
}
