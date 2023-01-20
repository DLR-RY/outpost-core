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

#include <outpost/smpc/subscription.h>
#include <outpost/smpc/topic.h>

#include <stdio.h>

namespace outpost
{
namespace smpc
{
class TestingTopicBase
{
public:
    void
    dumpConnectedSubscriptions(const outpost::smpc::TopicBase& base) const;
};

using namespace outpost::smpc;

void
TestingTopicBase::dumpConnectedSubscriptions(const outpost::smpc::TopicBase& base) const
{
    printf("topic %p\n", reinterpret_cast<const void*>(this));

    for (Subscription* topic = base.mSubscriptions; topic != 0;
         topic = topic->mNextTopicSubscription)
    {
        printf("- %p\n", reinterpret_cast<void*>(topic));
    }
}

}  // namespace smpc
}  // namespace outpost
