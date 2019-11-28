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

#ifndef OUTPOST_HAL_RECEIVER_INTERFACE_H_
#define OUTPOST_HAL_RECEIVER_INTERFACE_H_

#include <outpost/base/slice.h>
#include <outpost/time/duration.h>

#include <stdint.h>

namespace outpost
{
namespace hal
{
class ReceiverInterface
{
public:
    virtual ~ReceiverInterface() = default;

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
    receive(outpost::Slice<uint8_t>& buffer, outpost::time::Duration timeout) = 0;
};

}  // namespace hal
}  // namespace outpost

#endif /* OUTPOST_HAL_RECEIVER_INTERFACE_H_ */
