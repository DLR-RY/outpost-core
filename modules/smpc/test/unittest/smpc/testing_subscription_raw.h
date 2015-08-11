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
