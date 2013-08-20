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
		 * @author  Fabian Greif <fabian.greif@dlr.de>
		 * @ingroup	rtos
		 */
		class Thread
		{
		public:
			/**
			 * Create a new thread.
			 *
			 * During the creation of new threads the
			 *
			 * @param priority
			 * 	   Thread priority. FreeRTOS supports priorities between
			 *     0 and (configMAX_PRIORITIES - 1).
			 *     Lower values represent a lower priority, 0 is the
			 *     priority of the idle task and the overall lowest priority.
			 *
			 * @param stack
			 *     Stack size in **bytes**. If the stack is smaller than the
			 *     default stack size it is replaced with the default size.
			 *     See the definition of `minimumStackSize` in `thread.cpp`.
			 *
			 * @param name
			 *     Name of the thread. Length is limited to
			 *     `configMAX_TASK_NAME_LEN` characters.
			 *
			 * @see	rtos::FailureHandler::fatal()
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
			 * Set a new priority for the thread.
			 *
			 * The calling thread may be preempted if it lowers its own
			 * priority or raises another thread's priority.
			 *
			 * @param priority
			 *     Thread priority. FreeRTOS supports priorities between
			 *     0 and (configMAX_PRIORITIES - 1).
			 *     Lower values represent a lower priority, 0 is the
			 *     priority of the idle task and the overall lowest priority.
			 */
			void
			setPriority(uint8_t priority);

			/**
			 * Read the priority.
			 *
			 * @return	Priority of the thread
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
			 * @param timeout
			 *     Time to sleep.
			 */
			static void
			sleep(::cobc::time::Duration duration);

			/**
			 * Start the FreeRTOS scheduler.
			 *
			 * @warning This function will not return.
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

			void * handle;	// FreeRTOS thread handle (xTaskHandle)

			const uint8_t priority;
			std::size_t stackSize;
			const char * const name;
		};
	}
}

#endif // COBC_RTOS_FREERTOS_THREAD_H
