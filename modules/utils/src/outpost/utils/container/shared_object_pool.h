/*
 * Copyright (c) 2017-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017-2018, Jan-Gerd Mess (DLR RY-AVS)
 * - 2018, Fabian Greif (DLR RY-AVS)
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_SMART_OBJECT_POOL_H_
#define OUTPOST_UTILS_SMART_OBJECT_POOL_H_

#include "shared_buffer.h"

#include <outpost/rtos/mutex_guard.h>
#include <outpost/utils/container/list.h>

namespace outpost
{
namespace utils
{
/**
 * \ingroup SharedBuffer
 * \brief Base class of the SharedObjectPool for passing instances by reference.
 *
 */
class SharedBufferPoolBase
{
public:
    /**
     * \brief Allocation of an unused SharedBufferPoiner from the pool.
     *
     * \param pointer Reference to the SharedBufferPointer
     * \return Returns true if a valid SharedBudderPointer was found, otherwise false.
     */
    virtual bool
    allocate(SharedBufferPointer& pointer) = 0;

    /**
     * \brief Allocation of an unused ConstSharedBufferPoiner from the pool.
     *
     * \param pointer Reference to the ConstSharedBufferPointer
     * \return Returns true if a valid ConstSharedBudderPointer was found, otherwise false.
     */
    virtual bool
    allocate(ConstSharedBufferPointer& pointer) = 0;

    /**
     * \brief Getter function for the overall number of elements in the pool.
     *
     * \return Returns the overall number of items in the pool, including occupied and unused
     * buffers.
     */
    virtual size_t
    numberOfElements() const = 0;

    /**
     * @brief Gets the size expected of a single element.
     *
     * @return size allocated to each individual element.
     */
    virtual size_t
    getElementSize() const = 0;

    /**
     * \brief Getter function for the number of free elements in the pool.
     *
     * \return Returns the number of unused items in the pool.
     */
    virtual size_t
    numberOfFreeElements() const = 0;

    /**
     * \brief Default destructor.
     *
     * Should not be called unless absolutely certain that all buffers in the pool are unused,
     * since this might free their underlying memory.
     */
    virtual ~SharedBufferPoolBase() = default;
};

/**
 * \ingroup SharedBuffer
 * \brief A SharedBufferPool holds SharedBuffer instances and allows for allocating matching
 * SharedBufferPointer instances these when needed.
 *
 * \tparam E Length of a single element in bytes
 * \tparam N Number of elements
 */
template <size_t E, size_t N>
class ExternalSharedBufferPool : public SharedBufferPoolBase
{
public:
    explicit ExternalSharedBufferPool(uint8_t* address) : mLastIndex(0)
    {
        initialize(address);
    }

    /**
     * \brief Default destructor.
     *
     * Should not be called unless absolutely certain that all buffers in the pool are unused.
     */
    virtual ~ExternalSharedBufferPool() = default;

    /**
     * \brief Allocation of an unused SharedBufferPoiner from the pool.
     *
     *  When looking for an unused element,
     *  the element next to the one that was allocated last is considered first.
     *
     * \param pointer Reference to the SharedBufferPointer
     * \return Returns true if a valid SharedBudderPointer was found, otherwise false.
     */
    bool
    allocate(SharedBufferPointer& pointer) override
    {
        outpost::rtos::MutexGuard lock(mMutex);
        size_t i = mLastIndex;
        bool res = false;
        do
        {
            if (!mBuffer[i].isUsed())
            {
                pointer = SharedBufferPointer(&mBuffer[i]);
                res = true;
                mLastIndex = (i + 1) % N;
            }
            i = (i + 1) % N;
        } while (i != mLastIndex && !res);
        return res;
    }

    /**
     * \brief Allocation of an unused SharedBufferPoiner from the pool.
     *
     *  When looking for an unused element,
     *  the element next to the one that was allocated last is considered first.
     *
     * \param pointer Reference to the SharedBufferPointer
     * \return Returns true if a valid SharedBudderPointer was found, otherwise false.
     */
    bool
    allocate(ConstSharedBufferPointer& pointer) override
    {
        outpost::rtos::MutexGuard lock(mMutex);
        size_t i = mLastIndex;
        bool res = false;
        do
        {
            if (!mBuffer[i].isUsed())
            {
                pointer = ConstSharedBufferPointer(&mBuffer[i]);
                res = true;
                mLastIndex = (i + 1) % N;
            }
            i = (i + 1) % N;
        } while (i != mLastIndex && !res);
        return res;
    }

    /**
     * \brief Getter function for the overall number of elements in the pool.
     *
     * \return Returns the overall number of items in the pool, including occupied and unused
     * buffers.
     */
    inline size_t
    numberOfElements() const override
    {
        return N;
    }

    inline size_t
    getElementSize() const override
    {
        return E;
    }

    /**
     * \brief Getter function for the number of free elements in the pool.
     *
     * \return Returns the number of unused items in the pool.
     */
    size_t
    numberOfFreeElements() const override
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
    SharedBuffer mBuffer[N];

    size_t mLastIndex;

    outpost::rtos::Mutex mMutex;

    void
    initialize(uint8_t* address)
    {
        for (size_t i = 0; i < N; i++)
        {
            mBuffer[i].setPointer(outpost::Slice<uint8_t>::unsafe(address + i * E, E));
        }
    };
};

template <size_t E, size_t N, size_t Alignment = 1>
class SharedBufferPoolStorage
{
protected:
    SharedBufferPoolStorage() : mDataBuffer(){};

    /**
     * \brief Default destructor.
     *
     * Should not be called unless absolutely certain that all buffers in the pool are unused.
     */
    virtual ~SharedBufferPoolStorage() = default;

    std::array<std::array<uint8_t, E>, N>&
    getDataBuffer(void)
    {
        return mDataBuffer;
    }

public:
    static bool constexpr isValidAlignment()
    {
        return E % Alignment == 0;
    }

protected:
    static_assert(isValidAlignment(), "Object Size is not a multiple of the Alignment");
    alignas(Alignment) std::array<std::array<uint8_t, E>, N> mDataBuffer;
};

/**
 * \ingroup SharedBuffer
 * \brief The ExternalSharedBufferPool is a SharedBufferPool on external linked storage.
 *
 * \tparam E Length of a single element in bytes
 * \tparam N Number of elements
 */
template <size_t E, size_t N, size_t Alignment = 1>
class SharedBufferPool : public SharedBufferPoolStorage<E, N, Alignment>,
                         public ExternalSharedBufferPool<E, N>
{
public:
    SharedBufferPool() : ExternalSharedBufferPool<E, N>(&(this->getDataBuffer()[0][0])){};

    /**
     * \brief Default destructor.
     *
     * Should not be called unless absolutely certain that all buffers in the pool are unused.
     */
    virtual ~SharedBufferPool() = default;
};

}  // namespace utils
}  // namespace outpost

#endif /* MODULES_UTILS_OBJECT_POOL_H_ */
