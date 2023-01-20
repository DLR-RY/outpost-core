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
 * - 2017, Muhammad Bassam (DLR RY-AVS)
 */

#ifndef OUTPOST_COMM_RMAP_COMMON_H_
#define OUTPOST_COMM_RMAP_COMMON_H_

#include <stdint.h>

namespace outpost
{
namespace comm
{
namespace rmap
{
static constexpr uint16_t maxTransactionId = UINT16_MAX;
static constexpr uint8_t maxConcurrentTransactions = 10;
static constexpr uint16_t bufferSize = 1024;  // The amount of data bytes.
static constexpr uint8_t numberOfReceiveBuffers =
        3;  // How many reply packages can be queued,
            // including the currently processed one (i.e. min = 1)
static constexpr uint8_t defaultLogicalAddress = 0xFE;
static constexpr uint8_t defaultExtendedAddress = 0x00;
static constexpr uint8_t protocolIdentifier = 0x01;

static constexpr uint16_t writeCommandOverhead = 17;  // including data crc
static constexpr uint16_t readCommandOverhead = 16;
static constexpr uint16_t readReplyOverhead = 13;  // includes data crc
static constexpr uint16_t writeReplyOverhead = 8;

static constexpr uint8_t minimumReplySize = writeReplyOverhead;
// Maximum physical output ports that router can have (see ECSS-E-ST-50-12C pg. 98)
static constexpr uint8_t maxPhysicalRouterOutputPorts = 32;

// Maximum nodes is specified from the number of SpW target addresses that
// can be accommodated into a single RMAP packet (see ECSS-E-ST-50-52C pg. 24)
static const uint8_t maxAddressLength = 12;
static const uint8_t maxNodeNameLength = 20;

}  // namespace rmap
}  // namespace comm
}  // namespace outpost

#endif /* OUTPOST_COMM_RMAP_COMMON_H_ */
