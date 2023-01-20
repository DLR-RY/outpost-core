/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 * - 2016, Jan Sommer (DLR SC-SRV)
 */

#ifndef OUTPOST_SMPC_SUBSCRIPTION_H
#define OUTPOST_SMPC_SUBSCRIPTION_H

#include "subscriber.h"
#include "topic.h"

#include <outpost/utils/functor.h>

namespace outpost
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
class Subscription : public ImplicitList<Subscription>
{
public:
    friend class TopicBase;
    friend class TestingTopicBase;  // for unit tests
    friend class SubscriptionRaw;
    friend class ImplicitList<Subscription>;

    template <typename T, typename S>
    struct SubscriberFunction
    {
        typedef void (S::*Type)(typename Topic<T>::Type* message);
    };

    /**
     * Constructor.
     *
     * Binds a subscriber to a topic.
     *
     * \param[in]    topic
     *         Topic to subscribe to
     * \param[in]    subscriber
     *         Subscribing class. Must be a subclass of outpost::smpc::Subscriber.
     * \param[in]    function
     *         Member function pointer of the subscribing class.
     */
    template <typename T, typename S>
    Subscription(Topic<T>& topic, S* subscriber, typename SubscriberFunction<T, S>::Type function);

    /**
     * Destroy the subscription
     *
     * \warning
     *     The destruction and creation of subscriptions during the normal
     *     runtime is not thread-safe. If topics need to be
     *     destroyed outside the initialization of the application
     *     it is necessary to hold all other threads which
     *     might also create or destroy topics and/or subscriptions.
     */
    ~Subscription();

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

    /**
     * Release all subscriptions.
     *
     * Counterpart to connectSubscriptionsToTopics().
     *
     * \warning
     *      Should only be called during tear down of a program. Use of this
     *      function is not thread-safe. To use halt all threads that
     *      might create or destroy subscriptions or topics.
     */
    static void
    releaseAllSubscriptions();

protected:
    /**
     * Relay message to the subscribing component.
     */
    inline void
    execute(void* message) const
    {
        outpost::utils::OperationResult res = outpost::utils::OperationResult::invalid;
        mFunctor.execute(res, message);
    }

private:
    // Disable default constructor
    Subscription();

    // Disable copy constructor
    Subscription(const Subscription&);

    // Disable copy assignment operator
    Subscription&
    operator=(const Subscription&);

    /// List of all subscriptions currently in the system
    static Subscription* listOfAllSubscriptions;

    /**
     * Used by Subscription::connectSubscriptionsToTopics to map the
     * subscriptions to their corresponding topics.
     */
    TopicBase* const mTopic;
    Subscription* mNextTopicSubscription;

    /**
     * Base-type to cast all member function pointers to. The correct type
     * is restored when calling the function. Although it the member
     * function is later called with a void-pointer everything remains
     * safe as only member functions with a correct signature can
     * be bound to a topic.
     */
    typedef void (Subscriber::*Function)(void*);

    const Functor<void(void*)> mFunctor;
};

}  // namespace smpc
}  // namespace outpost

// ----------------------------------------------------------------------------
// Implementation of the template constructor
template <typename T, typename S>
outpost::smpc::Subscription::Subscription(Topic<T>& topic,
                                          S* subscriber,
                                          typename SubscriberFunction<T, S>::Type function) :
    ImplicitList<Subscription>(listOfAllSubscriptions, this),
    mTopic(&topic),
    mNextTopicSubscription(0),
    mFunctor(*reinterpret_cast<Subscriber*>(subscriber), reinterpret_cast<Function>(function))
{
}

#endif
