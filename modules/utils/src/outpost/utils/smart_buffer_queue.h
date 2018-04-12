/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Jan-Gerd Mess (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_SMART_BUFFER_QUEUE_H_
#define OUTPOST_UTILS_SMART_BUFFER_QUEUE_H_

#include <outpost/rtos/queue.h>

#include "smart_buffer.h"

namespace outpost
{
namespace utils
{

class SmartBufferQueueBase : public outpost::rtos::Queue<size_t>
{
public:
	inline
	SmartBufferQueueBase(size_t N) : outpost::rtos::Queue<size_t>(N)
	{
	}

	virtual
	~SmartBufferQueueBase()
	{
	}

	virtual bool
	send(SmartBufferPointer& data) = 0;

	virtual bool
	receive(
			SmartBufferPointer& data,
			outpost::time::Duration timeout = outpost::time::Duration::infinity()) = 0;

	virtual uint16_t
	getNumberOfItems() = 0;
};

template<size_t N>
class SmartBufferQueue : public SmartBufferQueueBase
{
public:
	SmartBufferQueue() : SmartBufferQueueBase(N), itemsInQueue(0), lastIndex(0)
	{
		for (size_t i = 0; i < N; i++)
		{
			isUsed[i] = false;
		}
	}

	bool
	isEmpty()
	{
		outpost::rtos::MutexGuard lock(mMutex);
		for (size_t i = 0; i < N; i++)
		{
			if(isUsed[i])
				return false;
		}
		return true;
	}

	bool
	isFull()
	{
		outpost::rtos::MutexGuard lock(mMutex);
		/*for (size_t i = 0; i < N; i++)
		{
			if(!isUsed[i])
				return false;
		}
		return true;*/
		return itemsInQueue == N;
	}

    virtual bool
    send(SmartBufferPointer& data) override
    {
    	outpost::rtos::MutexGuard lock(mMutex);
    	bool res = false;
    	size_t i = lastIndex;
    	size_t endSearch = (lastIndex-1)%N;
    	//for (size_t i = 0; i < N; i++)
    	do
    	{
    		if (!isUsed[i])
    		{
    			mPointers[i] = data;
    			isUsed[i] = true;
    			lastIndex = (i+1)%N;
    			if (outpost::rtos::Queue<size_t>::send(i))
    			{
    				itemsInQueue++;
    				res = true;
    			} else
    			{
    				isUsed[i] = false;
    				mPointers[i] = _empty;
    			}
    			break;
    		}
    		i = (i+1)%N;
    	} while(i != endSearch);
    	return res;
    }

    virtual bool
    receive(
    		SmartBufferPointer& data,
			outpost::time::Duration timeout = outpost::time::Duration::infinity()) override
    {
    	bool res = false;
    	size_t index;
    	if (outpost::rtos::Queue<size_t>::receive(index, timeout))
    	{
    		outpost::rtos::MutexGuard lock(mMutex);
    		data = SmartBufferPointer(mPointers[index]);
    		mPointers[index] = _empty;
			isUsed[index] = false;
			itemsInQueue--;
			res = true;
    	}
    	return res;
    }

	virtual uint16_t
	getNumberOfItems()
	{
		outpost::rtos::MutexGuard lock(mMutex);
		return itemsInQueue;
	}

private:

    SmartBufferPointer _empty;

    outpost::rtos::Mutex mMutex;

    uint16_t itemsInQueue;

    size_t lastIndex;

    SmartBufferPointer mPointers[N];
	bool isUsed[N];
};

}
}

#endif /* MODULES_UTILS_SRC_MU_COMMON_UTILS_SMART_BUFFER_QUEUE_H_ */
