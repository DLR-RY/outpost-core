/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Jan Malburg (DLR RY-AVS)
 */

#ifndef MODULES_UTILS_COMMUNICATOR_H_
#define MODULES_UTILS_COMMUNICATOR_H_

#include <outpost/time/duration.h>

namespace outpost
{
namespace utils
{
template <typename T>
class Receiver
{
public:
    Receiver() = default;
    virtual ~Receiver() = default;

    /**
     * \brief Receives data from the object.
     *
     * Can be either blocking (timeout > 0) or non-blocking (timeout = 0).
     * \param data Reference for the data type to be received.
     * \param timeout Duration for which the caller is willing to wait for incoming data
     * \return Returns true if data was received, false
     * otherwise (e.g. a timeout occured)
     */
    virtual bool
    receive(T& data, outpost::time::Duration timeout) = 0;

    /**
     * \brief Receives data from the object. With unlimited timeout.
     * Can block
     *
     * \param data Reference for the data type to be received.
     * \return Returns true if data was received, false
     * otherwise
     */
    bool
    receive(T& data)
    {
        return receive(data, outpost::time::Duration::myriad());
    }
};

template <typename T>
class Sender
{
public:
    Sender() = default;
    virtual ~Sender() = default;

    /**
     * \brief sends data .
     *
     * Can be either blocking (timeout > 0) or non-blocking (timeout = 0).
     * \param data Reference for the data type to be send.
     * \param timeout Duration for which the caller is willing to wait for sender to be valid
     * \return Returns true if data was send, false
     * otherwise (e.g. a timeout occured)
     */
    virtual bool
    send(T& data, outpost::time::Duration timeout) = 0;

    /**
     * \brief Sends data with unlimited timeout. Can block.
     *
     * \param data Reference for the data type to be send.
     * \return Returns true if data was send, false
     * otherwise
     */
    bool
    send(T& data)
    {
        return send(data, outpost::time::Duration::myriad());
    }
};

}  // namespace utils
}  // namespace outpost

#endif /* MODULES_UTILS_SRC_OUTPOST_UTILS_COMMUNICATOR_H_ */
