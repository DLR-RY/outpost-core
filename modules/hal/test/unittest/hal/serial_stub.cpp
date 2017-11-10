/*
 * Copyright (c) 2015-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include "serial_stub.h"

#include <string.h>

using unittest::hal::SerialStub;

SerialStub::SerialStub()
{
}

SerialStub::~SerialStub()
{
}

void
SerialStub::close()
{
    mDataToReceive.clear();
    mDataToTransmit.clear();
}

bool
SerialStub::isAvailable()
{
    return !mDataToReceive.empty();
}

size_t
SerialStub::getNumberOfBytesAvailable()
{
    return mDataToReceive.size();
}

size_t
SerialStub::read(outpost::Slice<uint8_t> data,
                 outpost::time::Duration /*timeout*/)
{
    size_t length = data.getNumberOfElements();
    size_t available = mDataToReceive.size();

    if (length > available)
    {
        length = available;
    }

    if (length > 0)
    {
        memcpy(&data[0], &mDataToReceive[0], length);
        mDataToReceive.erase(mDataToReceive.begin(), mDataToReceive.begin() + length);
    }
    return length;
}

size_t
SerialStub::write(outpost::Slice<const uint8_t> data,
                  outpost::time::Duration /*timeout*/)
{
    mDataToTransmit.insert(mDataToTransmit.end(), data.begin(), data.end());
    return data.getNumberOfElements();
}

void
SerialStub::flushReceiver()
{
    // Do nothing
}

void
SerialStub::flushTransmitter()
{
    // Do nothing
}
