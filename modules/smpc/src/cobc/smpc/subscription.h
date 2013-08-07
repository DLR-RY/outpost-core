
#ifndef COBC_SMPC__SUBSCRIPTION_H
#define COBC_SMPC__SUBSCRIPTION_H

#include "list.h"
#include "subscriber.h"
#include "topic.h"

namespace cobc
{
	namespace smpc
	{
		/**
		 * Subscription to a topic.
		 *
		 * Every component that wants to receive data from a topic needs to
		 * Instantiate one or more objects of this class. Each instance binds
		 * a topic to a member function of the subscribing class. Data send to
		 * the topic will be forwarded to the given member function.
		 *
		 * To avoid the overhead of generating a copy of this class for every
		 * type of topic and subscriber this class works internally with void
		 * pointers.
		 *
		 * @ingroup	smpc
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class Subscription : public List<Subscription>
		{
			friend class TopicBase;
			friend class SubscriptionRaw;
			friend class List<Subscription>;

		public:
			/**
			 * Constructor.
			 *
			 * Binds a subscriber to a topic.
			 *
			 * @param[in]	topic
			 * 		Topic to subscribe to
			 * @param[in]	subscriber
			 * 		Subscribing class. Must be a subclass of cobc::com::Subscriber.
			 * @param[in]	function
			 * 		Member function pointer of the subscribing class.
			 */
			template <typename T, typename S>
			Subscription(Topic<T>& topic,
						 S *subscriber,
						 void (S::*function)(typename Topic<T>::Type * message));

			/**
			 * Destroy the subscription
			 *
			 * @warning
			 * 	The destruction and creation of subscriptions during the normal
			 * 	runtime is not thread-safe. If topics need to be
			 * 	destroyed outside the initialization of the application
			 * 	it is necessary to hold all other threads which
			 * 	might also create or destroy topics and/or subscriptions.
			 */
			~Subscription();

			/**
			 * Connect all subscriptions to it's assigned topic.
			 *
			 * Has to be called at program startup to initialize the
			 * Publisher<>Subscriber protocol.
			 *
			 * @internal
			 * Builds the internal linked lists.
			 */
			static void
			connectSubscriptionsToTopics();

		protected:
			/**
			 * Base-type to cast all member function pointers to. The correct type
			 * is restored when calling the function. Although it the member
			 * function is later called with a void-pointer everything remains
			 * safe as only member functions with a correct signature can
			 * be bound to a topic.
			 */
			typedef void (Subscriber::*Function)(void *);

			/**
			 * Relay message to the subscribing component.
			 */
			inline void
			notify(void * message) const
			{
				(subscriber->*function)(message);
			}

			/**
			 * Release all subscriptions.
			 *
			 * Counterpart to connectSubscriptionsToTopics(). Use of this
			 * function is not thread-safe. To use halt all threads that
			 * might create or destroy subscriptions or topics.
			 */
			static void
			releaseAllSubscriptions();

		private:
			/// List of all subscriptions currently in the system
			static Subscription * listOfAllSubscriptions;

			/**
			 * Used by Subscription::connectSubscriptionsToTopics to map the
			 * subscriptions to their corresponding topics.
			 */
			TopicBase * const topic;
			Subscription * nextTopicSubscription;

			/// Object and member function to forward a received message to
			Subscriber * const subscriber;
			Function const function;
		};

#ifndef __DOXYGEN__
		class TestingSubscription : public Subscription
		{
		public:
			using Subscription::connectSubscriptionsToTopics;
			using Subscription::releaseAllSubscriptions;
		};
#endif
	}
}

// ----------------------------------------------------------------------------
// Implementation of the template constructor
template <typename T, typename S>
cobc::smpc::Subscription::Subscription(Topic<T>& topic,
		S *subscriber,
		void (S::*function)(typename Topic<T>::Type * message)) :
	List<Subscription>(listOfAllSubscriptions, this),
	topic(&topic),
	nextTopicSubscription(0),
	subscriber(reinterpret_cast<Subscriber *>(subscriber)),
	function(reinterpret_cast<Function>(function))
{
}

#endif // COBC_SMPC__SUBSCRIPTION_H
