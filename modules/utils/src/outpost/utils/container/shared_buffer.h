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

#ifndef OUTPOST_UTILS_SMART_BUFFER_H_
#define OUTPOST_UTILS_SMART_BUFFER_H_
#include <outpost/rtos/mutex_guard.h>
#include <outpost/base/slice.h>

#include <stdio.h>
#include <string.h>

#include <array>

namespace outpost
{
namespace utils
{
/**
 * \ingroup SharedBuffer
 * \brief Reference counting byte buffer as the underlying data structure for the
 * SharedBufferPointer
 */
class SharedBuffer
{
public:
    /**
     * \brief Constructor for an empty (invalid) buffer.
     *
     * Needed for array creation. The array then needs to be filled with setPointer(uint8_t*,size_t)
     * or SharedBuffer::setData
     */
    SharedBuffer();

    /**
     * \brief Constructor for creating a SharedBuffer instance from a pointer and a given size in
     * bytes.
     * \param slice Slice holding the byte array.
     */
    SharedBuffer(outpost::Slice<uint8_t> slice) : mReferenceCounter(0), mBuffer(slice)
    {
    }

    /**
     * \brief Standard destructor
     *
     * Should only be called manually if absolutely certain that the buffer is not in use or part of
     * a pool.
     */
    virtual ~SharedBuffer();

    SharedBuffer(const SharedBuffer&) = delete;

    const SharedBuffer&
    operator=(SharedBuffer&) = delete;

    /**
     * \brief Getter function for the underlying byte array.
     * \return Pointer to the byte array.
     */
    inline outpost::Slice<uint8_t>
    getPointer() const
    {
        return mBuffer;
    }

    /**
     * \brief Indexed access to te elements of the underlying byte array
     * \param index Index to be returned. Warning: Any checks for valid index ranges need to be
     * performed by the caller.
     * \return Byte value at the specified index
     */
    inline uint8_t& operator[](size_t index) const
    {
        return mBuffer[index];
    }

    /**
     * \brief Setter function the pointer and size of an unused SharedBuffer. Nothing will happen,
     * if the buffer is currently in use.
     * \param slice Slice holding the new byte buffer.
     */
    inline void
    setPointer(outpost::Slice<uint8_t> slice)
    {
        if (!isUsed())
        {
            mBuffer = slice;
        }
    }

    /**
     * \brief Getter function for the usage state of the SharedBuffer. Calls outpost::rtos::Mutex
     * protected functions internally.
     * \return Returns true if the SharedBuffer is currently in use, false otherwise.
     */
    inline bool
    isUsed() const
    {
        return SharedBuffer::isUsedAtomic(mReferenceCounter);
    }

    /**
     * \brief Getter function of the current reference count.
     * \return Returns the current reference count.
     */
    inline size_t
    getReferenceCount() const
    {
        return mReferenceCounter;
    }

    /**
     * \brief Getter function for the validity state of a SharedBuffer.
     * \return Return true if the underlying byte array is not a nullptr, false otherwise.
     */
    inline bool
    isValid() const
    {
        return mBuffer.getNumberOfElements() > 0;
    }

private:
    friend class SharedBufferPointer;

    /**
     * \brief Increments the reference count.
     *
     * Used by its friend class SharedBufferPointer, it does not need to be called manually.
     * Uses outpost::rtos::Mutex protected functions internally for mutual exclusion.
     */
    void
    incrementCount();

    /**
     * \brief Decrements the reference count.
     *
     * Used by its friend class SharedBufferPointer, it does not need to be called manually.
     * Uses outpost::rtos::Mutex protected functions internally for mutual exclusion.
     */
    void
    decrementCount();

    /**
     * \brief outpost::rtos::Mutex protected access to the usage state, derived from the reference
     * counter.
     *
     * Called internally by the member function isUsed().
     * Does not need to be called manually.
     * \param ref Reference counter to be checked.
     */
    inline static bool
    isUsedAtomic(const size_t& ref)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        return ref != 0;
    }

    /**
     * \brief outpost::rtos::Mutex protected increment of the reference counter.
     *
     * Called internally by the member function isUsed().
     * Does not need to be called manually.
     * \param ref Reference counter to be incremented.
     */
    inline static void
    incrementCountAtomic(size_t& ref)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        ref++;
    }

    /**
     * \brief outpost::rtos::Mutex protected decrement of the reference counter.
     *
     * Called internally by the member function isUsed().
     * Does not need to be called manually.
     * \param ref Reference counter to be decremented.
     */
    inline static void
    decrementCountAtomic(size_t& ref)
    {
        outpost::rtos::MutexGuard lock(mMutex);
        if (ref > 0)
        {
            ref--;
        }
    }

    /**
     * \brief outpost::rtos::Mutex for allowing only one reference counter to be changed at a time.
     */
    static outpost::rtos::Mutex mMutex;

    /**
     * \brief Reference counter for the current usage state.
     */
    size_t mReferenceCounter;

    /**
     * \brief Pointer to the underlying byte array.
     */
    outpost::Slice<uint8_t> mBuffer;
};

class SharedChildPointer;

/**
 * \ingroup SharedBuffer
 *
 * Wrapper class for SharedBuffer instances that handles passing
 * (i.e. copying, assignment, etc.), nesting and
 * provides the means for user-defined type control.
 */
class SharedBufferPointer
{
public:
    /**
     * \brief Constructor for an empty (invalid) SharedBufferPointer.
     *
     * Used for array creation and invalidation of unused SharedBufferPointer instances.
     */
    SharedBufferPointer() : mPtr(nullptr), mType(0), mOffset(0), mLength(0)
    {
    }

    /**
     * \brief Constructor for a SharedBufferPointer instance wrapping a SharedBuffer.
     *
     * \param pT Pointer to the SharedBuffer instance.
     */
    SharedBufferPointer(SharedBuffer* pT) :
        mPtr(pT),
        mType(0),
        mOffset(0),
        mLength([pT]() -> size_t {
            if (pT)
            {
                return pT->mBuffer.getNumberOfElements();
            }
            return 0;
        }())
    {
        incrementCount();
    }

    /**
     * \brief Copy constructor for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be copied.
     */
    SharedBufferPointer(const SharedBufferPointer& other) :
        mPtr(other.mPtr),
        mType(other.mType),
        mOffset(other.mOffset),
        mLength(other.mLength)
    {
        incrementCount();
    }

    /**
     * \brief Move constructor for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    SharedBufferPointer(const SharedBufferPointer&& other) :
        mPtr(other.mPtr),
        mType(other.mType),
        mOffset(other.mOffset),
        mLength(other.mLength)
    {
        incrementCount();
    }

    /**
     * \brief Standard deconstructor.
     *
     * May be called as soon as the SharedBufferPointer instance is not needed anymore.
     * It is recommended to leave this to the standard C++ scope or overwrite an existing reference:
     * \code{.cpp}
     * mBuffer = SharedBufferPointer();
     * \endcode
     *
     * The buffer itself, however, can only be reused once its reference counter reaches 0.
     *
     */
    virtual ~SharedBufferPointer()
    {
        decrementCount();
    }

    /**
     * \brief Assignment operator for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
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

    /**
     * \brief Move operator for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
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

    /**
     * \brief Getter function for the validity of a SharedBufferPointer and its underlying
     * SharedBuffer.
     *
     * \return Returns true if the underlying SharedBuffer is neither a nullptr nor invalid,
     * otherwise false.
     */
    inline bool
    isValid() const
    {
        return mPtr != nullptr && mPtr->isValid();
    }

    /**
     * \brief Getter function for whether the current buffer is a nested buffer of some other
     * SharedBufferPointer instance.
     *
     * \return Always returns false for instances of SharedBufferPointer, but needed for
     * inheritance.
     */
    virtual inline bool
    isChild() const
    {
        return false;
    }

    /**
     * \brief Getter function for a child (nested) SharedBufferPointer that is working on the same
     * SharedBuffer.
     *
     * \param ptr Reference to which the nested buffer will be assigned
     * \param type Type id of the nested buffer for identification of purpose (e.g. protocol, etc.)
     * \param pOffset Offset of the nested SharedBufferPointer
     * \param length Length of the nested SharedBufferPointer in bytes
     * \return Returns true if the underlying SharedBuffer is neither a nullptr nor invalid,
     * otherwise wise.
     */
    bool
    getChild(SharedChildPointer& ptr, uint16_t type, size_t pOffest, size_t length) const;

    inline SharedBuffer* operator->() const
    {
        return mPtr;
    }

    /**
     * \brief Dereferencing operator will return the underlying SharedBuffer to allow access to the
     * data.
     *
     * Warning: This also means that pointers to SharedBufferPointer instances may not be passed
     * around, since this breaks the reference counting mechanism. Use references instead!
     *
     * \return Returns a reference to the underlying SharedBuffer instance
     */
    inline SharedBuffer& operator*() const
    {
        return *mPtr;
    }

    /**
     * \brief Comparison operator for two SharedBufferPointer instances.
     *
     * Two SharedBufferPointer instances are considered equal
     * if their underlying SharedBuffer instances are the same.
     * \param other Reference to the SharedBufferPointer instance to be compared
     * \return Returns true if the SharedBufferPointer instances are equal, otherwise false.
     */
    inline bool
    operator==(const SharedBufferPointer& other) const
    {
        return mPtr == other.mPtr;
    }

    /**
     * \brief Comparison operator for SharedBufferPointer instances and void pointers.
     *
     * A SharedBufferPointer instance is considered equal to a void pointer
     * if the pointer points to the underlying SharedBuffer instance.
     * \param other Void pointer to be compared to the SharedBuffer instance.
     * \return Returns true if the void pointer points to the SharedBuffer instance, otherwise
     * false.
     */
    inline bool
    operator==(const void* other) const
    {
        return mPtr == other;
    }

    /**
     * \brief Comparison operator for two SharedBufferPointer instances.
     *
     * \see operator==(const SharedBufferPointer& other)
     * \param other Reference to the SharedBufferPointer instance to be compared
     * \return Returns true if the SharedBufferPointer instances are not equal, otherwise false.
     */
    inline bool
    operator!=(const SharedBufferPointer& other) const
    {
        return mPtr != other.mPtr;
    }

    /**
     * \brief Comparison operator for SharedBufferPointer instances and void pointers.
     *
     * A SharedBufferPointer instance is considered equal to a void pointer
     * if the pointer points to the underlying SharedBuffer instance.
     * \see operator==(const void* other)
     * \param other Void pointer to be compared to the SharedBuffer instance.
     * \return Returns true if the void pointer does not point to the SharedBuffer instance,
     * otherwise false.
     */
    inline bool
    operator!=(const void* other) const
    {
        return mPtr != other;
    }

    /**
     * \brief Cast operator to a uint8_t pointer.
     *
     * Returns a uint8_t pointer to the underlying SharedBuffer including a possible offset.
     * If the underlying SharedBuffer is not valid, a nullptr is returned instead.
     * \return Returns a uint8_t pointer.
     */
    inline operator uint8_t*() const
    {
        if (mPtr->isValid())
        {
            return &(*mPtr)[mOffset];
        }
        else
        {
            return nullptr;
        }
    }

    /**
     * \brief Indexing operator.
     *
     * Returns a uint8_t reference to the underlying SharedBuffer including a possible offset.
     *
     * \return Returns a uint8_t reference to the SharedBuffer.
     */
    inline uint8_t& operator[](size_t index) const
    {
        return (*mPtr)[index + mOffset];
    }

    /**
     * \brief Getter function for the buffer's maximum length.
     *
     * \return Returns the buffers maximum size or zero if it is not valid.
     */
    inline size_t
    getLength() const
    {
        if (isValid())
        {
            return mLength;
        }
        return 0;
    }

    /**
     * \brief Cast operator to a outpost::Slice.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline operator outpost::Slice<uint8_t>() const
    {
        return mPtr->getPointer().subSlice(mOffset, mLength);
    }

    /**
     * \brief Cast operator to a outpost::Slice.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline operator outpost::Slice<const uint8_t>() const
    {
        return mPtr->getPointer().subSlice(mOffset, mLength);
    }

    /**
     * \brief Conversion function to an outpost::Slice slice of the underlying SharedBuffer
     * including a possible offset.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline outpost::Slice<uint8_t>
    asSlice() const
    {
        return mPtr->getPointer();
    }

    /**
     * \brief Setter function for the buffers type id, that can be user-defined.
     *
     * \param type The type id to be set for the current SharedBufferPointer
     */
    inline void
    setType(uint16_t type)
    {
        mType = type;
    }

    /**
     * \brief Getter function for the buffers type id, that can be user-defined.
     *
     * \return Returns the type id to be set for the current SharedBufferPointer
     */
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

/**
 * \ingroup SharedBuffer
 * \brief Nested derivative of SharedBufferPointer that also features a SharedBufferPointer to its
 * parent.
 */
class SharedChildPointer : public SharedBufferPointer
{
public:
    friend SharedBufferPointer;

    SharedChildPointer() = default;

    /**
     * \brief Copy constructor for a SharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be copied.
     */
    SharedChildPointer(const SharedChildPointer& other) :
        SharedBufferPointer(other),
        mParent(other.mParent)
    {
    }

    /**
     * \brief Move constructor for a SharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be moved.
     */
    SharedChildPointer(const SharedChildPointer&& other) :
        SharedBufferPointer(other),
        mParent(other.mParent)
    {
    }

    /**
     * \brief Assignment operator for a SharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be assigned.
     */
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

    /**
     * \brief Move operator for a SharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be moved.
     */
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

    virtual ~SharedChildPointer() = default;

    /**
     * \brief Getter function for the SharedChildPointer instance's parent buffer.
     *
     * \return Returns a copy of the SharedChildPointer instance's parent buffer
     */
    SharedBufferPointer
    getParent() const
    {
        return mParent;
    }

    /**
     * \brief Getter function for the SharedChildPointer instance's root buffer.
     *
     * \return Returns a copy of the SharedChildPointer instance's root buffer
     */
    SharedBufferPointer
    getOrigin() const
    {
        return SharedBufferPointer(mPtr);
    }

    /**
     * \brief Getter function for whether  the SharedChildPointer is actually a child buffer of some
     * valid parent.
     *
     * \return Returns true if the SharedChildPointer instance's parent is valid.
     */
    virtual inline bool
    isChild() const
    {
        return mParent.isValid();
    }

private:
    SharedChildPointer(SharedBuffer* pT, const SharedBufferPointer& parent) :
        SharedBufferPointer(pT),
        mParent(parent)
    {
    }

    SharedBufferPointer mParent;
};

}  // namespace utils
}  // namespace outpost

#endif /* OUTPOST_UTILS_SMART_BUFFER_H_ */
