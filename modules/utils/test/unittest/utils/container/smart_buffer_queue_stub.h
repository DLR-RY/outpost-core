/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef UNITTEST_UTILS_SMART_BUFFER_QUEUE_STUB_H_
#define UNITTEST_UTILS_SMART_BUFFER_QUEUE_STUB_H_

#include <outpost/time/duration.h>

#include <queue>

namespace unittest
{
namespace utils
{
template <size_t N>
class SharedBufferQueue
{
public:
    SharedBufferQueue()
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
    send(const outpost::utils::SharedBufferPointer& data)
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
    receive(outpost::utils::SharedBufferPointer& data, outpost::time::Duration)
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
    std::queue<outpost::utils::SharedBufferPointer> mQueue;

    outpost::utils::SharedBufferPointer _empty;

    outpost::utils::SharedBufferPointer mPointers[N];
    bool isUsed[N];
};

}  // namespace utils
}  // namespace unittest

#endif /* MODULES_UTILS_TEST_UNITTEST_UTILS_SMART_BUFFER_QUEUE_STUB_H_ */
