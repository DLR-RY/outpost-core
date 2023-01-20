/*
 * Copyright (c) 2014-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2018, Fabian Greif (DLR RY-AVS)
 * - 2014, Norbert Toth (DLR RY-AVS)
 * - 2015, 2017, Jan Sommer (DLR SC-SRV)
 */

#ifndef OUTPOST_HAL_SERIAL_H
#define OUTPOST_HAL_SERIAL_H

#include <outpost/base/slice.h>
#include <outpost/time/duration.h>

#include <stdint.h>

#include <cstddef>

namespace outpost
{
namespace hal
{
/**
 * Serial Interface.
 *
 * \author  Fabian Greif
 */
class Serial
{
public:
    virtual ~Serial() = 0;

    /**
     * Close the UART device
     */
    virtual void
    close() = 0;

    /**
     * Check if data is available in the input buffers.
     *
     * \retval true   Data is available and can be read via read(...).
     * \retval false  No data available.
     */
    virtual bool
    isAvailable() = 0;

    /**
     * Check how many bytes are available in the input buffers.
     *
     * \return  Number of bytes in the buffer
     */
    virtual size_t
    getNumberOfBytesAvailable() = 0;

    /**
     * Read a block of bytes.
     *
     * Waits until the timeout occurs to read the given number of
     * bytes. May return earlier if enough bytes are available. The
     * exact number of bytes read will be returned, it will be up to
     * \p data.getNumberOfElements() bytes but can also be any lower value.
     *
     * \param data
     *      Buffer to write the received data to.
     * \param timeout
     *      Will return if call has exceeded this time, default is
     *      blocking call.
     * \return
     *      Number of bytes which could be read, maximal
     *      \p data.getNumberOfElements().
     */
    virtual size_t
    read(outpost::Slice<uint8_t> data,
         outpost::time::Duration timeout = std::numeric_limits<outpost::time::Duration>::max()) = 0;

    /**
     * Write a block of bytes with timeout.
     *
     * \param data
     *      Buffer containing the data to send.
     * \param timeout
     *      Will return if call has exceeded this time, default is
     *      blocking call.
     * \return
     *      Number of bytes which could be sent, maximal
     *      \p data.getNumberOfElements().
     */
    virtual size_t
    write(outpost::Slice<const uint8_t> data,
          outpost::time::Duration timeout =
                  std::numeric_limits<outpost::time::Duration>::max()) = 0;

    /**
     * Flush receive buffers.
     *
     * Data remaining in the receive buffer is discarded. Afterwards all
     * internal buffers are empty.
     */
    virtual void
    flushReceiver() = 0;

    /**
     * Sends eventually buffered data. Afterwards all internal buffers
     * are empty.
     */
    virtual void
    flushTransmitter() = 0;
};

}  // namespace hal
}  // namespace outpost

#endif
