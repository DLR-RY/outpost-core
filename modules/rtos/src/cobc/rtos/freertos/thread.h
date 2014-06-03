/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_FREERTOS_THREAD_H
#define COBC_RTOS_FREERTOS_THREAD_H

#include <cstddef>
#include <stdint.h>

#include <cobc/time/duration.h>

namespace cobc
{
    namespace rtos
    {
        /**
         * Wrapper class for the Thread function of the Operating System.
         *
         * Encapsulates the FreeRTOS C thread functions and provides C++-style
         * access to them.
         *
         * The run()-method of a derived class is invoked in the newly created
         * thread context. The derived class can also hold data members
         * associated with the specific thread.
         *
         * TODO FreeRTOS memory handling.
         *
         * If more threads than available are created the
         * FailureHandler::fatal() handler is called.
         *
         * \author  Fabian Greif
         * \ingroup    rtos
         */
        class Thread
        {
        public:
            /// Unique identifier to identify a thread.
            typedef uint32_t Identifier;

            /**
             * Initial return value of getIdentifier() before the
             * thread have been started and an associated thread id.
             */
            static const Identifier invalidIdentifier = 0xffffffff;

            /**
             * Create a new thread.
             *
             * During the creation of new threads the
             *
             * \param priority
             *        Thread priority. A lower number represent a lower priority.
             *        see Thread::setPriority(uint8_t).
             *
             * \param stack
             *     Stack size in **bytes**. If the stack is smaller than the
             *     default stack size it is replaced with the default size.
             *     See the definition of \c minimumStackSize in \c thread.cpp.
             *
             * \param name
             *     Name of the thread. Length is limited to
             *     \c configMAX_TASK_NAME_LEN characters.
             *
             * \see    rtos::FailureHandler::fatal()
             */
            Thread(uint8_t priority,
                    size_t stack = 0,
                    const char * name = 0);

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
             * Set a new priority for the thread.
             *
             * The calling thread may be preempted if it lowers its own
             * priority or raises another thread's priority.
             *
             * FreeRTOS supports priorities between 0 and
             * (configMAX_PRIORITIES - 1). that may not cleanly map to the
             * 0..255 priorities defined here. Set configMAX_PRIORITIES to 256
             * to have a direct mapping. In any other case it is tried to
             * center the available priorities around 128.
             *
             * E.g. with configMAX_PRIORITIES = 16 the following priority steps
             * are available (stepwidth = 16, offset = 8):
             *
             *   8,24,40,56,72,88,104,120,136,152,168,184,200,216,232,248
             *
             * From the FreeRTOS manual:
             *
             *     The number of priorities available to the application tasks.
             *     Any number of tasks can share the same priority.
             *
             *     Each available priority consumes RAM within the RTOS kernel
             *     so this value should not be set any higher than actually
             *     required by your application.
             *
             * \param priority
             *     Thread priority. Lower values represent a lower priority,
             *     0 is the overall lowest priority.
             */
            void
            setPriority(uint8_t priority);

            /**
             * Read the priority.
             *
             * \return    Priority of the thread
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
             *     Time to sleep.
             */
            static void
            sleep(::cobc::time::Duration duration);

            /**
             * Start the FreeRTOS scheduler.
             *
             * \warning This function will not return.
             */
            static void
            startScheduler();

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
            // disable copy constructor
            Thread(const Thread& other);

            // disable assignment operator
            Thread&
            operator = (const Thread& other);

            static void
            wrapper(void *object);

            void * handle;    // FreeRTOS thread handle (xTaskHandle)

            const uint8_t priority;
            size_t stackSize;
            const char * const name;
        };
    }
}

#endif // COBC_RTOS_FREERTOS_THREAD_H
