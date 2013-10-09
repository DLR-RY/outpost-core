
#include <cobc/smpc/topic.h>

#include <unittest/harness.h>

/*
 * Check that it is possible to compile topic with selected types.
 */

TEST(TopicTypeTest, testInteger)
{
	cobc::smpc::Topic<const uint16_t> keepAlive;

	keepAlive.publish(123);

	uint16_t apid = 234;
	keepAlive.publish(apid);
}

TEST(TopicTypeTest, testArray)
{
	static const std::size_t numberOfSunSensors = 10;

	cobc::smpc::Topic<const uint16_t[numberOfSunSensors]> sunSensors;

	uint16_t values[10] = {};

	sunSensors.publish(values);
}

TEST(TopicTypeTest, testPowerStruct)
{
	struct PowerState
	{
		bool device1;
		bool device2;
	};

	cobc::smpc::Topic<const PowerState> powerState;

	PowerState p;
	p.device1 = true;
	p.device2 = false;

	powerState.publish(p);
}
