/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
/**
 * \file
 * \brief    Default global configuration settings of the libCOBC library
 */

#ifndef MISSION_COBC_CONSTANTS_H
#define MISSION_COBC_CONSTANTS_H

#include <stdint.h>

#include <cobc/pus/time.h>
#include <cobc/time/duration.h>

namespace mission
{
namespace spp
{

}

namespace pus
{
static const uint16_t maximumNumberOfScheduleEntries = 50;
static const uint16_t maximumNumberOfSubSchedules = 10;
static const uint16_t maximumNumberOfInterlocks = 10;

/**
 * Space needed for the packet header and the data field header.
 *
 * - packet header     : 6 byte
 * - data field header : 4 byte
 *
 * \see TimeCode
 */
static const uint16_t telecommandSourcePacketHeaderSize = 10;

/**
 * Length of a TC source packet.
 */
static const uint16_t maximumTelecommandSourcePacketLength = 128;

/**
 * Number of parameter bytes for a PUS TC packet.
 *
 * Does not contain the the packet header (6 bytes), the data field header
 * (4 bytes) and the optional packet error control (not used here = 0 bytes).
 */
static const uint16_t maximumTelecommandApplicationDataLength =
        maximumTelecommandSourcePacketLength - telecommandSourcePacketHeaderSize;

/**
 * Space needed for the packet header and the data field header.
 *
 * - packet header     : 6 byte
 * - data field header : 4 byte + 6 byte time code
 *
 * \see TimeCode
 */
static const uint16_t telemetrySourcePacketHeaderSize = 6 + 4 + 6;

static const uint16_t maximumTelemetrySourcePacketLength = 128;
static const uint16_t maximumTelemetryApplicationDataLength =
        maximumTelemetrySourcePacketLength - telemetrySourcePacketHeaderSize;

static const uint16_t maximumNumberOfDestinations = 5;

// TODO replace this with something useful
static const uint8_t defaultDestinationId = 0;

/**
 * Maximum size of a source packet transported through the large data transfer
 * service.
 *
 * \warning
 *      Keep in mind that largeDataBufferSize should be divisible by
 *      \c largeDataSegmentSize without a remainder.
 */
static const uint16_t largeDataBufferSize = 4096;

/**
 * Size of a segment transported by the large data transfer service.
 *
 * This value combined with the large data segment header has always to be
 * smaller than the maximum length of a telemetry and telecommand source packet.
 *
 * \warning
 *      Keep in mind that largeDataBufferSize should be divisible by
 *      \c largeDataSegmentSize without a remainder.
 */
static const uint16_t largeDataSegmentSize = 64;

/**
 * Time after which an uplink is aborted if no new messages are received
 * successfully.
 */
static const cobc::time::Duration largeDataUplinkReceptionTimeout = cobc::time::Seconds(10);

/**
 * Default time code used for the telemetry packets.
 *
 * Total length is 6 bytes. With 4 bytes coarse time and 2 bytes fine
 * time the usable time span covers ~136 years with a resolution of
 * ~15 ms.
 *
 * \see cobc::pus::Telemetry
 */
typedef ::cobc::spp::UnsegmentedTimeCode<4, 2> TimeCode;
}
}

#endif // MISSION_COBC_CONSTANTS_H
