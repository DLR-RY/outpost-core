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

#ifndef OUTPOST_SMPC_TOPIC_RAW_H
#define OUTPOST_SMPC_TOPIC_RAW_H

#include <stddef.h>
#include <stdint.h>

#include <outpost/rtos/mutex.h>
#include <outpost/container/implicit_list.h>
#include <outpost/utils/meta.h>

namespace outpost
{
namespace smpc
{
// forward declaration
class SubscriptionRaw;

/**
 * Non type-safe %Topic.
 *
 * Data can be published under a predefined topic. Any number of
 * subscribers can connect to that topic to get notified if new
 * data is published.
 *
 * There is also a type-safe version of this Publisher<>%Subscriber
 * protocol called outpost::smpc::Topic available. Use that version if
 * you want to exchange data with a known length. If in doubt use the
 * type-safe version!
 *
 * \see     outpost::smpc::Topic
 * \ingroup smpc
 * \author  Fabian Greif
 */
class TopicRaw : protected ImplicitList<TopicRaw>
{
public:
    // Needed to allow SubscriptionRaw() to append itself to the
    // subscription list
    friend class SubscriptionRaw;
    friend class ImplicitList<TopicRaw>;

    /**
     * Create a new raw topic.
     */
    TopicRaw();

    /**
     * Destroy the topic.
     *
     * \warning
     *         The destruction and creation of topics during the normal
     *         runtime is not thread-safe. If topics need to be
     *         destroyed outside the initialization of the application
     *         it is necessary to hold all other threads which
     *         might also create or destroy topics and/or subscriptions.
     */
    ~TopicRaw();

    /**
     * Publish new data.
     *
     * Forwards the pointer to all connected subscribers.
     */
    void
    publish(const void* message, size_t length);

private:
    // disable copy constructor
    TopicRaw(const TopicRaw&);

    // disable assignment operator
    TopicRaw&
    operator=(const TopicRaw&);

    static void
    clearSubscriptions();

    /// List of all raw topics currently active.
    static TopicRaw* listOfAllTopics;

    /// Mutex used to protect the publish() method.
    rtos::Mutex mMutex;

    /// Pointer to the list of mSubscriptions
    SubscriptionRaw* mSubscriptions;
};

}
}

#endif
