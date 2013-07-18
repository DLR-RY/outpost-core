#ifndef COBC__TRAITS_H
#define COBC__TRAITS_H

#include <stdint.h>

namespace cobc
{
	/**
	 * Arithmetic Traits.
	 * 
	 * Traits give types more information e.g. the number of bytes needed
	 * to represent the given type.
	 * 
	 * @author	Fabian Greif <fabian.greif@dlr.de>
	 */
	template<typename T>
	struct Traits
	{
	};
	
	template<>
	struct Traits<uint8_t>
	{
		typedef int8_t SignedType;
		typedef uint8_t UnsignedType;
	
		static const int width = 1;
	};
	
	template<>
	struct Traits<int8_t>
	{
		typedef int8_t SignedType;
		typedef uint8_t UnsignedType;
	
		static const int width = 1;
	};
	
	template<>
	struct Traits<uint16_t>
	{
		typedef int16_t SignedType;
		typedef uint16_t UnsignedType;
	
		static const int width = 2;
	};
	
	template<>
	struct Traits<int16_t>
	{
		typedef int16_t SignedType;
		typedef uint16_t UnsignedType;
	
		static const int width = 2;
	};
	
	template<>
	struct Traits<uint32_t>
	{
		typedef int32_t SignedType;
		typedef uint32_t UnsignedType;
	
		static const int width = 4;
	};
	
	template<>
	struct Traits<int32_t>
	{
		typedef int32_t SignedType;
		typedef uint32_t UnsignedType;
	
		static const int width = 4;
	};
	
	template<>
	struct Traits<uint64_t>
	{
		typedef int64_t SignedType;
		typedef uint64_t UnsignedType;
	
		static const int width = 8;
	};
	
	template<>
	struct Traits<int64_t>
	{
		typedef int64_t SignedType;
		typedef uint64_t UnsignedType;
	
		static const int width = 8;
	};
}

#endif // COBC__TRAITS_H
