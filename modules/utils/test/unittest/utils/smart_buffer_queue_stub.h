/*
 * Copyright (c) 2016-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2018, Jan-Gerd Mess (DLR RY-AVS)
 * - 2018, Fabian Greif (DLR RY-AVS)
 */

#ifndef UNITTEST_UTILS_SMART_BUFFER_QUEUE_STUB_H_
#define UNITTEST_UTILS_SMART_BUFFER_QUEUE_STUB_H_

#include <outpost/time/duration.h>

#include <queue>

namespace unittest
{
namespace utils
{
template <size_t N>
class SmartBufferQueue
{
public:
    SmartBufferQueue()
    {
        for (size_t i = 0; i < N; i++)
        {
            isUsed[i] = false;
        }
    }

    bool
    isEmpty()
    {
        for (size_t i = 0; i < N; i++)
        {
            if (isUsed[i])
                return false;
        }
        return true;
    }

    bool
    isFull()
    {
        for (size_t i = 0; i < N; i++)
        {
            if (!isUsed[i])
                return false;
        }
        return true;
    }

    bool
    send(const outpost::utils::SmartBufferPointer& data)
    {
        bool res = false;
        for (size_t i = 0; i < N; i++)
        {
            if (!isUsed[i])
            {
                mPointers[i] = data;
                isUsed[i] = true;
                res = true;
                mQueue.push(data);
                break;
            }
        }
        return res;
    }

    bool
    receive(outpost::utils::SmartBufferPointer& data, outpost::time::Duration)
    {
        bool res = false;
        if (mQueue.size() > 0)
        {
            for (size_t i = 0; i < N; i++)
            {
                if (mQueue.front() == mPointers[i] && isUsed[i] == true)
                {
                    isUsed[i] = false;
                    res = true;
                    data = mPointers[i];
                    mPointers[i] = _empty;
                    mQueue.pop();
                    break;
                }
            }
        }
        return res;
    }

private:
    std::queue<outpost::utils::SmartBufferPointer> mQueue;

    outpost::utils::SmartBufferPointer _empty;

    outpost::utils::SmartBufferPointer mPointers[N];
    bool isUsed[N];
};

}  // namespace utils
}  // namespace unittest

#endif /* MODULES_UTILS_TEST_UNITTEST_UTILS_SMART_BUFFER_QUEUE_STUB_H_ */
