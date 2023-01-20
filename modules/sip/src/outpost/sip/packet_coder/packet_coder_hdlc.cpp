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

#include "packet_coder_hdlc.h"

#include <outpost/utils/coding/hdlc.h>

outpost::sip::PacketCoderHdlc::PacketCoderHdlc()
{
}

outpost::sip::PacketCoderHdlc::~PacketCoderHdlc()
{
}

bool
outpost::sip::PacketCoderHdlc::encode(outpost::Slice<const uint8_t> const& input,
                                      outpost::Slice<uint8_t>& output)
{
    size_t encodedLength = outpost::utils::HdlcStuffing::encode(input, output);

    bool result = false;
    if (encodedLength > input.getNumberOfElements())
    {
        // encodedLength should be bigger as input
        // at least 2 bytes (0x7E, boundary)
        result = true;
    }

    return result;
}

bool
outpost::sip::PacketCoderHdlc::decode(outpost::Slice<const uint8_t> const& input,
                                      outpost::Slice<uint8_t>& output)
{
    size_t decodedLength = outpost::utils::HdlcStuffing::decode(input, output);

    bool result = false;
    if (decodedLength != 0)
    {
        result = true;
    }

    return result;
}

bool
outpost::sip::PacketCoderHdlc::isPacketBoundaryByte(uint8_t data)
{
    bool result = false;
    if (data == flagByte)
    {
        result = true;
    }
    return result;
}
