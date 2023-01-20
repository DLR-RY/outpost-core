/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of the Simple Interface
 * Protocol.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Norbert Toth (DLR RY-AVS)
 */

#ifndef OUTPOST_SIP_PACKET_WRITER_H_
#define OUTPOST_SIP_PACKET_WRITER_H_

#include <outpost/base/slice.h>
#include <outpost/sip/sip.h>
#include <outpost/utils/storage/serialize.h>

#include <stdint.h>

namespace outpost
{
namespace sip
{
/*
 * Write a SIP packet.
 */
class PacketWriter
{
public:
    /*
     * Constructs a packet
     *
     * /param bufferIn
     * 		The created packet will be wrote in this buffer.
     */
    explicit PacketWriter(outpost::Slice<uint8_t> bufferIn);

    ~PacketWriter();

    /*
     * Set WorkerID.
     */
    inline void
    setWorkerId(uint8_t workerId)
    {
        mWorkerId = workerId;
    }

    /*
     * Set counter.
     */
    inline void
    setCounter(uint8_t counter)
    {
        mCounter = counter;
    }

    /*
     * Set packet type.
     */
    inline void
    setType(uint8_t type)
    {
        mType = type;
    }

    /*
     * Set payload data.
     */
    inline void
    setPayloadData(outpost::Slice<uint8_t> payloadData)
    {
        mPayloadBuffer = payloadData;
    }

    /*
     * Create packet and get it.
     */
    outpost::Slice<uint8_t>
    get();

private:
    uint16_t mLength;
    uint8_t mWorkerId;
    uint8_t mCounter;
    uint8_t mType;
    outpost::Slice<uint8_t> mPayloadBuffer;
    outpost::Slice<uint8_t> mBufferToWrite;
};
}  // namespace sip
}  // namespace outpost
#endif /* OUTPOST_SIP_PACKET_WRITER_H_ */
