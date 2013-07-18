
#include <stdint.h>

#include <cobc/smpc/topic.h>
#include <cobc/smpc/subscription.h>

#include <cstring>
#include <unittest/harness.h>

struct Data
{
	uint32_t foo;
	uint16_t bar;
};

static cobc::smpc::Topic<const Data> topic;

class Component : public cobc::smpc::Subscriber
{
public:
	Component() :
		received({ false, false, false, false })
	{
	}
	
	void
	onReceiveData0(const Data * data) {
		received[0] = true;
	}
	
	void
	onReceiveData1(const Data * data) {
		received[1] = true;
	}
	
	void
	onReceiveData2(const Data * data) {
		received[2] = true;
	}
	
	void
	onReceiveData3(const Data * data) {
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
		cobc::smpc::TestingSubscription::releaseAllSubscriptions();
	}
	
	Component component;
	Data data;
};

TEST_F(SubscriptionTest, receiveNone)
{
	topic.publish(data);
	for (int i = 0; i < 4; ++i) {
		EXPECT_FALSE(component.received[i]);
	}
	
	cobc::smpc::TestingSubscription::connectSubscriptionsToTopics();
	
	topic.publish(data);
	for (int i = 0; i < 4; ++i) {
		EXPECT_FALSE(component.received[i]);
	}
}

TEST_F(SubscriptionTest, receiveTwo)
{
	cobc::smpc::Subscription * subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
	cobc::smpc::Subscription * subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);
	
	cobc::smpc::TestingSubscription::connectSubscriptionsToTopics();
	
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
	cobc::smpc::Subscription * subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
	cobc::smpc::Subscription * subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);
	cobc::smpc::Subscription * subscription2 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData2);
	cobc::smpc::Subscription * subscription3 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData3);
		
	cobc::smpc::TestingSubscription::connectSubscriptionsToTopics();
	
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
	cobc::smpc::Subscription * subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
	cobc::smpc::Subscription * subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);
	cobc::smpc::Subscription * subscription2 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData2);
	cobc::smpc::Subscription * subscription3 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData3);
		
	cobc::smpc::TestingSubscription::connectSubscriptionsToTopics();

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
	cobc::smpc::Subscription * subscription0 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData0);
	cobc::smpc::Subscription * subscription1 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData1);
	cobc::smpc::Subscription * subscription2 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData2);
	cobc::smpc::Subscription * subscription3 = new cobc::smpc::Subscription(topic, &component, &Component::onReceiveData3);
		
	cobc::smpc::TestingSubscription::connectSubscriptionsToTopics();

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
