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

#include <stdio.h>

#include <outpost/smpc/topic.h>
#include <outpost/smpc/subscription.h>

namespace outpost
{
namespace smpc
{

class TestingTopicBase
{
public:
    void
    dumpConnectedSubscriptions(outpost::smpc::TopicBase& base);
};

using namespace outpost::smpc;

void
TestingTopicBase::dumpConnectedSubscriptions(outpost::smpc::TopicBase& base)
{
    printf("topic %p\n", reinterpret_cast<void *>(this));

    for (Subscription* topic = base.mSubscriptions;
            topic != 0;
            topic = topic->mNextTopicSubscription)
    {
        printf("- %p\n", reinterpret_cast<void *>(topic));
    }
}

}
}
