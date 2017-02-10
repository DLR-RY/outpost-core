/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
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
SerialStub::read(cobc::BoundedArray<uint8_t> data,
                 cobc::time::Duration /*timeout*/)
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
SerialStub::write(cobc::BoundedArray<const uint8_t> data,
                  cobc::time::Duration /*timeout*/)
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
