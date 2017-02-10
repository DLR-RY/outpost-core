/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
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

#ifndef UNITTEST_SMPC_TOPIC_LOGGER_H
#define UNITTEST_SMPC_TOPIC_LOGGER_H

#include <cobc/smpc/subscription.h>
#include <cobc/smpc/topic.h>

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
    TopicLogger(cobc::smpc::Topic<T>& topic);

    void
    clear();

    const T&
    getNext() const;

    void
    dropNext();

    bool
    isEmpty() const;

private:
    typedef typename cobc::remove_const<T>::type NonConstType;

    void
    onReceive(T* item);

    cobc::smpc::Subscription mSubscription;
    std::list<NonConstType> mItems;
};

}
}

#include "topic_logger_impl.h"

#endif
