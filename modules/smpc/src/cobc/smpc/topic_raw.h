/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_SMPC_TOPIC_RAW_H
#define COBC_SMPC_TOPIC_RAW_H

#include <cstddef>
#include <stdint.h>
#include <cobc/rtos/mutex.h>

#include "list.h"

namespace cobc
{
	namespace smpc
	{
		// forward declaration
		class SubscriptionRaw;

		/**
		 * Non type-safe %Topic.
		 *
		 * Data can be published under a predefined topic. Any number of
		 * subscribers can connect to that topic to get notified if new
		 * data is published.
		 *
		 * There is also a type-safe version of this Publisher<>%Subscriber
		 * protocol called cobc::smpc::Topic available. Use that version if
		 * you want to exchange data with a known length. If in doubt use the
		 * type-safe version!
		 *
		 * @see		cobc::smpc::Topic
		 * @ingroup	smpc
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class TopicRaw : protected List<TopicRaw>
		{
			// Needed to allow SubscriptionRaw() to append itself to the
			// subscription list
			friend class SubscriptionRaw;
			friend class List<TopicRaw>;

		public:
			/**
			 * Create a new raw topic.
			 */
			TopicRaw();

			/**
			 * Destroy the topic.
			 *
			 * @warning
			 * 		The destruction and creation of topics during the normal
			 * 		runtime is not thread-safe. If topics need to be
			 * 		destroyed outside the initialization of the application
			 * 		it is necessary to hold all other threads which
			 * 		might also create or destroy topics and/or subscriptions.
			 */
			~TopicRaw();

			/**
			 * Publish new data.
			 *
			 * Forwards the pointer to all connected subscribers.
			 */
			void
			publish(const void * message, size_t length);

		protected:
			/// List of all raw topics currently active.
			static TopicRaw* listOfAllTopics;

			/// Mutex used to protect the publish() method.
			rtos::Mutex mutex;

			/// Pointer to the list of subscriptions
			SubscriptionRaw* subscriptions;

		private:
			static void
			clearSubscriptions();
		};
	}
}

#endif // COBC_SMPC_TOPIC_RAW_H
