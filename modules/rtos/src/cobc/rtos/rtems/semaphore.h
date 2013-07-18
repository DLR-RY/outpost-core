/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_RTEMS_SEMAPHORE_HPP
#define COBC_RTOS_RTEMS_SEMAPHORE_HPP

#include <rtems.h>
#include <cobc/time/duration.h>

#include <cobc/time/duration.h>

namespace cobc
{
	namespace rtos
	{
		/**
		 * Counting semaphores.
		 *
		 * A counting semaphore can be used to control access to a pool of two
		 * or more resources. For example, access to three printers could be
		 * administered by a semaphore created with an initial count of three.
		 *
		 * Thread synchronization may be achieved by creating a semaphore with an
		 * initial count of zero. One thread waits for the arrival of another
		 * thread by issuing a acquire() directive when it reaches
		 * a synchronization point. The other thread performs a corresponding
		 * release() operation when it reaches its synchronization point, thus
		 * unblocking the pending thread.
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 * @ingroup	rtos
		 */
		class Semaphore
		{
		public:
			/**
			 * Create a Semaphore.
			 *
			 * RTEMS has only one pool for mutexes and semaphores. Make sure
			 * the maximum number of allocated mutexes and semaphores combined
			 * is smaller than the number given via `CONFIGURE_MAXIMUM_SEMAPHORES`.
			 *
			 * @param count
			 * 		Initial value for the semaphore.
			 */
			explicit Semaphore(uint32_t count);

			/**
			 * Destroy the semaphore and release it's resources.
			 *
			 * Frees the resources for another semaphore or mutex to be
			 * created.
			 */
			~Semaphore();

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
			 * 		Timeout in milliseconds resolution.
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
				rtems_semaphore_release(id);
			}

		private:
			// disable copy constructor
			Semaphore(const Semaphore& other);

			// disable assignment operator
			Semaphore&
			operator = (const Semaphore& other);

			rtems_id id;
		};

		/**
		 * Binary semaphores.
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 * @ingroup	rtos
		 */
		class BinarySemaphore
		{
		public:
			/**
			 * Create a Semaphore.
			 *
			 * RTEMS has only one pool for mutexes and semaphores. Make sure
			 * the maximum number of allocated mutexes and semaphores combined
			 * is smaller than the number given via `CONFIGURE_MAXIMUM_SEMAPHORES`.
			 *
			 * @param initial
			 * 		Initial value for the semaphore.
			 */
			explicit BinarySemaphore(bool initial);

			/**
			 * Destroy the semaphore and release it's resources.
			 *
			 * Frees the resources for another semaphore or mutex to be
			 * created.
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
			 * 		Timeout in milliseconds resolution.
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
				rtems_semaphore_release(id);
			}

		private:
			// disable copy constructor
			BinarySemaphore(const BinarySemaphore& other);

			// disable assignment operator
			BinarySemaphore&
			operator = (const BinarySemaphore& other);

			rtems_id id;
		};
	}
}

#endif // COBC_RTOS_RTEMS_SEMAPHORE_HPP
