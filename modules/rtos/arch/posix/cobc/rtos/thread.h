/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#ifndef COBC_POSIX_THREAD_H
#define COBC_POSIX_THREAD_H

#include <stddef.h>
#include <stdint.h>

#include <string>
#include <pthread.h>

#include <cobc/time/duration.h>

namespace cobc
{
namespace rtos
{

/**
 * Wrapper class for the Thread function of the Operating System.
 *
 * The run()-method of a derived class is invoked in the newly created
 * thread context. The derived class can also hold data members
 * associated with the specific thread.
 *
 * \author    Fabian Greif
 * \ingroup    rtos
 */
class Thread
{
public:
    /// Unique identifier to identify a thread.
    typedef uint32_t Identifier;

    enum FloatingPointSupport
    {
        noFloatingPoint,
        floatingPoint
    };

    /**
     * Initial return value of getIdentifier() before the
     * thread have been started and an associated thread id.
     */
    static const Identifier invalidIdentifier = 0xFFFFFFFF;

    /**
     * Use the default value for the stack size.
     *
     * The default value is depending on the project settings.
     */
    static const size_t defaultStackSize = 0;

    /**
     * Create a new thread.
     *
     * \param priority
     *         Not used for POSIX.
     * \param stack
     *         Not used for POSIX.
     * \param name
     *         Name of the thread. Must not be longer than 16 characters.
     *
     * \see    rtos::FailureHandler::fatal()
     */
    explicit
    Thread(uint8_t priority,
           size_t stack = defaultStackSize,
           const char* name = 0,
           FloatingPointSupport floatingPointSupport = noFloatingPoint);

    /**
     * Destructor.
     *
     * Frees the resources used by this thread to be used by another
     * thread.
     */
    virtual
    ~Thread();

    /**
     * Start the execution of the thread.
     *
     * This may preempt the current thread if the thread to be executed
     * has a higher priority.
     */
    void
    start();

    /**
     * Get a unique identifier for this thread.
     *
     * Only valid after the thread has be started.
     *
     * \return  Unique identifier.
     */
    Identifier
    getIdentifier() const;

    /**
     * Get the unique identifier for the currently executed thread.
     *
     * \return  Unique identifier.
     */
    static Identifier
    getCurrentThreadIdentifier();

    /**
     * Unused for POSIX.
     */
    void
    setPriority(uint8_t priority);

    /**
     * Unused for POSIX.
     */
    uint8_t
    getPriority() const;

    /**
     * Give up the processor but remain in ready state.
     */
    static void
    yield();

    /**
     * Suspend the current thread for the given time.
     *
     * \param timeout
     *         Time to sleep in milliseconds.
     */
    static void
    sleep(::cobc::time::Duration timeout);

protected:
    /**
     * Working method of the thread.
     *
     * This method is called after the thread is started. It may never
     * return (endless loop). On a return the fatal error
     * handler will be called.
     */
    virtual void
    run() = 0;

private:
    static void*
    wrapper(void* object);

    bool mIsRunning;
    pthread_t mPthreadId;
    Identifier mTid;
    std::string mName;
};

}
}

#endif // COBC_POSIX_THREAD_H
