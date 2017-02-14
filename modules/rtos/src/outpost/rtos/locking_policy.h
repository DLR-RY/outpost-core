/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_RTOS_LOCKING_POLICY_H
#define COBC_RTOS_LOCKING_POLICY_H

#include <outpost/rtos/mutex.h>

namespace outpost
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
