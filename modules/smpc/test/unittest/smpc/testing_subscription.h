/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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
