/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef UNITTEST_SMPC_TESTING_SUBSCRIPTION_H
#define UNITTEST_SMPC_TESTING_SUBSCRIPTION_H

#include <outpost/smpc/subscription.h>

namespace unittest
{
namespace smpc
{
class TestingSubscription : public outpost::smpc::Subscription
{
public:
    using Subscription::connectSubscriptionsToTopics;
    using Subscription::releaseAllSubscriptions;
};

}  // namespace smpc
}  // namespace unittest

#endif  // UNITTEST_SMPC_TESTING_SUBSCRIPTION_H
