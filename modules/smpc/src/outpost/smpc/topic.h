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
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_SMPC_TOPIC_H
#define OUTPOST_SMPC_TOPIC_H

#include <outpost/rtos/mutex.h>
#include <outpost/utils/container/implicit_list.h>
#include <outpost/utils/meta.h>

#include <stdint.h>

namespace outpost
{
namespace smpc
{
// forward declaration
class Subscription;

/**
 * Non-template base class for %Topic<>.
 *
 * \warning
 *      This class should only be used through outpost::smpc::Topic
 *      and never alone!
 *
 * \see     Topic
 * \author  Fabian Greif
 */
class TopicBase : protected ImplicitList<TopicBase>
{
public:
    // Needed to allow Subscription() to append itself to the
    // subscription list
    friend class Subscription;
    friend class ImplicitList<TopicBase>;
    friend class TestingTopicBase;

    /**
     * Constructor.
     */
    TopicBase();

    /**
     * Destroy the topic.
     *
     * \warning
     *         The destruction and creation of topic during the normal
     *         runtime is not thread-safe. If topics need to be
     *         destroyed outside the initialization of the application
     *         it is necessary to hold all other threads which
     *         might also create or destroy topics and/or subscriptions.
     */
    ~TopicBase();

    /**
     * Publish new data.
     *
     * Forwards the pointer to all connected subscribers. This
     * function is thread safe.
     */
    void
    publishTypeUnsafe(void* message);

protected:
    /// List of all topics currently active.
    static TopicBase* listOfAllTopics;

private:
    // disable copy constructor
    TopicBase(const TopicBase&);

    // disable assignment operator
    TopicBase&
    operator=(const TopicBase&);

    static void
    clearSubscriptions();

    /// Used to protect the publish() method
    rtos::Mutex mMutex;

    /// Pointer to the list of subscriptions
    Subscription* mSubscriptions;
};

/**
 * Type-safe %Topic.
 *
 * Type-safe data exchange between different components. Data
 * can be published under a predefined topic. Any number of
 * subscribers can connect to that topic to get notified if new
 * data is published.
 *
 * There is also a not type-safe version of this %Topic<>%Subscriber
 * protocol called outpost::smpc::TopicRaw. Use this version if you need
 * to exchange data with a unknown and/or changing length.
 *
 * \ingroup smpc
 * \see     TopicRaw
 * \see     Subscription
 * \see     Subscriber
 * \author  Fabian Greif
 */
template <typename T>
class Topic : protected TopicBase
{
public:
    // Needed to allow up-casting in smpc::Subscription constructor
    friend class Subscription;

    /// Type of the data distributed by this topic.
    typedef T Type;
    typedef typename outpost::remove_const<T>::type NonConstType;

    /**
     * Constructor.
     */
    inline Topic() : TopicBase()
    {
    }

    /**
     * Destroy the topic.
     *
     * \warning
     *      The destruction and creation of topics during the normal
     *      runtime is not thread-safe. If topics need to be
     *      destroyed outside the initialization of the application
     *      it is necessary to hold all other threads which
     *      might also create or destroy topics and/or subscriptions.
     */
    inline ~Topic()
    {
    }

    // disable copy constructor
    Topic(const Topic&) = delete;

    // disable assignment operator
    Topic&
    operator=(const Topic&) = delete;

    /**
     * Publish new data.
     *
     * Forwards the pointer to all connected subscribers. This
     * function is thread safe.
     */
    inline void
    publish(T& message)
    {
        // This cast may remove any const qualifiers from the
        // type. This is no problem because they are enforced
        // when creating a subscription.
        // The qualifiers are only stripped for the internal
        // processing which is done invisible to the user.
        // See also outpost::com::Subscription::Subscription().
        NonConstType* ptr = const_cast<NonConstType*>(&message);
        TopicBase::publishTypeUnsafe(reinterpret_cast<void*>(ptr));
    }
};

}  // namespace smpc
}  // namespace outpost

#endif
