/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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

#include <stdio.h>

#include <cobc/smpc/topic.h>
#include <cobc/smpc/subscription.h>

namespace cobc
{
namespace smpc
{

class TestingTopicBase
{
public:
    void
    dumpConnectedSubscriptions(cobc::smpc::TopicBase& base);
};

using namespace cobc::smpc;

void
TestingTopicBase::dumpConnectedSubscriptions(cobc::smpc::TopicBase& base)
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
