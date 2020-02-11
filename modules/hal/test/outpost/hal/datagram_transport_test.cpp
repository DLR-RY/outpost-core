/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */

#include <outpost/hal/datagram_transport.h>

#include <unittest/harness.h>

using outpost::hal::DatagramTransport;

TEST(DatagramTransportTest, shouldAllowToComposeAddressConstants)
{
    constexpr DatagramTransport::Address address(DatagramTransport::IpAddress(127, 0, 0, 1), 8080);

    EXPECT_EQ(address.getPort(), 8080);

    std::array<uint8_t, 4> Ip = address.getIpAddress().getArray();
    std::array<uint8_t, 4> refIp = DatagramTransport::IpAddress(127, 0, 0, 1).getArray();
    for (size_t i = 0; i < refIp.size(); i++)
    {
        EXPECT_EQ(Ip[i], refIp[i]);
    }
}
