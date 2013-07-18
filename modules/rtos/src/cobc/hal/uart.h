/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_HAL_UART_H
#define COBC_HAL_UART_H

#include <cstddef>
#include <stdint.h>

namespace cobc
{
	namespace hal
	{
		/**
		 * UART Interface
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class Uart
		{
		public:
			virtual
			~Uart() = 0;

			/**
			 * Send a single byte.
			 */
			virtual void
			write(uint8_t data) = 0;

			/**
			 * Write a block of bytes.
			 *
			 * @param	*buffer
			 * 		Pointer to a buffer
			 * @param	n
			 * 		Number of bytes to be read
			 */
			virtual void
			write(const uint8_t *buffer, std::size_t n) = 0;

			/**
			 * Flush send buffer.
			 *
			 * Sends eventually buffered data. Afterwards all internal buffers
			 * are empty.
			 */
			virtual void
			flush() = 0;

			/**
			 * Read a single byte
			 */
			virtual bool
			read(uint8_t& c) = 0;

			/**
			 * Read a block of bytes.
			 *
			 * @param	*buffer
			 * 		Pointer to a buffer big enough to storage `n` bytes
			 * @param	n
			 * 		Number of bytes to be read
			 *
			 * @return	Number of bytes which could be read, maximal `n`
			 */
			virtual std::size_t
			read(uint8_t *buffer, std::size_t n) = 0;
		};
	}
}

#endif // COBC_HAL_UART_H
