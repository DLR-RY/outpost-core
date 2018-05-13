/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */

#ifndef OUTPOST_SMPC_TOPIC_LOGGER_IMPL_H
#define OUTPOST_SMPC_TOPIC_LOGGER_IMPL_H

#include "topic_logger.h"

template <typename T>
unittest::smpc::TopicLogger<T>::TopicLogger(outpost::smpc::Topic<T>& topic) :
    mSubscription(topic, this, &TopicLogger::onReceive)
{
}

template <typename T>
void
unittest::smpc::TopicLogger<T>::clear()
{
    mItems.clear();
}

template <typename T>
const T&
unittest::smpc::TopicLogger<T>::getNext() const
{
    return mItems.front();
}

template <typename T>
void
unittest::smpc::TopicLogger<T>::dropNext()
{
    mItems.pop_front();
}

template <typename T>
bool
unittest::smpc::TopicLogger<T>::isEmpty() const
{
    return mItems.empty();
}

template <typename T>
void
unittest::smpc::TopicLogger<T>::onReceive(T* item)
{
    mItems.push_back(*item);
}

#endif
