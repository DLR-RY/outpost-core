/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Andre Nahrwold (DLR SC-SRV)
 */

#ifndef UNITTEST_HAL_SPI_STUB_H
#define UNITTEST_HAL_SPI_STUB_H

#include <outpost/hal/spi.h>

#include <vector>

using namespace outpost::hal;

namespace unittest
{
namespace hal
{
/**
 * Spi interface stub.
 *
 * Provides direct access to the send- and receive queue.
 *
 * \author  Andre Nahrwold
 */
class SpiStub : public Spi
{
public:
    /**
     * Constructor
     */
    SpiStub();

    /**
     * Destructor
     */
    ~SpiStub();

    void
    close() override;

    Spi::ReturnCode
    read(outpost::Slice<uint8_t> data, uint8_t cs = outpost::hal::Spi::noChipSelect) override;

    Spi::ReturnCode
    write(outpost::Slice<const uint8_t> data,
          uint8_t cs = outpost::hal::Spi::noChipSelect) override;

    Spi::ReturnCode
    transfer(outpost::Slice<const uint8_t> outData,
             outpost::Slice<uint8_t> inData,
             uint8_t cs = outpost::hal::Spi::noChipSelect) override;

    /// Data which should be accessible through the read + transfer function
    std::vector<uint8_t> mDataToRead;

    /// Data which has been stored through the write + transfer function
    std::vector<uint8_t> mDataToWrite;
};

}  // namespace hal
}  // namespace unittest

#endif
