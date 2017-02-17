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

#ifndef UNITTEST_SMPC_TOPIC_LOGGER_H
#define UNITTEST_SMPC_TOPIC_LOGGER_H

#include <outpost/smpc/subscription.h>
#include <outpost/smpc/topic.h>

#include <unittest/smpc/testing_subscription.h>

#include <list>

namespace unittest
{
namespace smpc
{

template <typename T>
class TopicLogger
{
public:
    TopicLogger(outpost::smpc::Topic<T>& topic);

    void
    clear();

    const T&
    getNext() const;

    void
    dropNext();

    bool
    isEmpty() const;

private:
    typedef typename outpost::remove_const<T>::type NonConstType;

    void
    onReceive(T* item);

    outpost::smpc::Subscription mSubscription;
    std::list<NonConstType> mItems;
};

}
}

#include "topic_logger_impl.h"

#endif
