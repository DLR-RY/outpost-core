/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_POSIX_MUTEX_HPP
#define COBC_RTOS_POSIX_MUTEX_HPP

#include <pthread.h>

namespace cobc
{
	namespace rtos
	{
		/**
		 * Mutex
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class Mutex
		{
		public:
			inline
			Mutex()
			{
				pthread_mutex_init(&mutex, NULL);
			}

			inline
			~Mutex()
			{
				pthread_mutex_destroy(&mutex);
			}

			/**
			 * Acquire the mutex.
			 *
			 * This function may block if the mutex is currently held by an
			 * other thread.
			 *
			 * @returns	`true` if the mutex could be acquired.
			 */
			inline bool
			acquire()
			{
				return (pthread_mutex_lock(&mutex) == 0);
			}

			/**
			 * Releas the mutex.
			 */
			inline void
			release()
			{
				pthread_mutex_unlock(&mutex);
			}

		private:
			// disable copy constructor
			Mutex(const Mutex& other);

			// disable assignment operator
			Mutex&
			operator = (const Mutex& other);

			pthread_mutex_t mutex;
		};
	}
}

#endif // COBC_RTOS_POSIX_MUTEX_HPP
