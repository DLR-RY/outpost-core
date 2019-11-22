/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2019, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_HAL_SPACEWIRE_DISPATCHER_H_
#define OUTPOST_HAL_SPACEWIRE_DISPATCHER_H_

#include "protocol_dispatcher.h"
#include "spacewire.h"
#include <array>

namespace outpost
{
namespace hal
{
template <uint32_t numberOfQueues,  // how many queues can be included
          uint32_t maxPacketSize    // max number of bytes a received package can contain
          >
class SpacewireDispatcher : public ProtocolDispatcher<uint8_t, numberOfQueues>
{
public:
    SpacewireDispatcher(SpaceWire& spw,
                        uint8_t priority,
                        size_t stackSize,
                        char* threadName,
                        outpost::support::parameter::HeartbeatSource heartbeatSource) :
        outpost::hal::ProtocolDispatcher<uint8_t,numberOfQueues>(
                mReceiver, outpost::asSlice(mBuffer), 1, priority, stackSize, threadName, heartbeatSource),
        mReceiver(spw){

        };

    virtual ~SpacewireDispatcher() = default;

    /**
     * Send the contents of buffer
     *
     * @param buffer	data to send
     * @param timeout	maximum time to wait for sending
     *
     * @return 		true  if successful
     * 				false if timeout
     * 						 invalid data
     * 						 any failure in underlying sender
     */
    virtual bool
    send(const outpost::Slice<const uint8_t>& buffer,
         outpost::time::Duration timeout = outpost::time::Duration::maximum());

private:
    // As an private inner class so we don't expose an unchecked receive
    class SpaceWireReceiver : public Receiver
    {
    public:
        SpaceWireReceiver(SpaceWire& spw) : mSpw(spw){};

        virtual ~SpaceWireReceiver() = default;

        /**
         * receives a data package
         *
         * @param buffer 	buffer to write the data to
         * @param timeout	max timeout to wait for data
         *
         * @return 0	If no package received in the time or failure in underlying receiver
         * 		   >0   Number of bytes received in package, if larger then
         * buffer.getNumberOfElements(), data has been lost
         */
        virtual uint32_t
        receive(outpost::Slice<uint8_t>& buffer, outpost::time::Duration timeout);

        SpaceWire& mSpw;
    };
    // use the receiver also to store the spw object for send.
    SpaceWireReceiver mReceiver;

    std::array<uint8_t,maxPacketSize> mBuffer;
};

}  // namespace hal
}  // namespace outpost

#endif /* MODULES_HAL_SRC_OUTPOST_HAL_SPACEWIRE_DISPATCHER_H_ */
