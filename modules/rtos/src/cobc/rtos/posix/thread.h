/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_POSIX_THREAD_H
#define COBC_POSIX_THREAD_H

#include <cstddef>
#include <stdint.h>

#include <pthread.h>

// for nanosleep()
#include <time.h>

#include <cobc/time/duration.h>

namespace cobc
{
	namespace rtos
	{
		/**
		 * Wrapper class for the Thread function of the Operating System.
		 *
		 * Encapsulates the RTEMS C thread functions and provides C++-style
		 * access to them.
		 *
		 * The run()-method of a derived class is invoked in the newly created
		 * thread context. The derived class can also hold data members
		 * associated with the specific thread.
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 * @ingroup	rtos
		 */
		class Thread
		{
		public:
			/**
			 * Create a new thread.
			 *
			 * @param priority
			 * 		Not used for POSIX.
			 * @param stack
			 * 		Not used for POSIX.
			 * @param name
			 * 		Not used for POSIX.
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
			static inline void
			yield()
			{
				// On Linux, this call always succeeds
				pthread_yield();
			}

			/**
			 * Suspend the current thread for the given time.
			 *
			 * @param timeout
			 * 		Time to sleep in milliseconds.
			 */
			static inline void
			sleep(::cobc::time::Duration timeout)
			{
				uint64_t nanoseconds = timeout.microseconds() * 1000;

				timespec req = {
					static_cast<time_t>(nanoseconds / 1000000000),	// seconds
					static_cast<long int>(nanoseconds % 1000000000)	// nanoseconds
				};
				timespec rem;

				nanosleep(&req, &rem);
			}

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
			static void *
			wrapper(void * object);

			bool isRunning;
			pthread_t tid;
		};
	}
}

#endif // COBC_POSIX_THREAD_H
