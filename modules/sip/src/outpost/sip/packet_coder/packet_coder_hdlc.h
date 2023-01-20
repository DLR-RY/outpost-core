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

#ifndef OUTPOST_SIP_PACKET_CODER_HDLC_H
#define OUTPOST_SIP_PACKET_CODER_HDLC_H

#include <outpost/sip/packet_coder/packet_coder.h>
#include <outpost/utils/coding/hdlc.h>

namespace outpost
{
namespace sip
{
/**
 * Packet encoder/decoder using HDLC byte stuffing
 *
 * The generated output byte stream can be passed directly to the UART
 * interface.
 *
 */
class PacketCoderHdlc : public PacketCoder
{
public:
    PacketCoderHdlc();

    virtual ~PacketCoderHdlc();

    /**
     * Encode a buffer into the output buffer.
     *
     * \param   input
     *      buffer to be send.
     * \param[out] output
     *      Buffer in which the encoded buffer is placed. Leading and trailing
     *      framing flag bytes are added.
     *
     *      Must not point to the same buffer in which the data of the buffer
     *      is located.
     *
     * \retval  true    Buffer was successfully encoded.
     * \retval  false   Encoded Buffer is to big for the provided
     *                  destination buffer.
     */
    bool
    encode(outpost::Slice<const uint8_t> const& input, outpost::Slice<uint8_t>& output) override;

    /**
     * Decode a buffer in place.
     *
     * Expects a HDLC byte stuffed buffer without leading or trailing flag bytes.
     *
     * \param input
     *      Encoded buffer. The data in the buffer is overwritten
     *      during the decoding.
     *
     *      The bounded array in the packet points into the array given
     *      through this parameter.
     * \param[out] output
     *      Decoded buffer header
     *
     * \retval true     Data successfully decoded
     * \retval false    Error happened during decoding. No valid
     *                  header is present in the packet parameter. The
     *                  data in \p buffer might be altered.
     */
    bool
    decode(outpost::Slice<const uint8_t> const& input, outpost::Slice<uint8_t>& output) override;

    /**
     * Check if byte equals the boundary byte.
     */
    bool
    isPacketBoundaryByte(uint8_t data) override;

private:
    static const uint8_t flagByte = outpost::utils::HdlcStuffing::boundary_byte;
};

}  // namespace sip
}  // namespace outpost
#endif
