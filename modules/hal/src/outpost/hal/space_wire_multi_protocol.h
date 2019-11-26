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

#ifndef OUTPOST_HAL_SPACEWIRE_MULTI_PROTOCOL_H_
#define OUTPOST_HAL_SPACEWIRE_MULTI_PROTOCOL_H_

#include "protocol_dispatcher.h"
#include "protocol_dispatcher_thread.h"
#include "spacewire.h"

#include <outpost/time/clock.h>

#include <array>

namespace outpost
{
namespace hal
{
class SpaceWireMultiProtocolInterface : public virtual ProtocolDispatcherInterface<uint8_t>,
                                        public TimeCodeProvider
{
public:
    SpaceWireMultiProtocolInterface() = default;
    virtual ~SpaceWireMultiProtocolInterface() = default;
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
         outpost::time::Duration timeout = outpost::time::Duration::maximum()) = 0;
};

template <uint32_t numberOfQueues,       // how many queues can be included
          uint32_t maxPacketSize = 4500  // max number of bytes a received package can contain
          >
class SpaceWireMultiProtocol : public ProtocolDispatcher<uint8_t, numberOfQueues>,
                               public SpaceWireMultiProtocolInterface
{
public:
    SpaceWireMultiProtocol(SpaceWire& spw,
                           uint8_t priority,
                           size_t stackSize,
                           char* threadName,
                           outpost::support::parameter::HeartbeatSource heartbeatSource,
                           outpost::time::Clock& clock) :
        outpost::hal::ProtocolDispatcher<uint8_t, numberOfQueues>(1),
        mThread(*this,
                mReceiver,
                outpost::asSlice(mBuffer),
                priority,
                stackSize,
                threadName,
                heartbeatSource),
        mReceiver(spw),
        mClock(clock){

        };

    virtual ~SpaceWireMultiProtocol() = default;

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
         outpost::time::Duration timeout = outpost::time::Duration::maximum()) override;

    /**
     * Add a listener for timecode
     * @param queue the queue to add
     * @return false if queue == nullptr or all places for Listener are filled
     */
    virtual bool
    addTimeCodeListener(outpost::rtos::Queue<TimeCode>* queue) override;

    /**
     * Starts the underlying thread, call after SpW is opened
     */
    void
    start();

private:
    // As an private inner class so we don't expose an unchecked receive
    class SpaceWireReceiver : public ReceiverInterface
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

    outpost::hal::ProtocolDispatcherThread mThread;

    // use the receiver also to store the spw object for send.
    SpaceWireReceiver mReceiver;

    std::array<uint8_t, maxPacketSize> mBuffer;

    outpost::time::Clock& mClock;
};

}  // namespace hal
}  // namespace outpost

#include "space_wire_multi_protocol_impl.h"

#endif /* MODULES_HAL_SRC_OUTPOST_HAL_SPACEWIRE_DISPATCHER_H_ */
