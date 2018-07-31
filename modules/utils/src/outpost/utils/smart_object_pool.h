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

#ifndef OUTPOST_UTILS_SMART_OBJECT_POOL_H_
#define OUTPOST_UTILS_SMART_OBJECT_POOL_H_

#include <outpost/utils/list.h>
#include <outpost/rtos/mutex_guard.h>

#include "smart_buffer.h"

namespace outpost
{
namespace utils
{

/**
 * Base class of the SmartObjectPool for passing by reference.
 *
 */
template<typename T>
class SmartObjectPoolBase
{
public:
	virtual bool allocate(SmartPointer<T>& pointer) = 0;
	virtual size_t numberOfElements() const = 0;
	virtual size_t numberOfFreeElements() const = 0;

	virtual ~SmartObjectPoolBase()
	{
	}
};

/**
 * A SmartObjectPool holds poolable objects and allows for allocating these when needed.
 *
 * When looking for an unused element,
 * the element next to the one that was allocated last is considered first.
 *
 * \warning T must implement bool isUsed()
 */
template<typename T, size_t E, size_t N>
class SmartObjectPool : public SmartObjectPoolBase<T>
{
public:
	SmartObjectPool() :
		lastIndex(0)
	{
	}

	virtual ~SmartObjectPool()
	{
	}

	bool allocate(SmartPointer<T>& pointer) const
	{
		outpost::rtos::MutexGuard lock(mMutex);
		bool res = false;
		size_t searchLastIndex = (lastIndex-1)%N;
		size_t i = lastIndex;
		do
		{
			if (!mBuffer[i].isUsed())
			{
				pointer = SmartPointer<T>(&mBuffer[i]);
				lastIndex = (i+1)%N;
				res = true;
			}
			i = (i+1)%N;
		} while(i != searchLastIndex && !res);
		return res;
	}

	void print() const
	{
		for (size_t i = 0; i < N; i++)
		{
			printf("SmartBuffer: %u (Address %x) : ", i, &mBuffer[i]);
			if (mBuffer[i].isUsed())
			{
				printf("used\n");
			} else
			{
				printf("free\n");
			}
		}
		printf("\n");
	}

	size_t numberOfElements() const
	{
		return N;
	}

	size_t numberOfFreeElements() const
	{
		outpost::rtos::MutexGuard lock(mMutex);
		size_t temp = 0;
		for (size_t i = 0; i < N; i++)
		{
			if (!mBuffer[i].isUsed())
			{
				temp++;
			}
		}
		return temp;
	}

	virtual size_t getHighWatermark() const
	{
		return 0;
	}

protected:
	T mBuffer[N];

	size_t lastIndex;

	outpost::rtos::Mutex mMutex;
};

/**
 * Specific implementation for SmartBuffers and their storage
 *
 */
template<size_t E, size_t N>
class SmartObjectPool<SmartBuffer, E, N> : public SmartObjectPoolBase<SmartBuffer>
{
public:
	SmartObjectPool() : lastIndex(0)
	{
		for (size_t i = 0; i < N;i++)
		{
			mBuffer[i].setPointer(mDataBuffer[i], E);
		}
	}

	virtual ~SmartObjectPool()
	{
	}

	bool allocate(SmartBufferPointer& pointer)
	{
		outpost::rtos::MutexGuard lock(mMutex);
		bool res = false;
		size_t searchLastIndex = (lastIndex-1)%N;
		size_t i = lastIndex;
		do {
			if (!mBuffer[i].isUsed())
			{
				pointer = SmartBufferPointer(&mBuffer[i]);
				res = true;
				lastIndex = i;
			}
			i = (i+1)%N;
		} while(i != searchLastIndex && !res);
		return res;
	}

	void print() const
	{
		for (unsigned int i = 0; i < N; i++)
		{
			printf("SmartBuffer: %u (Address %p) : ", i, &mBuffer[i]);
			if (mBuffer[i].isUsed())
			{
				printf("used (%lu)\n", mBuffer[i].getReferenceCount());
			} else
			{
				printf("free\n");
			}
		}
		printf("\n");
	}

	size_t numberOfElements() const
	{
		return N;
	}

	size_t numberOfFreeElements() const
	{
		size_t temp = 0;
		for (size_t i = 0; i < N; i++)
		{
			if (!mBuffer[i].isUsed())
			{
				temp++;
			}
		}
		return temp;
	}
protected:
	uint8_t mDataBuffer[N][E] __attribute__ ((aligned (4)));
	SmartBuffer mBuffer[N];

	size_t lastIndex;

	outpost::rtos::Mutex mMutex;
};

template<size_t E, size_t N>
using SmartBufferPool = SmartObjectPool<SmartBuffer, E, N>;

using SmartBufferPoolBase = SmartObjectPoolBase<SmartBuffer>;

}
}

#endif /* MODULES_UTILS_OBJECT_POOL_H_ */
