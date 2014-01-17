/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_POSIX_SEMAPHORE_HPP
#define COBC_RTOS_POSIX_SEMAPHORE_HPP

#include <stdint.h>

#include <semaphore.h>
#include <pthread.h>

#include <cobc/time/duration.h>

namespace cobc
{
	namespace rtos
	{
		/**
		 * Counting Semaphore.
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class Semaphore
		{
		public:
			/**
			 * Create a Semaphore.
			 *
			 * @param count
			 * 		Initial value for the semaphore.
			 */
			explicit Semaphore(uint32_t count);

			/**
			 * Destroy the semaphore and release it's resources.
			 */
			~Semaphore();

			/**
			 * Decrement the count.
			 *
			 * Blocks if the count is currently zero until it is incremented
			 * by another thread calling the release() method.
			 */
			inline bool
			acquire()
			{
				return (sem_wait(&sid) == 0);
			}

			/**
			 * Decrement the count.
			 *
			 * Same a acquire() but abort after `timeout` milliseconds.
			 *
			 * @param timeout
			 * 		Timeout in milliseconds.
			 *
			 * @return	`true` if the semaphore could be successfully acquired,
			 * 			`false` in case of an error or timeout.
			 */
			bool
			acquire(time::Duration timeout);

			/**
			 * Increment the count.
			 *
			 * This function will never block, but may preempt if an other
			 * thread waiting for this semaphore has a higher priority.
			 */
			inline void
			release()
			{
				sem_post(&sid);
			}

		private:
			// disable copy constructor
			Semaphore(const Semaphore& other);

			// disable assignment operator
			Semaphore&
			operator = (const Semaphore& other);

			/// POSIX semaphore handle
			sem_t sid;
		};

		/**
		 * Binary semaphore.
		 *
		 * Restricts the value of the semaphore to 0 and 1.
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class BinarySemaphore
		{
		public:
			enum State
			{
				acquired,
				released
			};

			/**
			 * Create a binary semaphore.
			 *
			 * @param	initial
			 * 		Initial value of the semaphore.
			 */
			explicit BinarySemaphore(State initial);

			/**
			 * Destroy the semaphore and release it's resources.
			 */
			~BinarySemaphore();

			/**
			 * Decrement the count.
			 *
			 * Blocks if the count is currently zero until it is incremented
			 * by another thread calling the release() method.
			 */
			bool
			acquire();

			/**
			 * Decrement the count.
			 *
			 * Same a acquire() but abort after `timeout` milliseconds.
			 *
			 * @param timeout
			 * 		Timeout in milliseconds.
			 *
			 * @return	`true` if the semaphore could be successfully acquired,
			 * 			`false` in case of an error or timeout.
			 */
			bool
			acquire(time::Duration timeout);

			/**
			 * Increment the count.
			 *
			 * This function will never block, but may preempt if an other
			 * thread waiting for this semaphore has a higher priority.
			 */
			void
			release();

		private:
			// disable copy constructor
			BinarySemaphore(const BinarySemaphore& other);

			// disable assignment operator
			BinarySemaphore&
			operator = (const BinarySemaphore& other);

			// POSIX handles
			pthread_mutex_t mutex;
			pthread_cond_t signal;
			State value;
		};
	}
}

#endif // COBC_RTOS_POSIX_SEMAPHORE_HPP
