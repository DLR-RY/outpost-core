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

#include "can_bus_stub.h"

#include <math.h>
#include <string.h>

using unittest::hal::CanBusStubBase;

CanBusStubBase::ReturnCode
CanBusStubBase::open()
{
    mOpen = true;
    return ReturnCode::success;
}

CanBusStubBase::ReturnCode
CanBusStubBase::close()
{
    mDataToReceive.clear();
    mDataToTransmit.clear();
    mOpen = false;
    return ReturnCode::success;
}

CanBusStubBase::ReturnCode
CanBusStubBase::read(CanFrame& frame, outpost::time::Duration timeout)
{
    (void) timeout;

    if (!mOpen)
    {
        return ReturnCode::notInitialized;
    }
    else if (mDataToReceive.isEmpty())
    {
        return ReturnCode::bufferEmpty;
    }

    frame = mDataToReceive.getFront();
    mDataToReceive.removeFront();
    return ReturnCode::success;
}

CanBusStubBase::ReturnCode
CanBusStubBase::write(const CanFrame& frame)
{
    if (!mOpen)
    {
        return ReturnCode::notInitialized;
    }

    mDataToTransmit.append(frame);
    return ReturnCode::success;
}

CanBusStubBase::ReturnCode
CanBusStubBase::clearReceiveBuffer()
{
    mDataToReceive.clear();
    return ReturnCode::success;
}

CanBusStubBase::ReturnCode
CanBusStubBase::transferToOtherStub(CanBusStubBase& rxStub, bool clearTxBuffer)
{
    if (!mOpen || !rxStub.mOpen)
    {
        return ReturnCode::notInitialized;
    }

    rxStub.mDataToReceive.append(mTransmitBuffer.skipFirst(1).first(mDataToTransmit.getSize()));

    if (clearTxBuffer)
    {
        mDataToTransmit.clear();
    }

    return ReturnCode::success;
}

bool
CanBusStubBase::isDataAvailable()
{
    return !mDataToReceive.isEmpty();
}
