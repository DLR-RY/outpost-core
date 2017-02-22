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
 * - 2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_RTOS_LOCKING_POLICY_H
#define OUTPOST_RTOS_LOCKING_POLICY_H

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
