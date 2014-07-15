/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef UNITTEST_SMPC_TESTING_SUBSCRIPTION_H
#define UNITTEST_SMPC_TESTING_SUBSCRIPTION_H

#include <cobc/smpc/subscription.h>

namespace unittest
{
namespace smpc
{

class TestingSubscription : public cobc::smpc::Subscription
{
public:
    using Subscription::connectSubscriptionsToTopics;
    using Subscription::releaseAllSubscriptions;
};

}
}

#endif // UNITTEST_SMPC_TESTING_SUBSCRIPTION_H
