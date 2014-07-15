/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef UNITTEST_SMPC_TESTING_SUBSCRIPTION_RAW_H
#define UNITTEST_SMPC_TESTING_SUBSCRIPTION_RAW_H

#include <cobc/smpc/subscription_raw.h>

namespace unittest
{
namespace smpc
{

class TestingSubscriptionRaw : public cobc::smpc::SubscriptionRaw
{
public:
    using SubscriptionRaw::connectSubscriptionsToTopics;
    using SubscriptionRaw::releaseAllSubscriptions;
};

}
}

#endif // UNITTEST_SMPC_TESTING_SUBSCRIPTION_RAW_H
