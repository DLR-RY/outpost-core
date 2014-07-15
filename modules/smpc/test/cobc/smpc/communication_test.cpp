
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

static bool
operator==(Data const& lhs, Data const& rhs)
{
    return ((lhs.foo == rhs.foo) &&
            (lhs.bar == rhs.bar));
}

static cobc::smpc::Topic<const Data> topic;

class Component : public cobc::smpc::Subscriber
{
public:
    Component() :
        received(false),
        data({0, 0}),
        subscription(topic, this, &Component::onReceiveData)
    {
    }

    void
    onReceiveData(const Data * data)
    {
        received = true;
        this->data = *data;
    }

    bool received;
    Data data;

private:
    cobc::smpc::Subscription subscription;
};

TEST(CommunicationTest, receiveSingle)
{
    Component component;

    cobc::smpc::Subscription::connectSubscriptionsToTopics();

    Data data = {
        0x12345678,
        0x9876
    };

    topic.publish(data);

    ASSERT_TRUE(component.received);
    EXPECT_EQ(data, component.data);

    unittest::smpc::TestingSubscription::releaseAllSubscriptions();
}
