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

#ifndef OUTPOST_HAL_SPI_H
#define OUTPOST_HAL_SPI_H

#include <outpost/base/slice.h>

#include <stdint.h>

#include <cstddef>

namespace outpost
{
namespace hal
{
/**
 * Spi Interface.
 *
 * \author  Andre Nahrwold
 */
class Spi
{
public:
    /**
     * Defines the available return codes
     */
    enum class ReturnCode
    {
        /// On success
        success,
        /// When the device was not opened before
        notOpen,
        /// The data for read, write or transfer are invalid (e.g. nullptr)
        dataInvalid,
        /// The selected chip select is not available for this device
        chipSelectInvalid,
        /// When the in and out data have different sizes (for transfer only)
        inAndOutDataHaveDifferentSize
    };

    /**
     * This chip select value can be used if no chip select should be executed
     */
    static constexpr uint8_t noChipSelect = 0xFF;

    /**
     * Constructor
     */
    Spi() = default;

    virtual ~Spi() = default;

    /**
     * Close the SPI device
     */
    virtual void
    close() = 0;

    /**
     * Read a block of bytes.
     *
     * Reads the given number of bytes from the desired device connected to the
     * given chip select.
     *
     * \param data
     *      Buffer to write the received data to.
     * \param cs
     *      Chip select of the desired device.
     * \return
     *      A return code
     */
    virtual ReturnCode
    read(outpost::Slice<uint8_t> data, uint8_t cs = noChipSelect) = 0;

    /**
     * Write a block of bytes.
     *
     * Writes the given number of bytes to the desired device connected to the
     * given chip select.
     *
     * \param data
     *      Buffer containing the data to send.
     * \param cs
     *      Chip select of the desired device.
     * \return
     *      A return code
     */
    virtual ReturnCode
    write(outpost::Slice<const uint8_t> data, uint8_t cs = noChipSelect) = 0;

    /**
     * Simultaneously write and read a block of bytes.
     *
     * Writes the given number of bytes from \p outData while reading the
     * same number of bytes to \p inData from the desired device connected to
     * the given chip select.
     *
     * \param outData
     *      Buffer containing the data to send.
     * \param inData
     *      Buffer to write the received data to.
     * \param cs
     *      Chip select of the desired device.
     * \return
     *      A return code
     */
    virtual ReturnCode
    transfer(outpost::Slice<const uint8_t> outData,
             outpost::Slice<uint8_t> inData,
             uint8_t cs = noChipSelect) = 0;
};

}  // namespace hal
}  // namespace outpost

#endif
