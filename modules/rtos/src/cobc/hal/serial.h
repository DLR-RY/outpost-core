/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_HAL_SERIAL_H
#define COBC_HAL_SERIAL_H

#include <cstddef>
#include <stdint.h>

#include <cobc/time/duration.h>

namespace cobc
{
	namespace hal
	{
		/**
		 * Serial Interface
		 *
		 * @author	Fabian Greif <fabian.greif@dlr.de>
		 */
		class Serial
		{
		public:
			/* Added by Muhammad FIXME: If this is the right position?? */
			typedef enum states_e{
				OPEN = 0,
				CLOSE,
				ENABLED,
				DISABLED,
				READY,
				BUSY
			}serial_states_t;

			typedef enum parity_e{
				NONE = 0,
				EVEN,
				ODD,
				MARK,
				SPACE
			}serial_parity_t;

			virtual
			~Serial() = 0;

			/**
			 * Opens the UART device
			 */
			virtual void 
			open() = 0;

			/**
			 * CLose the UART device
			 */
			virtual void 
			close() = 0;

			/**
			 * Check if data is available in the input buffers.
			 *
			 * @retval true   Data is available and can be read via read(...).
			 * @retval false  No data available.
			 */
			virtual bool
			isAvailable(void) = 0;

			/**
			 * Read a block of bytes.
			 *
			 * Waits until the timeout occurs to read the given number of
			 * bytes. May return earlier if enough bytes are available. The
			 * exact number of bytes read will be return, it will be up to
			 * `length` byte but can also be any value lower.
			 *
			 * @param	*data
			 * 		Pointer to a buffer big enough to storage `length` bytes
			 * @param	length
			 * 		Number of bytes to be read
			 *
			 * @return	Number of bytes which could be read, maximal `length`
			 */
			virtual std::size_t
			read(uint8_t* data, std::size_t length, time::Duration timeout = time::Duration::infinity()) = 0;

			/**
			 * Write a block of bytes with timeout.
			 *
			 *
			 *
			 * @param	*data
			 * 		Pointer to a buffer
			 * @param	length
			 * 		Number of bytes to be write
			 * @return  Number of bytes written.
			 */
			virtual std::size_t
			write(const uint8_t* data, std::size_t length, time::Duration timeout = time::Duration::infinity()) = 0;

			/**
			 * Flush send/receive buffers.
			 *
			 * Sends eventually buffered data. Afterwards all internal buffers
			 * are empty.
			 */
			virtual void
			flush() = 0;
		};
	}
}

#endif // COBC_HAL_SERIAL_H
