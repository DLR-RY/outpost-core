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
