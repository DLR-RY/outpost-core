/*
 * Copyright (c) 2015-2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of the Simple Interface
 * Protocol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2015-2018, Fabian Greif (DLR RY-AVS)
 * - 2020, Thomas Firchau (DLR RY-AVS)
 * - 2021, Norbert Toth (DLR RY-AVS)
 */

#ifndef OUTPOST_SIP_FRAME_CODER_H
#define OUTPOST_SIP_FRAME_CODER_H

#include <outpost/base/slice.h>

namespace outpost
{
namespace sip
{
/**
 * Packet encoder/decoder
 *
 * The generated output byte stream can be passed directly to the UART
 * interface.
 *
 * \author  Fabian Greif
 * \author  Norbert Toth
 */
class PacketCoder
{
public:
    virtual ~PacketCoder() = default;

    /**
     * Encode a buffer into the destination buffer.
     *
     * \param   input
     *      Packet to be send.
     * \param[out] output
     *      Buffer in which the encoded packet is placed. Leading and trailing
     *      framing zero bytes are added.
     *
     *      Must not point to the same buffer in which the data of the packet
     *      is located.
     *
     * \retval  true    buffer was successfully encoded.
     * \retval  false   Encoded buffer is to big for the provided
     *                  destination buffer.
     */
    virtual bool
    encode(outpost::Slice<const uint8_t> const& input, outpost::Slice<uint8_t>& output) = 0;

    /**
     * Decode a buffer in place.
     *
     * \param[in] input
     *      Encoded packet.
     *
     *      The bounded array in the packet points into the array given
     *      through this parameter.
     * \param[out] output
     *      Decoded buffer header
     *
     * \retval true     Data successfully decoded
     * \retval false    Error happened during decoding. No valid buffer
     *                  header is present in the buffer parameter. The
     *                  data in \p buffer might be altered.
     */
    virtual bool
    decode(outpost::Slice<const uint8_t> const& input, outpost::Slice<uint8_t>& output) = 0;

    /**
     * Check if byte equals the boundary byte.
     */
    virtual bool
    isPacketBoundaryByte(uint8_t data) = 0;
};

}  // namespace sip
}  // namespace outpost
#endif
