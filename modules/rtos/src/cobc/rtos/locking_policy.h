/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_LOCKING_POLICY_H
#define COBC_RTOS_LOCKING_POLICY_H

#include <cobc/rtos/mutex.h>

namespace cobc
{
namespace rtos
{
namespace locking_policy
{

/**
 * No locking.
 */
class SingleThreaded
{
public:
    class Lock
    {
    public:
        explicit inline
        Lock(const SingleThreaded&)
        {
        }

        inline
        ~Lock()
        {
        }

    private:
        // Disable copy constructor and copy assignment operator
        Lock(const Lock&);

        Lock&
        operator=(const Lock&);
    };
};

/**
 * Create a Mutex per object.
 */
class MutexLock
{
public:
    class Lock
    {
    public:
        explicit inline
        Lock(const MutexLock& parent) :
            mMutex(parent.mMutex)
        {
            mMutex.acquire();
        }

        inline
        ~Lock()
        {
            mMutex.release();
        }

    private:
        // Disable copy constructor and copy assignment operator
        Lock(const Lock&);

        Lock&
        operator=(const Lock&);

        Mutex& mMutex;
    };

private:
    friend class Lock;

    mutable Mutex mMutex;
};

/**
 * Use a Mutex shared between multiple objects.
 */
template <Mutex& mutex>
class SharedMutexLock
{
public:
    class Lock
    {
    public:
        explicit inline
        Lock(const SharedMutexLock&)
        {
            mutex.acquire();
        }

        inline
        ~Lock()
        {
            mutex.release();
        }

    private:
        // Disable copy constructor and copy assignment operator
        Lock(const Lock&);

        Lock&
        operator=(const Lock&);
    };

private:
    friend class Lock;
};

}
}
}

#endif
