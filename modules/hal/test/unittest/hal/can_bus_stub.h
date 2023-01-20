/*
 * Copyright (c) 2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2022, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef UNITTEST_HAL_CAN_STUB_H
#define UNITTEST_HAL_CAN_STUB_H

#include <outpost/hal/can_bus.h>
#include <outpost/utils/container/deque.h>

namespace unittest
{
namespace hal
{
/**
 * Serial interface stub.
 *
 * Provides direct access to the send- and receive queue.
 *
 * \author  Fabian Greif
 */

class CanBusStubBase : public outpost::hal::CanBus
{
public:
    CanBusStubBase(const outpost::Slice<CanFrame>& rxBuffer,
                   const outpost::Slice<CanFrame>& txBuffer) :
        mDataToReceive(rxBuffer),
        mTransmitBuffer(txBuffer),
        mDataToTransmit(mTransmitBuffer),
        mOpen(false)
    {
    }

    virtual ~CanBusStubBase() = default;

    virtual ReturnCode
    open() override;

    virtual ReturnCode
    close() override;

    virtual ReturnCode
    read(CanFrame& frame,
         outpost::time::Duration timeout = outpost::time::Duration::myriad()) override;

    virtual ReturnCode
    write(const CanFrame& frame) override;

    virtual ReturnCode
    clearReceiveBuffer() override;

    virtual bool
    isDataAvailable() override;

    /// Test method to simulate CAN communication
    ReturnCode
    transferToOtherStub(CanBusStubBase& rxStub, bool clearTxBuffer = true);

    /// Data which should be accessible through the read function
    outpost::Deque<CanFrame> mDataToReceive;

    /// Data which has be stored through the write function
    const outpost::Slice<CanFrame> mTransmitBuffer;
    outpost::Deque<CanFrame> mDataToTransmit;

    bool mOpen;
};

template <size_t N>
class CanBusStubStorage
{
protected:
    std::array<outpost::hal::CanBus::CanFrame, N> mRxBuffer;
    std::array<outpost::hal::CanBus::CanFrame, N> mTxBuffer;
};

template <size_t N>
class CanBusStubWithStorage : public CanBusStubStorage<N>, public CanBusStubBase
{
public:
    CanBusStubWithStorage() :
        CanBusStubStorage<N>(),
        CanBusStubBase(outpost::asSlice(CanBusStubStorage<N>::mRxBuffer),
                       outpost::asSlice(CanBusStubStorage<N>::mTxBuffer))
    {
    }
};

using CanBusStub = CanBusStubWithStorage<32>;

}  // namespace hal
}  // namespace unittest

#endif
