/**
 * \file
 * \author Muhammad Bassam
 *
 * \copyright DLR - German Aerospace Center
 *
 * \brief RTEMS UART Interface for GR712RC LEON3-FT processor
 *
 * Following file presents the UART driver of the LEON3-FT processor for
 * GR712RC development board. At the moment is created outside the libcobc scope
 * to verify its operability and eventually it will be shifted under libcobc/modules/rtos/src/cobc/hal/rtems-leon3
 *
 * Please note to provide better encapsulation according to the standard set Serial class object is intended to be
 * used for access to the UART driver
 *
 * TODO: Usage structure.
 *  Serial.h
 *      --> Uart.h
 *          -->apbuart.c
 */

#ifndef UART_H_
#define UART_H_

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
        /**
         * \brief GR712RC UART driver interface
         * \see        cobc::hal::Serial
         * \author    Muhammad Bassam
         *
         * This uses the APBUART driver register by Driver manager to access
         * through the system level IO calls.
         */
        class UART : public hal::Serial{
        public:

            /**
             * \brief UART interface constructor
             *
             * Create the object to access the device specified by
             * path which can be found in the AMBA plug&play map.
             *
             * \param path
             *         Path of the device mapped from AMBA PnP
             * \param baud
             *         Baud rate of the device
             * \param blocking
             *         Blocking option for the device (APBUART_BLK_RX, APBUART_BLK_TX, APBUART_BLK_FLUSH)
             * \param par
             *         Parity settings of the device
             */
            UART(char * path, uint32_t baud, uint8_t blocking, serial_parity_t par);
            //UART(char * path, uint32_t baud, uint8_t blocking);

            /**
             * \brief Virtual UART interface destructor
             *
             * Destroys the object and free resources.
             */
            ~UART();

            virtual void open();
            virtual void close();

            /**
             * \brief Virtual method to check the device availability
             *
             * Private status field is updated during every device
             * operation stage. The availability can be expressed as a
             * READY state of the device
             *
             * \return True if available, false if not available
             */
            virtual bool isAvailable();

            /**
             * \brief Read a block of bytes within a give duration
             *
             * Read is only successful if device is opened and in READY
             * state. Otherwise the request will discarded. THe data bytes
             * are being read in according to the length passed.
             *
             * \param *data
             *         Pointer to a buffer big enough to storage \p length bytes
             * \param length
             *         Number of bytes to be read
             * \param timeout
             *         Timeout if the read is blocked and no data has been read within this time
             *
             * \return    Number of bytes which could be read, maximal \p length
             */
            virtual std::size_t read(uint8_t* data, std::size_t length, time::Duration timeout = time::Duration::infinity());

            /**
             * \brief Write a block of bytes within a give duration
             *
             * \param *data
             *         Pointer to a buffer big enough to storage \p length bytes
             * \param length
             *         Number of bytes to be written
             * \param timeout
             *         Timeout if the write is blocked and not enough data bytes has been written within this time
             *
             * \return    Number of bytes which could be read, maximal \p length
             */
            virtual std::size_t write(const uint8_t* data, std::size_t length, time::Duration timeout = time::Duration::infinity());

            /**
             * \brief Flush send/receive buffers.
             *
             * Sends eventually buffered data. Afterwards all internal buffers
             * are empty.
             * TODO: Complete the implementation
             */
            virtual void flush();

        private:
            // Private methods intended to be accessed only within object 
            int dev_enable();
            int dev_disable();
            void print_status();

            int devHandle;              /** Handle to the device */
            char devName[20];            /** Device name */
            uint32_t baud;              /** Connected device baud rate */
            serial_parity_t parity;     /** Device parity */
            serial_states_t devState;      /** If the device is enabled */
            unsigned int txCount;          /** Total transmitted data count of interface */
            unsigned int rxCount;          /** Total received data count of interface */
            uint8_t blockingMode;
            apbuart_stats devStatus;
        };
    }
}

#endif // UART_H_ 
