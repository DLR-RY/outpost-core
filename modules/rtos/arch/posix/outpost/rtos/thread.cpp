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
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#include "thread.h"

#include <iostream>

// for the access to gettid
#include <sys/syscall.h>
#include <unistd.h>

// for nanosleep()
#include <time.h>

#include <outpost/rtos/failure_handler.h>

#include "internal/time.h"

// ----------------------------------------------------------------------------
void*
outpost::rtos::Thread::wrapper(void* object)
{
    Thread* thread = reinterpret_cast<Thread*>(object);

    thread->mTid = Thread::getCurrentThreadIdentifier();
    thread->run();

    // Returning from a thread is a fatal error, nothing more to
    // do here than call the fatal error handler.
    rtos::FailureHandler::fatal(rtos::FailureCode::returnFromThread());

    pthread_exit(0);
    return NULL;
}

// ----------------------------------------------------------------------------
outpost::rtos::Thread::Thread(uint8_t,
                           size_t,
                           const char* name,
                           FloatingPointSupport /*floatingPointSupport*/) :
    mIsRunning(false),
    mPthreadId(),
    mTid(),
    mName()
{
    if (name != 0)
    {
        mName = std::string(name);
    }
}

outpost::rtos::Thread::~Thread()
{
    if (mIsRunning)
    {
        pthread_cancel(mPthreadId);
        pthread_join(mPthreadId, NULL);
    }
}

// ----------------------------------------------------------------------------
outpost::rtos::Thread::Identifier
outpost::rtos::Thread::getIdentifier() const
{
    return 0;
}

outpost::rtos::Thread::Identifier
outpost::rtos::Thread::getCurrentThreadIdentifier()
{
#ifdef SYS_gettid
    pid_t tid = syscall(SYS_gettid);
    return tid;
#else
#   error "SYS_gettid unavailable on this system"
#endif
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::start()
{
    mIsRunning = true;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int ret = pthread_create(&mPthreadId, &attr, &Thread::wrapper, reinterpret_cast<void *>(this));
    if (ret != 0)
    {
        FailureHandler::fatal(FailureCode::resourceAllocationFailed(Resource::thread));
    }

    if (!mName.empty())
    {
        int result = pthread_setname_np(mPthreadId, mName.c_str());
        if (result != 0)
        {
            std::cerr << "Failed to set thread name: '" << mName << "': " << result << std::endl;
        }
    }

    pthread_attr_destroy(&attr);
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::setPriority(uint8_t priority)
{
    // No priority defined for POSIX!
    (void) priority;
}

uint8_t
outpost::rtos::Thread::getPriority() const
{
    return 0;
}

// ----------------------------------------------------------------------------
void
outpost::rtos::Thread::yield()
{
    // On Linux, this call always succeeds
    pthread_yield();
}

void
outpost::rtos::Thread::sleep(::outpost::time::Duration timeout)
{
    const timespec req = toRelativeTime(timeout);
    timespec rem;

    nanosleep(&req, &rem);
}
