
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

    for (Subscription* topic = base.subscriptions;
            topic != 0;
            topic = topic->nextTopicSubscription)
    {
        printf("- %p\n", reinterpret_cast<void *>(topic));
    }
}

}
}
