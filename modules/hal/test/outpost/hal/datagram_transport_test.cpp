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
 * - 2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#include <unittest/harness.h>

#include <outpost/hal/datagram_transport.h>

using outpost::hal::DatagramTransport;

TEST(DatagramTransportTest, shouldAllowToComposeAddressConstants)
{
    constexpr DatagramTransport::Address address(DatagramTransport::IpAddress(127, 0, 0 , 1), 8080);
}
