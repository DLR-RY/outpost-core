/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
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

#ifndef OUTPOST_UTILS_SMART_BUFFER_H_
#define OUTPOST_UTILS_SMART_BUFFER_H_
#include <outpost/base/slice.h>
#include <outpost/rtos/mutex_guard.h>

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
    explicit SharedBuffer(outpost::Slice<uint8_t> slice) : mReferenceCounter(0), mBuffer(slice)
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
    // cppcheck-suppress functionConst
    getPointer()
    {
        return mBuffer;
    }

    /**
     * \brief Getter function for the underlying byte array.
     * \return Pointer to the byte array.
     */
    inline outpost::Slice<const uint8_t>
    getPointer() const
    {
        return mBuffer;
    }

    /**
     * \brief Indexed access to the elements of the underlying byte array
     *
     * \param index Index to be returned. Warning: Any checks for valid index ranges need to be
     * performed by the caller.
     *
     * \return Byte value at the specified index
     */
    inline uint8_t&
    operator[](int64_t index)
    {
        return mBuffer[index];
    }

    /**
     * \brief Indexed access to the elements of the underlying byte array
     *
     * \param index Index to be returned. Warning: Any checks for valid index ranges need to be
     * performed by the caller.
     *
     * \return Byte value at the specified index
     */
    inline const uint8_t&
    operator[](int64_t index) const
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
    friend class ConstSharedBufferPointer;
    friend class SharedBufferPointerBase;

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
    isUsedAtomic(const size_t ref)
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

/**
 * \ingroup SharedBuffer
 *
 * Wrapper class for SharedBuffer instances that handles passing
 * (i.e. copying, assignment, etc.), nesting and
 * provides the means for user-defined type control.
 */
class SharedBufferPointerBase
{
public:
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
        return (nullptr != mPtr) && mPtr->isValid();
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

    inline bool
    isUsed() const
    {
        return (nullptr != mPtr) && mPtr->isUsed();
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
    operator==(const SharedBufferPointerBase& other) const
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
    operator!=(const SharedBufferPointerBase& other) const
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
    getType() const
    {
        return mType;
    }

protected:
    friend class SharedChildPointer;
    friend class ConstSharedChildPointer;

    /**
     * \brief Constructor for an empty (invalid) SharedBufferPointer.
     *
     * Used for array creation and invalidation of unused SharedBufferPointer instances.
     */
    SharedBufferPointerBase() : mPtr(nullptr), mType(0), mOffset(0), mLength(0)
    {
    }

    /**
     * \brief Constructor for a SharedBufferPointer instance wrapping a SharedBuffer.
     *
     * \param pT Pointer to the SharedBuffer instance.
     */
    explicit SharedBufferPointerBase(SharedBuffer* pT) :
        mPtr(pT), mType(0), mOffset(0), mLength([pT]() -> size_t {
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
    SharedBufferPointerBase(const SharedBufferPointerBase& other) :
        mPtr(other.mPtr), mType(other.mType), mOffset(other.mOffset), mLength(other.mLength)
    {
        incrementCount();
    }

    /**
     * \brief Move constructor for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    SharedBufferPointerBase(const SharedBufferPointerBase&& other) :
        mPtr(other.mPtr), mType(other.mType), mOffset(other.mOffset), mLength(other.mLength)
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
    virtual ~SharedBufferPointerBase()
    {
        decrementCount();
    }

    /**
     * \brief Assignment operator for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    SharedBufferPointerBase&
    operator=(const SharedBufferPointerBase& other)
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
    SharedBufferPointerBase&
    operator=(const SharedBufferPointerBase&& other)
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

    void
    incrementCount() const
    {
        if (nullptr != mPtr)
        {
            mPtr->incrementCount();
        }
    }

    void
    decrementCount() const
    {
        if (nullptr != mPtr)
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

class SharedChildPointer;

/**
 * \ingroup SharedBuffer
 *
 * Wrapper class for SharedBuffer instances that handles passing
 * (i.e. copying, assignment, etc.), nesting and
 * provides the means for user-defined type control.
 */
class SharedBufferPointer : public SharedBufferPointerBase
{
public:
    /**
     * \brief Constructor for an empty (invalid) SharedBufferPointer.
     *
     * Used for array creation and invalidation of unused SharedBufferPointer instances.
     */
    SharedBufferPointer() : SharedBufferPointerBase()
    {
    }

    /**
     * \brief Constructor for a SharedBufferPointer instance wrapping a SharedBuffer.
     *
     * \param pT Pointer to the SharedBuffer instance.
     */
    explicit SharedBufferPointer(SharedBuffer* pT) : SharedBufferPointerBase(pT)
    {
    }

    /**
     * \brief Copy constructor for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be copied.
     */
    SharedBufferPointer(const SharedBufferPointer& other) : SharedBufferPointerBase(other)
    {
    }

    /**
     * \brief Move constructor for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    SharedBufferPointer(const SharedBufferPointer&& other) : SharedBufferPointerBase(other)
    {
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
    virtual ~SharedBufferPointer() = default;

    /**
     * \brief Assignment operator for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    SharedBufferPointer&
    operator=(const SharedBufferPointer& other)
    {
        SharedBufferPointerBase::operator=(other);
        return *this;
    }

    /**
     * \brief Move operator for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    SharedBufferPointer&
    operator=(const SharedBufferPointer&& other)
    {
        SharedBufferPointerBase::operator=(other);
        return *this;
    }

    /**
     * \brief Getter function for whether the current buffer is a nested buffer of some other
     * SharedBufferPointer instance.
     *
     * \return Always returns false for instances of SharedBufferPointer, but needed for
     * inheritance.
     */
    virtual inline bool
    isChild() const override
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
     * otherwise false.
     */
    bool
    getChild(SharedChildPointer& ptr, uint16_t type, size_t pOffset, size_t length) const;

    inline SharedBuffer*
    operator->() const
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
    inline SharedBuffer&
    operator*() const
    {
        return *mPtr;
    }

    /**
     * \brief Cast operator to a uint8_t pointer.
     *
     * Returns a uint8_t pointer to the underlying SharedBuffer including a possible offset.
     * If the underlying SharedBuffer is not valid, a nullptr is returned instead.
     * \return Returns a uint8_t pointer.
     */
    inline
    operator uint8_t*() const
    {
        if ((nullptr != mPtr) && mPtr->isValid())
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
    inline uint8_t&
    operator[](int64_t index) const
    {
        return (*mPtr)[index + mOffset];
    }

    /**
     * \brief Cast operator to a outpost::Slice.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline operator outpost::Slice<const uint8_t>() const
    {
        if (nullptr != mPtr)
        {
            return mPtr->getPointer().subSlice(mOffset, mLength);
        }
        else
        {
            return outpost::Slice<const uint8_t>::empty();
        }
    }

    /**
     * \brief Cast operator to a outpost::Slice.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline operator outpost::Slice<uint8_t>() const
    {
        if (nullptr != mPtr)
        {
            return mPtr->getPointer().subSlice(mOffset, mLength);
        }
        else
        {
            return outpost::Slice<uint8_t>::empty();
        }
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
        if (nullptr != mPtr)
        {
            return mPtr->getPointer().subSlice(mOffset, mLength);
        }
        else
        {
            return outpost::Slice<uint8_t>::empty();
        }
    }

private:
    friend class SharedChildPointer;

    /**
     * \brief Copy constructor for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be copied.
     */
    // cppcheck-suppress noExplicitConstructor
    SharedBufferPointer(const SharedBufferPointerBase& other) : SharedBufferPointerBase(other)
    {
    }
};

class ConstSharedChildPointer;

/**
 * \ingroup SharedBuffer
 *
 * Wrapper class for SharedBuffer instances that handles passing
 * (i.e. copying, assignment, etc.), nesting and
 * provides the means for user-defined type control.
 * This version prevent changes to the underlying data
 */
class ConstSharedBufferPointer : public SharedBufferPointerBase
{
public:
    /**
     * \brief Constructor for an empty (invalid) ConstSharedBufferPointer.
     *
     * Used for array creation and invalidation of unused ConstSharedBufferPointer instances.
     */
    ConstSharedBufferPointer() : SharedBufferPointerBase()
    {
    }

    /**
     * \brief Constructor for a ConstSharedBufferPointer instance wrapping a SharedBuffer.
     *
     * \param pT Pointer to the SharedBuffer instance.
     */
    explicit ConstSharedBufferPointer(SharedBuffer* pT) : SharedBufferPointerBase(pT)
    {
    }

    /**
     * \brief Copy constructor for a ConstSharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be copied.
     */
    // cppcheck-suppress noExplicitConstructor
    ConstSharedBufferPointer(const SharedBufferPointer& other) : SharedBufferPointerBase(other)
    {
    }

    /**
     * \brief Copy constructor for a ConstSharedBufferPointer instance.
     *
     * \param other Reference of the ConstSharedBufferPointer instance to be copied.
     */
    ConstSharedBufferPointer(const ConstSharedBufferPointer& other) : SharedBufferPointerBase(other)
    {
    }

    /**
     * \brief Move constructor for a ConstSharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    // cppcheck-suppress noExplicitConstructor
    ConstSharedBufferPointer(const SharedBufferPointer&& other) : SharedBufferPointerBase(other)
    {
    }

    /**
     * \brief Move constructor for a ConstSharedBufferPointer instance.
     *
     * \param other Reference of the ConstSharedBufferPointer instance to be moved.
     */
    ConstSharedBufferPointer(const ConstSharedBufferPointer&& other) :
        SharedBufferPointerBase(other)
    {
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
    virtual ~ConstSharedBufferPointer() = default;

    /**
     * \brief Assignment operator for a COnstSharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    ConstSharedBufferPointer&
    operator=(const SharedBufferPointer& other)
    {
        SharedBufferPointerBase::operator=(other);
        return *this;
    }

    /**
     * \brief Move operator for a ConstSharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be moved.
     */
    ConstSharedBufferPointer&
    operator=(const SharedBufferPointer&& other)
    {
        SharedBufferPointerBase::operator=(other);
        return *this;
    }

    /**
     * \brief Assignment operator for a ConstSharedBufferPointer instance.
     *
     * \param other Reference of the ConstSharedBufferPointer instance to be moved.
     */
    ConstSharedBufferPointer&
    operator=(const ConstSharedBufferPointer& other)
    {
        SharedBufferPointerBase::operator=(other);
        return *this;
    }

    /**
     * \brief Move operator for a ConstSharedBufferPointer instance.
     *
     * \param other Reference of the ConstSharedBufferPointer instance to be moved.
     */
    ConstSharedBufferPointer&
    operator=(const ConstSharedBufferPointer&& other)
    {
        SharedBufferPointerBase::operator=(other);
        return *this;
    }

    /**
     * \brief Getter function for whether the current buffer is a nested buffer of some other
     * SharedBufferPointer instance.
     *
     * \return Always returns false for instances of ConstSharedBufferPointer, but needed for
     * inheritance.
     */
    virtual inline bool
    isChild() const override
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
    getChild(ConstSharedChildPointer& ptr, uint16_t type, size_t pOffset, size_t length) const;

    inline const SharedBuffer*
    operator->() const
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
    inline const SharedBuffer&
    operator*() const
    {
        return *mPtr;
    }

    /**
     * \brief Cast operator to a uint8_t pointer.
     *
     * Returns a uint8_t pointer to the underlying SharedBuffer including a possible offset.
     * If the underlying SharedBuffer is not valid, a nullptr is returned instead.
     * \return Returns a uint8_t pointer.
     */
    inline operator const uint8_t*() const
    {
        if ((nullptr != mPtr) && mPtr->isValid())
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
     * \return Returns a uint8_t reference to the ConstSharedBuffer.
     */
    inline const uint8_t&
    operator[](int64_t index) const
    {
        return (*mPtr)[index + mOffset];
    }

    /**
     * \brief Cast operator to a outpost::Slice.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline operator outpost::Slice<const uint8_t>() const
    {
        if (nullptr != mPtr)
        {
            return mPtr->getPointer().subSlice(mOffset, mLength);
        }
        else
        {
            return outpost::Slice<const uint8_t>::empty();
        }
    }

    /**
     * \brief Conversion function to an outpost::Slice slice of the underlying SharedBuffer
     * including a possible offset.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline outpost::Slice<const uint8_t>
    asSlice() const
    {
        if (nullptr != mPtr)
        {
            return mPtr->getPointer().subSlice(mOffset, mLength);
        }
        else
        {
            return outpost::Slice<const uint8_t>::empty();
        }
    }

private:
    friend class ConstSharedChildPointer;

    /**
     * \brief Copy constructor for a SharedBufferPointer instance.
     *
     * \param other Reference of the SharedBufferPointer instance to be copied.
     */
    // cppcheck-suppress noExplicitConstructor
    ConstSharedBufferPointer(const SharedBufferPointerBase& other) : SharedBufferPointerBase(other)
    {
    }
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
    friend ConstSharedChildPointer;

    SharedChildPointer() = default;

    /**
     * \brief Copy constructor for a SharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be copied.
     */
    SharedChildPointer(const SharedChildPointer& other) :
        SharedBufferPointer(other), mParent(other.mParent)
    {
    }

    /**
     * \brief Move constructor for a SharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be moved.
     */
    SharedChildPointer(const SharedChildPointer&& other) :
        SharedBufferPointer(other), mParent(other.mParent)
    {
    }

    /**
     * \brief Assignment operator for a SharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be assigned.
     */
    SharedChildPointer&
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
    SharedChildPointer&
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
        return SharedBufferPointer(mParent);
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
    isChild() const override
    {
        return mParent.isUsed();
    }

private:
    SharedChildPointer(SharedBuffer* pT, const SharedBufferPointer& parent) :
        SharedBufferPointer(pT), mParent(parent)
    {
    }

    SharedBufferPointerBase mParent;
};

/**
 * \ingroup SharedBuffer
 * \brief Nested derivative of ConstSharedBufferPointer that also features a
 * ConstSharedBufferPointer to its parent.
 */
class ConstSharedChildPointer : public ConstSharedBufferPointer
{
public:
    friend ConstSharedBufferPointer;

    ConstSharedChildPointer() = default;

    /**
     * \brief Copy constructor for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be copied.
     */
    // cppcheck-suppress noExplicitConstructor
    ConstSharedChildPointer(const SharedChildPointer& other) :
        ConstSharedBufferPointer(other), mParent(other.mParent)
    {
    }

    /**
     * \brief Copy constructor for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the ConstSharedChildPointer instance to be copied.
     */
    ConstSharedChildPointer(const ConstSharedChildPointer& other) :
        ConstSharedBufferPointer(other), mParent(other.mParent)
    {
    }

    /**
     * \brief Move constructor for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be moved.
     */
    // cppcheck-suppress noExplicitConstructor
    ConstSharedChildPointer(const SharedChildPointer&& other) :
        ConstSharedBufferPointer(other), mParent(other.mParent)
    {
    }

    /**
     * \brief Move constructor for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the ConstSharedChildPointer instance to be moved.
     */
    ConstSharedChildPointer(const ConstSharedChildPointer&& other) :
        ConstSharedBufferPointer(other), mParent(other.mParent)
    {
    }

    /**
     * \brief Assignment operator for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the ConstSharedChildPointer instance to be assigned.
     */
    ConstSharedChildPointer&
    operator=(const ConstSharedChildPointer& other)
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
     * \brief Assignment operator for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be assigned.
     */
    ConstSharedChildPointer&
    operator=(const SharedChildPointer& other)
    {
        decrementCount();
        mPtr = other.mPtr;
        mType = other.mType;
        mOffset = other.mOffset;
        mLength = other.mLength;
        mParent = other.mParent;
        incrementCount();
        return *this;
    }

    /**
     * \brief Move operator for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the SharedChildPointer instance to be moved.
     */
    ConstSharedChildPointer&
    operator=(const SharedChildPointer&& other)
    {
        decrementCount();
        mPtr = other.mPtr;
        mType = other.mType;
        mOffset = other.mOffset;
        mLength = other.mLength;
        mParent = other.mParent;
        incrementCount();
        return *this;
    }

    /**
     * \brief Move operator for a ConstSharedChildPointer instance.
     *
     * \param other Reference of the ConstSharedChildPointer instance to be moved.
     */
    ConstSharedChildPointer&
    operator=(const ConstSharedChildPointer&& other)
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

    virtual ~ConstSharedChildPointer() = default;

    /**
     * \brief Getter function for the ConstSharedChildPointer instance's parent buffer.
     *
     * \return Returns a copy of the ConstSharedChildPointer instance's parent buffer
     */
    ConstSharedBufferPointer
    getParent() const
    {
        return ConstSharedBufferPointer(mParent);
    }

    /**
     * \brief Getter function for the ConstSharedChildPointer instance's root buffer.
     *
     * \return Returns a copy of the ConstSharedChildPointer instance's root buffer
     */
    ConstSharedBufferPointer
    getOrigin() const
    {
        return ConstSharedBufferPointer(mPtr);
    }

    /**
     * \brief Getter function for whether the ConstSharedChildPointer is actually a child buffer of
     * some valid parent.
     *
     * \return Returns true if the ConstSharedChildPointer instance's parent is valid.
     */
    virtual inline bool
    isChild() const override
    {
        return mParent.isUsed();
    }

private:
    ConstSharedChildPointer(SharedBuffer* pT, const ConstSharedBufferPointer& parent) :
        ConstSharedBufferPointer(pT), mParent(parent)
    {
    }

    SharedBufferPointerBase mParent;
};

}  // namespace utils
}  // namespace outpost

#endif /* OUTPOST_UTILS_SMART_BUFFER_H_ */
