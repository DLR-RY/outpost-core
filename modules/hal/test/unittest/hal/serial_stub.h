/*
 * Copyright (c) 2015, German Aerospace Center (DLR)
 *
 * This file is part of outpost 0.6.
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

#ifndef UNITTEST_HAL_SERIAL_STUB_H
#define UNITTEST_HAL_SERIAL_STUB_H

#include <vector>

#include <outpost/hal/serial.h>

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
class SerialStub : public outpost::hal::Serial
{
public:
    SerialStub();

    virtual
    ~SerialStub();

    virtual void
    close() override;

    virtual bool
    isAvailable() override;

    virtual size_t
    getNumberOfBytesAvailable() override;

    virtual size_t
    read(outpost::BoundedArray<uint8_t> data,
         outpost::time::Duration timeout = outpost::time::Duration::maximum()) override;

    virtual size_t
    write(outpost::BoundedArray<const uint8_t> data,
          outpost::time::Duration timeout = outpost::time::Duration::maximum()) override;

    /// Does nothing in this implementation
    virtual void
    flushReceiver() override;

    /// Does nothing in this implementation
    virtual void
    flushTransmitter() override;

    /// Data which should be accessible through the read function
    std::vector<uint8_t> mDataToReceive;

    /// Data which has be stored through the write function
    std::vector<uint8_t> mDataToTransmit;
};

}
}

#endif
