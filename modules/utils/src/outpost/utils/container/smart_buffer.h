/*
 *
 * Copyright (c) 2016, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_RTOS_SMART_BUFFER_H_
#define OUTPOST_RTOS_SMART_BUFFER_H_
#include <outpost/rtos/mutex_guard.h>
#include <outpost/utils/container/slice.h>

#include <stdio.h>
#include <string.h>

#include <array>

namespace outpost
{
namespace utils
{
class SmartBuffer
{
public:
    SmartBuffer();

    template <size_t N>
    SmartBuffer(uint8_t (&array)[N]);

    SmartBuffer(uint8_t* data, size_t e) : mReferenceCounter(0), mBuffer(data), mNumElements(e)
    {
    }

    virtual ~SmartBuffer();

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
        return SmartBuffer::isUsedAtomic(mReferenceCounter);
    }

    inline size_t
    getReferenceCount() const
    {
        return mReferenceCounter;
    }

private:
    friend class SmartBufferPointer;

    SmartBuffer(const SmartBuffer&);
    const SmartBuffer&
    operator=(SmartBuffer&);

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

class ChildSmartPointer;

class SmartBufferPointer
{
public:
    SmartBufferPointer() : mPtr(nullptr), mType(0), mOffset(0), mLength(0)
    {
    }

    SmartBufferPointer(SmartBuffer* pT) :
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

    static inline SmartBufferPointer
    empty()
    {
        return SmartBufferPointer();
    }

    SmartBufferPointer(const SmartBufferPointer& other) :
        mPtr(other.mPtr),
        mType(other.mType),
        mOffset(other.mOffset),
        mLength(other.mLength)
    {
        incrementCount();
    }

    SmartBufferPointer(const SmartBufferPointer&& other) :
        mPtr(other.mPtr),
        mType(other.mType),
        mOffset(other.mOffset),
        mLength(other.mLength)
    {
        incrementCount();
    }

    ~SmartBufferPointer()
    {
        decrementCount();
    }

    const SmartBufferPointer&
    operator=(const SmartBufferPointer& other)
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

    const SmartBufferPointer&
    operator=(const SmartBufferPointer&& other)
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
    getChild(ChildSmartPointer& ptr, uint16_t type, size_t pOffest, size_t length) const;

    inline SmartBuffer* operator->() const
    {
        return mPtr;
    }

    inline SmartBuffer& operator*() const
    {
        return *mPtr;
    }

    inline bool
    operator==(const SmartBufferPointer& other)
    {
        return mPtr == other.mPtr;
    }

    inline bool
    operator==(const void* other)
    {
        return mPtr == other;
    }

    inline bool
    operator!=(const SmartBufferPointer& other)
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
    SmartBuffer* mPtr;

    uint16_t mType;
    size_t mOffset;
    size_t mLength;
};

class ChildSmartPointer : public SmartBufferPointer
{
public:
    friend SmartBufferPointer;

    ChildSmartPointer() : SmartBufferPointer(), mParent(nullptr)
    {
    }

    ChildSmartPointer(SmartBuffer* pT, const SmartBufferPointer& parent) :
        SmartBufferPointer(pT),
        mParent(parent)
    {
    }

    ChildSmartPointer(const ChildSmartPointer& other) :
        SmartBufferPointer(other),
        mParent(other.mParent)
    {
    }

    ChildSmartPointer(const ChildSmartPointer&& other) :
        SmartBufferPointer(other),
        mParent(other.mParent)
    {
    }

    const ChildSmartPointer&
    operator=(const ChildSmartPointer& other)
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

    const ChildSmartPointer&
    operator=(ChildSmartPointer& other)
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

    const ChildSmartPointer&
    operator=(ChildSmartPointer&& other)
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

    const ChildSmartPointer&
    operator=(const ChildSmartPointer&& other)
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

    ~ChildSmartPointer()
    {
    }

    SmartBufferPointer
    getParent() const
    {
        return mParent;
    }

    SmartBufferPointer
    getOrigin() const
    {
        return SmartBufferPointer(mPtr);
    }

    inline bool
    isChild()
    {
        return mParent.isValid();
    }

private:
    SmartBufferPointer mParent;
};

}  // namespace utils
}  // namespace outpost

#include "smart_buffer_impl.h"

#endif /* SRC_MU_COMMON_UTILS_SMART_BUFFER_H_ */
