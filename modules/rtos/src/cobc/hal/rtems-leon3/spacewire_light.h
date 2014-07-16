/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef COBC_LEON3_SPACEWIRE_H
#define COBC_LEON3_SPACEWIRE_H

#include <stdint.h>
#include <cstddef>

#include <cobc/rtos/semaphore.h>
#include <cobc/utils.h>

#include "../spacewire.h"

extern "C" {
#include "../../../../ext/rtems-leon3/spwl/spacewirelight.h"
}

namespace cobc
{
    namespace leon3
    {
        /**
         * SpaceWire Light Interface
         *
         * Uses the SpaceWire Light driver for RTEMS 4.10 on LEON3 from
         * opencores.org.
         *
         * \see        cobc::hal::Spacewire
         * \author    Fabian Greif
         */
        class SpaceWireLight : public hal::SpaceWire
        {
        public:
            /**
             * Connects to the index'th SpaceWire Light device found in the AMBA
             * plug&play map. Allocation of receive/transmit buffers and a reset
             * of the device core is done when calling open().
             */
            SpaceWireLight(uint32_t index);

            virtual ~SpaceWireLight();

            virtual bool
            open();

            virtual void
            close();

            virtual void
            up(Blocking blockingMode = nonBlocking);

            virtual void
            down(Blocking blockingMode = nonBlocking);

            virtual bool
            isUp();


            virtual Result
            requestBuffer(TransmitBuffer *& buffer, Blocking blockingMode = blocking);

            virtual Result
            send(TransmitBuffer* buffer);


            virtual Result
            receive(ReceiveBuffer& buffer, Blocking blockingMode = blocking);

            virtual void
            releaseBuffer(const ReceiveBuffer& buffer);


            virtual void
            flushReceiveBuffer();

        private:
            /// Convert form the hal::SpaceWire Format to spwl flags
            inline uint16_t
            eopFlags(EndMarker marker)
            {
                uint16_t flags = 0;
                switch (marker) {
                    case eop: flags = SPWL_EOP; break;
                    case eep: flags = SPWL_EEP; break;
                    case partial: break;
                }
                return flags;
            }

            /// Convert form spwl flags to the hal::SpaceWire Format
            inline EndMarker
            eopMarker(uint16_t flags)
            {
                EndMarker marker;
                if (flags & SPWL_EOP) {
                    marker = eop;
                }
                else if (flags & SPWL_EEP) {
                    marker = eep;
                }
                else {
                    marker = partial;
                }
                return marker;
            }

            /// SpaceWire device amba index
            uint32_t index;

            /// Used by the spwl driver to identify the SpaceWire device
            spwl_handle handle;

            bool firstTransmit;
            rtos::Semaphore txSync;
            TransmitBuffer txBuffer;

            // Flash page size is 4096+128 byte (=4224) + 9 byte AMAP overhead
            // => 4233 byte minimum
            uint8_t txBufferData[4500] ATTRIBUTE_ALIGNED(4);
            struct spwl_txbuf spwlBuffer;

        };
    }
}

#endif // COBC_LEON3_SPACEWIRE_H
