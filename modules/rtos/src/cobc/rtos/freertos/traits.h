/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_RTOS_FREERTOS_TRAITS_H
#define COBC_RTOS_FREERTOS_TRAITS_H

#include <stdint.h>

namespace cobc
{
	namespace rtos
	{
		/**
		 * Access the signed and unsigned types of the different types used
		 * in FreeRTOS.
		 *
		 * @author  Fabian Greif
		 */
		template<typename T>
		struct Traits
		{
		};

		template<>
		struct Traits<int16_t>
		{
			typedef int16_t  SignedType;
			typedef uint16_t UnsignedType;
		};

		template<>
		struct Traits<uint16_t>
		{
			typedef int16_t  SignedType;
			typedef uint16_t UnsignedType;
		};

		template<>
		struct Traits<int32_t>
		{
			typedef int32_t  SignedType;
			typedef uint32_t UnsignedType;
		};

		template<>
		struct Traits<uint32_t>
		{
			typedef int32_t  SignedType;
			typedef uint32_t UnsignedType;
		};
		template<>
		struct Traits<int64_t>
		{
			typedef int64_t  SignedType;
			typedef uint64_t UnsignedType;
		};

		template<>
		struct Traits<uint64_t>
		{
			typedef int64_t  SignedType;
			typedef uint64_t UnsignedType;
		};
	}
}

#endif // COBC_RTOS_FREERTOS_TRAITS_H
