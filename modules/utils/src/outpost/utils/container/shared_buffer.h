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
#include <outpost/utils/container/slice.h>

#include <stdio.h>
#include <string.h>

#include <array>

namespace outpost
{
namespace utils
{
/**
 *
 * \defgroup    SharedBuffer
 * \brief       Shared buffers and corresponding allocation and distribution classes
 *
 * SharedBuffer instances (and SharedBufferPointer instances, which are mostly applied by the user)
 * provide a means to pre-allocate memory blocks (cf. SharedBufferPool) at system startup and fetch them from a pool as needed.
 * The buffer itself will keep track of the number of references to it. Once it is not referenced (i.e. not needed) anymore,
 * the SharedBufferPool will automatically consider it again for new allocation requests.
 * As long as they are reference, SharedBufferPointers can be passed around the system without the need to ever copy their contents.
 * This is especially useful for applications that push around big chunks of data by DMA transfers.
 *
 * In order to represent nested protocols (such as telemetry packets inside telemetry frames or similar),
 * SharedChildPointer instances can be generated that also keep track of their parent buffer.
 * Only if all parent and child buffers are not referenced by some part of the system,
 * the buffer is considered unoccupied and may be reallocated.
 *
 * In order to share buffers among different parts of the system in a thread-safe fashion, SharedBufferQueue (based on outpost::rtos::Queue)
 * and SharedRingBuffer can be used.
 *
 * The application of SharedBufferPointer instances and surrounding peripherals is best explained by a short pseudo-code example:
 *
 * Consider a system, that is composed of two subsytems A, B and C that shall communicate with each other.
 *
 * Subsystem A will periodically generate a report for which it needs memory. For this purpose,
 * it holds a reference to the system-wide memory pool, from which it can allocate blocks.
 * Data is then passed to a queue.
 *
 * \code{.cpp}
 * class SubsystemA
 * {
 * public:
 * 	SubsytemA(SharedBufferPoolBase& pool, SharedBufferQueueBase& queue) :
 * 		mPool(pool),
 * 		mQueue(queue)
 * 	{
 * 	}
 *
 * 	void run()
 * 	{
 * 		// Generate a message to the queue every 5 seconds
 * 		while(true)
 * 		{
 * 			// Code block for releasing p after mQueue.send
 * 			{
 * 				SharedBufferPointer p;
 * 				if(mPool.allocate(p))
 * 				{
 * 					// p now references a valid memory block
 *
 * 					// Generate data for p
 * 					// ...
 * 					// ...
 * 					mQueue.send(p);
 * 				}
 * 			}
 *
 * 			delay(5000);
 * 		}
 * 	}
 *
 * 	SharedBufferPoolBase& mPool;
 * 	SharedBufferQueueBase& mQueue;
 * };
 * \endcode
 *
 * Subsystem B will process messages from the queue and forward them to a serial interface.
 * On request (onRequest()), it will also generate a report, for which it allocates memory from its own pool.
 *
 * \code{.cpp}
 * class SubsystemB
 * {
 * public:
 * 	SubsytemB(SharedBufferQueueBase& queue) :
 * 		mQueue(queue)
 * 	{
 * 	}
 *
 * 	void run()
 * 	{
 * 		// Read messages from the queue, send them to a SerialInterface
 * 		while(true)
 * 		{
 * 			// Code block for releasing p after mQueue.send
 * 			{
 * 				SharedBufferPointer p;
 * 				if(mQueue.receive(p))
 * 				{
 * 					// p now references a valid memory block
 * 					serial.write(&p, p.getLength());
 * 				}
 *
 * 				// Note that p does not have to be freed here.
 * 				// If there are no further references once this block ends,
 * 				// p can be reallocated from the pool immediately.
 * 			}
 * 		}
 * 	}
 *
 * 	void onRequest()
 * 	{
 * 		SharedBufferPointer p;
 *		if(mMemoryPool.allocate(p))
 *		{
 *			// Generate report in p
 *			// ...
 *			// ...
 *			// ...
 *			// Forward it to the queue
 *			mQueue.send(p);
 *		}
 * 	}
 *
 *		SerialInterface serial;
 *		//MemoryPool containing 5 units of 16384 bytes each.
 *		//Note: Every instance can have its own pool if needed.
 *		//Note: Buffers will always (!) return to the pool they have been allocated from.
 *		SharedBufferPool<5, 16384> mMemoryPool;
 *		SharedBufferQueueBase& mQueue;
 * };
 * \endcode
 *
 * Subsystem C will receive packet data  that is surrounded by some extra protocol prefix
 * from a serial interface, check the incoming packets for validity and
 * forward the inner data to the queue.
 * Note that the receive buffer will only be considered for reallocation once all its references and all references to its children
 * are freed.
 *
 * \code{.cpp}
 * class SubsystemC
 * {
 * public:
 * 	SubsytemC(SharedBufferQueueBase& queue) :
 * 		mQueue(queue)
 * 	{
 * 	}
 *
 * 	void run()
 * 	{
 * 		// Read messages from the queue, send them to a SerialInterface
 * 		while(true)
 * 		{
 * 			// Code block for releasing p after mQueue.send
 * 			{
 * 				SharedBufferPointer p;
 * 				if(mMemoryPool.allocate(p))
 * 				{
 * 					// read from a serial interface
 * 					size_t length;
 * 					if(serial.read(&p, length))
 * 					{
 * 						// the serial protocol can now be checked for validity
 * 						// ..
 * 						// ..
 *
 *						// Now, pass only the data part to the next layer.
 *						// The buffer will only be considered unoccupied once all its child references are freed.
 * 						SharedChildPointer ch;
 * 						p.getChildBuffer(ch, protocolId, protocolOffset, p.getLength()-protocolOffset);
 * 						mQueue.send(ch);
 * 					}
 * 				}
 * 				// Note that p does not have to be freed here.
 * 				// If there are no further references once this block ends,
 * 				// p can be reallocated from the pool immediately.
 * 			}
 * 		}
 * 	}
 *
 *		SerialInterface serial;
 *		//MemoryPool containing 20 units of 16384 bytes each.
 *		//Note: Buffers will always (!) return to the pool they have been allocated from.
 *		uint16_t protocolId;
 *		uint16_t protocolOffset;
 *		SharedBufferPool<20, 16384> mMemoryPool;
 *		SharedBufferQueueBase& mQueue;
 * };
 * \endcode
 *
 * Now, the whole system can be assembled.
 * Note that calling the start function of each subsystem will eventually start a dedicated thread
 * and call the run method of each class.
 *
 * \code{.cpp}
 * class System
 * {
 * 	//MemoryPool containing 100 units of 16384 bytes each.
 * 	SharedBufferPool<100, 16384> mMemoryPool;
 * 	SharedBufferQueue<10> mQueue;
 * };
 * \endcode
 *
 * For further information, please contact jan-gerd.mess@dlr.de
 *
 */

/**
 * \ingroup SharedBuffer
 * \brief Reference counting byte buffer as the underlying data structure for the SharedBufferPointer
 */
class SharedBuffer
{
public:

    /**
     * \brief Constructor for an empty (invalid) buffer.
     *
     * Needed for array creation. The array then needs to be filled with setPointer(uint8_t*,size_t) or SharedBuffer::setData
     */
    SharedBuffer();

    /**
     * \brief Constructor for creating a SharedBuffer instance from an array.
     * \tparam N Size of the underlying array. May be implicit.
     * \param array[N] Array to be used as data storage.
     */
    template <size_t N>
    SharedBuffer(uint8_t (&array)[N]);

    /**
     * \brief Constructor for creating a SharedBuffer instance from a pointer and a given size in bytes.
     * \param data Pointer to the byte array.
     * \param e Size of the array in bytes.
     */
    SharedBuffer(uint8_t* data, size_t e) : mReferenceCounter(0), mBuffer(data), mNumElements(e)
    {
    }

    /**
     * \brief Standard destructor
     *
     * Should only be called manually if absolutely certain that the buffer is not in use or part of a pool.
     */
    virtual ~SharedBuffer();

    SharedBuffer(const SharedBuffer&) = delete;

    const SharedBuffer&
    operator=(SharedBuffer&) = delete;

    /**
     * \brief Getter function for the underlying byte array.
     * \return Pointer to the byte array.
     */
    inline uint8_t*
    getPointer() const
    {
        return mBuffer;
    }

    /**
     * \brief Indexed access to te elements of the underlying byte array
     * \param index Index to be returned. Warning: Any checks for valid index ranges need to be performed by the caller.
     * \return Byte value at the specified index
     */
    inline uint8_t& operator[](size_t index) const
    {
    	return mBuffer[index];
    }

    /**
     * \brief Setter function the pointer and size of an unused SharedBuffer. Nothing will happen, if the buffer is currently in use.
     * \param data Pointer to the new byte buffer.
     * \param e Size of the buffer in bytes.
     */
    inline void
    setPointer(uint8_t* data, size_t e)
    {
        if (!isUsed())
        {
            mBuffer = data;
            mNumElements = e;
        }
    }

    /**
     * \brief Setter function the pointer and size of an unused SharedBuffer from an outpost::Slice. Nothing will happen, if the buffer is currently in use.
     * \param array Slice to be used as underlying byte array.
     */
    inline void
    setData(outpost::Slice<uint8_t> array)
    {
        if (!isUsed())
        {
            mBuffer = array.begin();
            mNumElements = array.getNumberOfElements();
        }
    }

    /**
     * \brief Getter function for the usage state of the SharedBuffer. Calls outpost::rtos::Mutex protected functions internally.
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
    	return mBuffer != nullptr;
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
     * \brief outpost::rtos::Mutex protected access to the usage state, derived from the reference counter.
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
    uint8_t* mBuffer;

    /**
      * \brief Size of the underlying array in bytes.
      */
    size_t mNumElements;
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
                return pT->mNumElements;
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
     * \brief Getter function for the validity of a SharedBufferPointer and its underlying SharedBuffer.
     *
     * \return Returns true if the underlying SharedBuffer is neither a nullptr nor invalid, otherwise false.
     */
    inline bool
    isValid() const
    {
        return mPtr != nullptr && mPtr->isValid();
    }

    /**
     * \brief Getter function for whether the current buffer is a nested buffer of some other SharedBufferPointer instance.
     *
     * \return Always returns false for instances of SharedBufferPointer, but needed for inheritance.
     */
    virtual inline bool
    isChild()
    {
        return false;
    }

    /**
     * \brief Getter function for a child (nested) SharedBufferPointer that is working on the same SharedBuffer.
     *
     * \param ptr Reference to which the nested buffer will be assigned
     * \param type Type id of the nested buffer for identification of purpose (e.g. protocol, etc.)
     * \param pOffset Offset of the nested SharedBufferPointer
     * \param length Length of the nested SharedBufferPointer in bytes
     * \return Returns true if the underlying SharedBuffer is neither a nullptr nor invalid, otherwise wise.
     */
    bool
    getChild(SharedChildPointer& ptr, uint16_t type, size_t pOffest, size_t length) const;

    inline SharedBuffer* operator->() const
    {
        return mPtr;
    }

    /**
     * \brief Dereferencing operator will return the underlying SharedBuffer to allow access to the data.
     *
     * Warning: This also means that pointers to SharedBufferPointer instances may not be passed around,
     * since this breaks the reference counting mechanism. Use references instead!
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
    operator==(const SharedBufferPointer& other)
    {
        return mPtr == other.mPtr;
    }

    /**
     * \brief Comparison operator for SharedBufferPointer instances and void pointers.
     *
     * A SharedBufferPointer instance is considered equal to a void pointer
     * if the pointer points to the underlying SharedBuffer instance.
     * \param other Void pointer to be compared to the SharedBuffer instance.
     * \return Returns true if the void pointer points to the SharedBuffer instance, otherwise false.
     */
    inline bool
    operator==(const void* other)
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
    operator!=(const SharedBufferPointer& other)
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
     * \return Returns true if the void pointer does not point to the SharedBuffer instance, otherwise false.
     */
    inline bool
    operator!=(const void* other)
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
        return outpost::Slice<uint8_t>::unsafe(&(*mPtr)[mOffset], mLength);
    }

    /**
     * \brief Cast operator to a outpost::Slice.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline operator outpost::Slice<const uint8_t>() const
    {
        return outpost::Slice<const uint8_t>::unsafe(&(*mPtr)[mOffset], mLength);
    }

    /**
     * \brief Conversion function to an outpost::Slice slice of the underlying SharedBuffer including a possible offset.
     *
     * \return Returns a outpost::Slice of the data buffer.
     */
    inline outpost::Slice<uint8_t>
    asSlice() const
    {
        return outpost::Slice<uint8_t>::unsafe(&(*mPtr)[mOffset], mLength);
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
 * \brief Nested derivative of SharedBufferPointer that also features a SharedBufferPointer to its parent.
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
     * \brief Getter function for whether  the SharedChildPointer is actually a child buffer of some valid parent.
     *
     * \return Returns true if the SharedChildPointer instance's parent is valid.
     */
    virtual inline bool
    isChild()
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

#include "shared_buffer_impl.h"

#endif /* SRC_MU_COMMON_UTILS_SMART_BUFFER_H_ */
