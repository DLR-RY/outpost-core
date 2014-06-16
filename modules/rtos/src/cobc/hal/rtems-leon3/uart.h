/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_LEON3_UART_H_
#define COBC_LEON3_UART_H_

#include <stdint.h>
#include <cstddef>

#include <cobc/rtos/semaphore.h>
#include <cobc/utils.h>

#include "../serial.h"

extern "C" {
#include <apbuart.h>
}


namespace cobc
{
	namespace leon3
	{
		static const char * uartPath[] = {
		"/dev/apbuart0", "/dev/apbuart1", "/dev/apbuart2", "/dev/apbuart3", "/dev/apbuart4", "/dev/apbuart5"
		};

		/*! \class UART
		 * \brief GR712RC UART driver interface
		 * \see		cobc::hal::Serial
		 * \author	Muhammad Bassam <muhammad.bassam@dlr.de>
		 *
		 * This uses the APBUART driver register by Driver manager to access
		 * through the system level IO calls.
		 */
		class UART : public hal::Serial
		{
			friend class DevPrinter;
		public:
			/* Device states */
			enum States{
				OPEN = 0,
				CLOSE,
				ENABLED,
				DISABLED,
				READY,
				BUSY
			};

			/**
			 * Create the object to access the device specified by
			 * path which can be found in the AMBA plug&play map.
			 *
			 * \param id
			 * 		ID if the device corresponding to its path on AMBA PnP
			 * \param baud
			 * 		Baud rate of the device
			 * \param blocking
			 * 		Blocking option for the device (APBUART_BLK_RX, APBUART_BLK_TX, APBUART_BLK_FLUSH)
			 */
			UART(uint8_t id, uint32_t baud, uint8_t blocking);

            /**
             * \brief Virtual UART interface destructor
             *
             * Destroys the object and free resources.
             */
			virtual
            ~UART();

			/**
			 * Opens the UART device
			 */
            virtual
            void open();

            /**
			 * Opens the UART device
			 */
            virtual
            void close();

            /**
             * \brief Virtual method to check the device availability
             *
             * Private status field is updated during every device
             * operation stage. The availability can be expressed as a
             * READY state of the device
             *
             * \return True if available, false if not available
             */
            virtual
            bool isAvailable(){
				return ((devState == OPEN || devState == ENABLED) ? true : false);
			}

			/*!
			 * Read is only successful if device is opened and in READY
			 * state. Otherwise the request will discarded. THe data bytes
			 * are being read in according to the length passed.
			 *
			 * \param *data
			 * 		Pointer to a buffer big enough to storage `length` bytes
			 * \param length
			 * 		Number of bytes to be read
			 * \param timeout
			 * 		Timeout if the read is blocked and no data has been read within this time
			 *
			 * \return	Number of bytes which could be read, maximal `length`
			 */
			virtual std::size_t
			read(uint8_t* data, std::size_t length, time::Duration timeout = time::Duration::infinity());

			/**
			 * Write a block of bytes within a give duration
			 *
			 * \param *data
			 * 		Pointer to a buffer big enough to storage `length` bytes
			 * \param length
			 * 		Number of bytes to be written
			 * \param timeout
			 * 		Timeout if the write is blocked and not enough data bytes has been written within this time
			 *
			 * \return	Number of bytes which could be read, maximal `length`
			 */
			virtual std::size_t
			write(const uint8_t* data, std::size_t length, time::Duration timeout = time::Duration::infinity());

			/**
			 * Sends eventually buffered data. Afterwards all internal buffers
			 * are empty.
			 * TODO: Complete the implementation
			 */
			virtual
			void flush();

        private:
            // Private methods intended to be accessed only within object
            int dev_enable();
            int dev_disable();

            int devHandle;              /** Handle to the device */
			uint8_t devId;				/** Device name */
			uint32_t baud;              /** Connected device baud rate */
			enum States devState;	  	/** If the device is enabled */
			unsigned int txCount;      	/** Total transmitted data count of interface */
			unsigned int rxCount;      	/** Total received data count of interface */
			uint8_t blockingMode;		/** Device blocking mode */
			apbuart_stats devStatus;
        };
    }
}

#endif // COBC_LEON3_UART_H_
