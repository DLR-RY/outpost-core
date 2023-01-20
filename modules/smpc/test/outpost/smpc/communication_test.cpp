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

#include <outpost/smpc/subscription.h>
#include <outpost/smpc/topic.h>

#include <unittest/harness.h>
#include <unittest/smpc/testing_subscription.h>

#include <stdint.h>
#include <string.h>

struct Data
{
    uint32_t foo;
    uint16_t bar;
};

static bool
operator==(Data const& lhs, Data const& rhs)
{
    return ((lhs.foo == rhs.foo) && (lhs.bar == rhs.bar));
}

class Component : public outpost::smpc::Subscriber
{
public:
    explicit Component(outpost::smpc::Topic<const Data>& topic) :
        mReceived(false), mData({0, 0}), subscription(topic, this, &Component::onReceiveData)
    {
    }

    void
    onReceiveData(const Data* data)
    {
        mReceived = true;
        mData = *data;
    }

    bool mReceived;
    Data mData;

private:
    outpost::smpc::Subscription subscription;
};

TEST(CommunicationTest, receiveSingle)
{
    outpost::smpc::Topic<const Data> topic;
    Component component(topic);

    outpost::smpc::Subscription::connectSubscriptionsToTopics();

    Data data = {0x12345678, 0x9876};

    topic.publish(data);

    ASSERT_TRUE(component.mReceived);
    EXPECT_EQ(data, component.mData);

    unittest::smpc::TestingSubscription::releaseAllSubscriptions();
}
