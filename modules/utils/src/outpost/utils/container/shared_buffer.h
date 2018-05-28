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
 * - 2017-2018, Jan-Gerd Mess (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

/**
 *
 * \defgroup    SharedBuffer
 * \brief       Shared buffers and corresponding allocation and distribution classes
 *
 */

#ifndef OUTPOST_UTILS_SMART_BUFFER_H_
#define OUTPOST_UTILS_SMART_BUFFER_H_
#include <outpost/rtos/mutex_guard.h>
#include <outpost/utils/container/slice.h>

#include <stdio.h>
#include <string.h>

#include <array>

namespace outpost
{
namespace utils
{
class SharedBuffer
{
public:
    SharedBuffer();

    template <size_t N>
    SharedBuffer(uint8_t (&array)[N]);

    SharedBuffer(uint8_t* data, size_t e) : mReferenceCounter(0), mBuffer(data), mNumElements(e)
    {
    }

    virtual ~SharedBuffer();

    SharedBuffer(const SharedBuffer&) = delete;

    const SharedBuffer&
    operator=(SharedBuffer&) = delete;

    inline uint8_t*
    getPointer() const
    {
        return mBuffer;
    }

    inline uint8_t& operator[](size_t index) const
    {
        return mBuffer[index];
    }

    inline void
    setPointer(uint8_t* data, size_t e)
    {
        if (!isUsed())
        {
            mBuffer = data;
            mNumElements = e;
        }
    }

    inline void
    setData(outpost::Slice<uint8_t> array)
    {
        if (!isUsed())
        {
            mBuffer = array.begin();
            mNumElements = array.getNumberOfElements();
        }
    }

    inline bool
    isUsed() const
    {
        return SharedBuffer::isUsedAtomic(mReferenceCounter);
    }

    inline size_t
    getReferenceCount() const
    {
        return mReferenceCounter;
    }

private:
    friend class SharedBufferPointer;

    void
    incrementCount();

    void
    decrementCount();

    inline static bool
    isUsedAtomic(const size_t& ref)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        return ref != 0;
    }

    inline static void
    incrementCountAtomic(size_t& ref)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        ref++;
    }

    inline static void
    decrementCountAtomic(size_t& ref)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (ref > 0)
        {
            ref--;
        }
    }

    static outpost::rtos::Mutex mMutex;

    size_t mReferenceCounter;

    uint8_t* mBuffer;
    size_t mNumElements;
};

class SharedChildPointer;

class SharedBufferPointer
{
public:
    SharedBufferPointer() : mPtr(nullptr), mType(0), mOffset(0), mLength(0)
    {
    }

    SharedBufferPointer(SharedBuffer* pT) :
        mPtr(pT),
        mType(0),
        mOffset(0),
        mLength([pT]() -> size_t {
            if (pT)
            {
                return pT->mNumElements;
            }
            return 0;
        }())
    {
        incrementCount();
    }

    static inline SharedBufferPointer
    empty()
    {
        return SharedBufferPointer();
    }

    SharedBufferPointer(const SharedBufferPointer& other) :
        mPtr(other.mPtr),
        mType(other.mType),
        mOffset(other.mOffset),
        mLength(other.mLength)
    {
        incrementCount();
    }

    SharedBufferPointer(const SharedBufferPointer&& other) :
        mPtr(other.mPtr),
        mType(other.mType),
        mOffset(other.mOffset),
        mLength(other.mLength)
    {
        incrementCount();
    }

    ~SharedBufferPointer()
    {
        decrementCount();
    }

    const SharedBufferPointer&
    operator=(const SharedBufferPointer& other)
    {
        if (&other != this)
        {
            decrementCount();
            mPtr = other.mPtr;
            mType = other.mType;
            mOffset = other.mOffset;
            mLength = other.mLength;
            incrementCount();
        }
        return *this;
    }

    const SharedBufferPointer&
    operator=(const SharedBufferPointer&& other)
    {
        if (&other != this)
        {
            decrementCount();
            mPtr = other.mPtr;
            mType = other.mType;
            mOffset = other.mOffset;
            mLength = other.mLength;
            incrementCount();
        }
        return *this;
    }

    inline bool
    isValid() const
    {
        return mPtr != nullptr;
    }

    inline bool
    isChild()
    {
        return false;
    }

    bool
    getChild(SharedChildPointer& ptr, uint16_t type, size_t pOffest, size_t length) const;

    inline SharedBuffer* operator->() const
    {
        return mPtr;
    }

    inline SharedBuffer& operator*() const
    {
        return *mPtr;
    }

    inline bool
    operator==(const SharedBufferPointer& other)
    {
        return mPtr == other.mPtr;
    }

    inline bool
    operator==(const void* other)
    {
        return mPtr == other;
    }

    inline bool
    operator!=(const SharedBufferPointer& other)
    {
        return mPtr != other.mPtr;
    }

    inline bool
    operator!=(const void* other)
    {
        return mPtr != other;
    }

    inline operator uint8_t*() const
    {
        if (mPtr != nullptr)
        {
            return &(*mPtr)[mOffset];
        }
        else
        {
            return nullptr;
        }
    }

    inline uint8_t& operator[](size_t index) const
    {
        return (*mPtr)[index + mOffset];
    }

    inline size_t
    getLength() const
    {
        if (isValid())
        {
            return mLength;
        }
        return 0;
    }

    inline operator outpost::Slice<uint8_t>() const
    {
        return outpost::Slice<uint8_t>::unsafe(&(*mPtr)[mOffset], mLength);
    }

    inline operator outpost::Slice<const uint8_t>() const
    {
        return outpost::Slice<const uint8_t>::unsafe(&(*mPtr)[mOffset], mLength);
    }

    inline outpost::Slice<uint8_t>
    asSlice() const
    {
        return outpost::Slice<uint8_t>::unsafe(&(*mPtr)[mOffset], mLength);
    }

    inline void
    setType(uint16_t type)
    {
        mType = type;
    }

    inline uint16_t
    getType()
    {
        return mType;
    }

protected:
    void
    incrementCount() const
    {
        if (mPtr != nullptr)
        {
            mPtr->incrementCount();
        }
    }

    void
    decrementCount() const
    {
        if (mPtr != nullptr)
        {
            mPtr->decrementCount();
        }
    }

protected:
    SharedBuffer* mPtr;

    uint16_t mType;
    size_t mOffset;
    size_t mLength;
};

class SharedChildPointer : public SharedBufferPointer
{
public:
    friend SharedBufferPointer;

    SharedChildPointer() : SharedBufferPointer(), mParent(nullptr)
    {
    }

    SharedChildPointer(SharedBuffer* pT, const SharedBufferPointer& parent) :
        SharedBufferPointer(pT),
        mParent(parent)
    {
    }

    SharedChildPointer(const SharedChildPointer& other) :
        SharedBufferPointer(other),
        mParent(other.mParent)
    {
    }

    SharedChildPointer(const SharedChildPointer&& other) :
        SharedBufferPointer(other),
        mParent(other.mParent)
    {
    }

    const SharedChildPointer&
    operator=(const SharedChildPointer& other)
    {
        if (&other != this)
        {
            decrementCount();
            mPtr = other.mPtr;
            mType = other.mType;
            mOffset = other.mOffset;
            mLength = other.mLength;
            mParent = other.mParent;
            incrementCount();
        }
        return *this;
    }

    const SharedChildPointer&
    operator=(SharedChildPointer& other)
    {
        if (&other != this)
        {
            decrementCount();
            mPtr = other.mPtr;
            mType = other.mType;
            mOffset = other.mOffset;
            mLength = other.mLength;
            mParent = other.mParent;
            incrementCount();
        }
        return *this;
    }

    const SharedChildPointer&
    operator=(SharedChildPointer&& other)
    {
        if (&other != this)
        {
            decrementCount();
            mPtr = other.mPtr;
            mType = other.mType;
            mOffset = other.mOffset;
            mLength = other.mLength;
            mParent = other.mParent;
            incrementCount();
        }
        return *this;
    }

    const SharedChildPointer&
    operator=(const SharedChildPointer&& other)
    {
        if (&other != this)
        {
            decrementCount();
            mPtr = other.mPtr;
            mType = other.mType;
            mOffset = other.mOffset;
            mLength = other.mLength;
            mParent = other.mParent;
            incrementCount();
        }
        return *this;
    }

    ~SharedChildPointer()
    {
    }

    SharedBufferPointer
    getParent() const
    {
        return mParent;
    }

    SharedBufferPointer
    getOrigin() const
    {
        return SharedBufferPointer(mPtr);
    }

    inline bool
    isChild()
    {
        return mParent.isValid();
    }

private:
    SharedBufferPointer mParent;
};

}  // namespace utils
}  // namespace outpost

#include "shared_buffer_impl.h"

#endif /* SRC_MU_COMMON_UTILS_SMART_BUFFER_H_ */
