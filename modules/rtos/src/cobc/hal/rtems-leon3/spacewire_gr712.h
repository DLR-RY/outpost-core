/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
/**
 * File presents the interface to the GRSPW driver of the LEON3-FT processor for
 * GR712RC development board. This will replace the SpaceWireLight driver interface
 * used for the office model
 */

#ifndef COBC_LEON3_SPACEWIRE_GR712_H_
#define COBC_LEON3_SPACEWIRE_GR712_H_

#include <stdint.h>
#include <cstddef>

#include <cobc/rtos/semaphore.h>
#include <cobc/utils.h>

#include "../spacewire.h"

extern "C"
{
#include "../../../../ext/rtems-leon3/grspw/spw.h"
}

static void
drv_register()
{
    extern void
    grspw_drv_register(void);
    grspw_drv_register();
}

namespace cobc
{
    namespace leon3
    {

        /**
         * SpaceWire Driver Interface for GR712 ASIC
         *
         * Uses the SpaceWire GRSPW driver for RTEMS 4.10 on LEON3 from
         * RTEMS library
         *
         * \see        cobc::hal::Spacewire
         * \author    Muhammad Bassam
         */
        class SpaceWireGR712 : public hal::SpaceWire
        {
            friend class DevPrinter;
        public:
            /**
             * Creates the SpW object with the device found in the
             * driver table and assign the node ID. Receive/Transmit
             * buffers are allocated statically.
             *
             * \param [node]
             * 		Node address of the device
             * \param [devId]
             * 		ID of the device corresponding to its path
             */
            SpaceWireGR712(uint8_t node, uint8_t devId);

            /**
             * Destroy the object and close the device if opened
             */
            virtual
            ~SpaceWireGR712();

            /**
             * Opens the device for read-write accesses. Afterwards
             * clock will be enabled to the device and configuration
             * will took place
             *
             * \return
             *         [True] if the device is opened successfully
             *         [False] error in opening
             */
            virtual bool
            open();

            /**
             * Disable the clock to the device and close the device
             * if opened
             */
            virtual void
            close();

            /**
             * Brings the link up. Blocking and non blocking differentiate
             * the waiting on the link for specific time to be up or no
             * waiting
             *
             * \param blockingMode
             *         Specify if the link should block during transmission
             *         or reception if no data is available
             */
            virtual void
            up(Blocking blockingMode = nonBlocking);

            /**
             * Shut down the link
             *
             * \param blockingMode
             *         Specify if the link should block during transmission
             *         or reception if no data is available
             */
            virtual void
            down(Blocking blockingMode = nonBlocking);

            /**
             * Before proceeding the transactions on the link, it has
             * to be verified if entered in Running state. Otherwise
             * the driver will return with error messages
             *
             * \param blockingMode
             *         Specify if the link should block during transmission
             *         or reception if no data is available
             * \return
             *         [True] if the device is opened successfully
             *         [False] error in opening
             */
            virtual bool
            isUp();

            /**
             * A pointer to the transmit buffer has been passed for fill up. Currently it is
             * a minimal implementation without any buffer management
             *
             * \param[out]    buffer
             *         Pointer reference to a send buffer
             * \param[in]    blockingMode
             *         Default is blocking mode.
             * \return
             *         Success
             */
            virtual Result
            requestBuffer(TransmitBuffer *& buffer, Blocking blockingMode =
                                  blocking);

            /**
             * Sending the data over SpW link by using RTEMS standard IO calls
             *
             * \param buffer
             *         Pointer to the transmit buffer object
             * \return
             *         [Success] for sending specified data bytes
             *         [Failure] for failed to send specified data bytes
             */
            virtual Result
            send(TransmitBuffer * buffer);

            /**
             * Receiving the data over SpW link by using RTEMS standard IO calls
             *
             * \param buffer
             *         Reference to the receive buffer object
             * \param blockingMode
             *         Block the reception (Currently implemented in open() method call)
             * \return
             *         [Success] for sending specified data bytes
             *         [Failure] for failed to send specified data bytes
             */
            virtual Result
            receive(ReceiveBuffer& buffer, Blocking blockingMode = blocking);

            /**
             * Intended to be used for releasing the receiving buffer
             * Not implemented. As the buffer is internally handled in the
             * GRSPW driver provided by the vendor and provide on option for
             * releasing it
             *
             * \param buffer
             *         Reference to the receive buffer object
             */
            virtual void
            releaseBuffer(const ReceiveBuffer& buffer);

            /**
             * Flush the buffers for the object
             */
            virtual void
            flushReceiveBuffer();

            inline bool
            isOpened()
            {
                return opened;
            }

        private:

            static inline unsigned int
            readReg(unsigned int addr)
            {
                return (*(volatile unsigned int *) (addr));
            }

            static inline void
            writeReg(unsigned int addr, unsigned int val)
            {
                *((volatile unsigned int *) addr) = val;
            }

            // Spacewire device access info
            uint8_t node; /**< SpaceWire node address */
            uint8_t devId; /**< Device ID corresponding to its access path */
            int devHandle; /**< SpaceWire access device handle */
            //rtos::Semaphore txLock;
            bool firstTransmit;

            // TX & RX buffer
            TransmitBuffer txBuff; /**< Transmit buffer object */
            uint8_t txBuffData[4500] ATTRIBUTE_ALIGNED(4); /**< Transmit buffer place holder */
            uint8_t rxBuffData[4500] ATTRIBUTE_ALIGNED(4); /**< Receive buffer place holder */

            // Status information
            uint32_t txCount; /**< Transmit data counter */
            uint32_t rxCount; /**< Receive data counter */
            bool opened;
            spw_stats devStatus; /**< Device status information */

            // Spacewire device configuration
            spw_config devConfig; /**< Device configuration */
            spw_ioctl_packetsize devPacketSize; /**< Device packet configuration */
            static const uint32_t rTimeout = 500; /**< Read timeout constant in blocking mode [ms]*/
            static const unsigned int clockGatingUnlockReg = 0x80000D00;
            static const unsigned int clockGatingEnableReg = 0x80000D04;
            static const unsigned int clockGatingResetReg = 0x80000D08;
        };

    }
}

#endif // COBC_LEON3_SPACEWIRE_GR712_H_
