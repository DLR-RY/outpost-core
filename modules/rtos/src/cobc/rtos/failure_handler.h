/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_FAILURE_H
#define COBC_RTOS_FAILURE_H

#include <stdint.h>

namespace cobc
{
	namespace rtos
	{
		class FailureCode
		{
		public:
			inline static FailureCode
			resourceAllocationFailed()
			{
				return FailureCode(0x0100);
			}

			inline static FailureCode
			returnFromThread()
			{
				return FailureCode(0x01ff);
			}

			inline static FailureCode
			genericRuntimeError()
			{
				return FailureCode(0x0200);
			}

			inline uint32_t
			getCode() const
			{
				return code;
			}

		protected:
			inline FailureCode(uint32_t code) :
				code(code)
			{
			}

		private:
			const uint32_t code;
		};

		class FailureHandler
		{
		public:
			typedef void (*Handler)(FailureCode code);

			/**
			 * Fatal failure has occurred which makes a continuation of the
			 * current execution path impossible.
			 *
			 * If compiled using RTEMS this function will execute the RTEMS
			 * Fatal Error Manager.
			 *
			 * @warning This function WILL NOT RETURN!
			 *
			 * @param code
			 * 		Failure code to identify the failure.
			 */
			static void
			fatal(FailureCode code);

			/**
			 * Set a new failure handler.
			 *
			 * @warning This function must not return!
			 *
			 * @param newHandler
			 */
			static void
			setFailureHandlerFunction(Handler newHandler);

		protected:
			static Handler handler;
		};
	}
}

#endif // COBC_RTOS_FAILURE_H
