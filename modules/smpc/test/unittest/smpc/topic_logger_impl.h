/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#ifndef COBC_SMPC_TOPIC_LOGGER_IMPL_H
#define COBC_SMPC_TOPIC_LOGGER_IMPL_H

#include "topic_logger.h"

template<typename T>
unittest::smpc::TopicLogger<T>::TopicLogger(outpost::smpc::Topic<T>& topic) :
    mSubscription(topic, this, &TopicLogger::onReceive)
{
}

template<typename T>
void
unittest::smpc::TopicLogger<T>::clear()
{
    mItems.clear();
}

template<typename T>
const T&
unittest::smpc::TopicLogger<T>::getNext() const
{
    return mItems.front();
}

template<typename T>
void
unittest::smpc::TopicLogger<T>::dropNext()
{
    mItems.pop_front();
}

template<typename T>
bool
unittest::smpc::TopicLogger<T>::isEmpty() const
{
    return mItems.empty();
}

template<typename T>
void
unittest::smpc::TopicLogger<T>::onReceive(T* item)
{
    mItems.push_back(*item);
}

#endif
