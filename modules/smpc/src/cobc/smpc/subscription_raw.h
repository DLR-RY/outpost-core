/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#ifndef COBC_SMPC_SUBSCRIPTION_RAW_H
#define COBC_SMPC_SUBSCRIPTION_RAW_H

#include <stddef.h>

#include "subscriber.h"
#include "topic_raw.h"

namespace cobc
{
namespace smpc
{
/**
 * Subscription to a topic.
 *
 * Every component that wants to receive data from a topic needs to
 * Instantiate one or more objects of this class. Each instance binds
 * a topic to a member function of the subscribing class. Data send to
 * the topic will be forwarded to the given member function.
 *
 * To avoid the overhead of generating a copy of this class for every
 * type of topic and subscriber this class works internally with void
 * pointers.
 *
 * \ingroup smpc
 * \author  Fabian Greif
 */
class SubscriptionRaw : public ImplicitList<SubscriptionRaw>
{
public:
    friend class TopicRaw;

    template <typename S>
    struct SubscriberFunction
    {
        typedef void (S::*Type)(const void* message, size_t length);
    };

    /**
     * Constructor.
     *
     * Binds a subscriber to a topic.
     *
     * \param[in]    topic
     *         Raw Topic to subscribe to
     * \param[in]    subscriber
     *         Subscribing class. Must be a subclass of cobc::com::Subscriber.
     * \param[in]    function
     *         Member function pointer of the subscribing class.
     */
    template <typename S>
    SubscriptionRaw(TopicRaw& topic,
                    S* subscriber,
                    typename SubscriberFunction<S>::Type function);

    /**
     * Destroy the subscription
     *
     * \warning    The destruction and creation of subscriptions during the normal
     *             runtime is not thread-safe. If topics need to be
     *             destroyed outside the initialization of the application
     *             it is necessary to hold all other threads which
     *             might also create or destroy topics and/or subscriptions.
     */
    ~SubscriptionRaw();

    /**
     * Connect all subscriptions to it's assigned topic.
     *
     * Has to be called at program startup to initialize the
     * Publisher<>Subscriber protocol.
     *
     * \internal
     * Builds the internal linked lists.
     */
    static void
    connectSubscriptionsToTopics();

protected:
    /** Base-type to cast all member function pointers to. */
    typedef void (Subscriber::*Function)(const void*, size_t);

    /**
     * Relay message to the subscribing component.
     */
    inline void
    notify(const void* message, size_t length) const
    {
        (mSubscriber->*mFunction)(message, length);
    }

    /**
     * Release all subscriptions.
     *
     * Counterpart to connectSubscriptionsToTopics(). Use of this
     * function is not thread-safe. To use halt all threads that
     * might create or destroy subscriptions or topics.
     */
    static void
    releaseAllSubscriptions();

private:
    // Disable default constructor
    SubscriptionRaw();

    // Disable copy constructor
    SubscriptionRaw(const SubscriptionRaw&);

    // Disable copy assignment operator
    SubscriptionRaw&
    operator=(const SubscriptionRaw&);

    // List of all subscriptions currently in the system
    static SubscriptionRaw* listOfAllSubscriptions;

    // Used by Subscription::connect to map the subscriptions to
    // their corresponding topics.
    TopicRaw* const mTopic;
    SubscriptionRaw* mNextTopicSubscription;

    // Object and member function to forward a received message to
    Subscriber* const mSubscriber;
    Function const mFunction;
};

// ----------------------------------------------------------------------------
// Implementation of the template constructor
template <typename S>
SubscriptionRaw::SubscriptionRaw(TopicRaw& topic,
                                 S* subscriber,
                                 typename SubscriberFunction<S>::Type function) :
    ImplicitList<SubscriptionRaw>(listOfAllSubscriptions, this),
    mTopic(&topic),
    mNextTopicSubscription(0),
    mSubscriber(reinterpret_cast<Subscriber *>(subscriber)),
    mFunction(reinterpret_cast<Function>(function))
{
}

}
}

#endif // COBC_SMPC_SUBSCRIPTION_RAW_H
