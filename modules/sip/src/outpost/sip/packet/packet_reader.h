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

#ifndef OUTPOST_SIP_PAKCET_READER_H_
#define OUTPOST_SIP_PAKCET_READER_H_

#include <outpost/base/slice.h>
#include <outpost/sip/sip.h>
#include <outpost/utils/storage/serialize.h>

namespace outpost
{
namespace sip
{
class PacketReader
{
    /*
     * Read a SIP packet.
     * Deserialize it and check the CRC.
     */
public:
    /*
     * Constructs the SIP packet reader
     *
     * \param bufferIn
     * 		Consist a packet, which will be read out
     */
    explicit PacketReader(outpost::Slice<uint8_t> bufferIn);

    ~PacketReader();

    enum class readResults
    {
        success,
        lengthError,
        crcError
    };

    /*
     * Read a SIP packet
     *
     * \retval success
     *		Success
     * \retval lengthError
     *		Deserialize failed. Lenght is wrong.
     * \retval crcError
     * 		CRC error.
     */
    readResults
    readPacket();

    /*
     * Get length of the packet.
     */
    inline uint16_t
    getLength() const
    {
        return mLength;
    }

    /*
     * Get WorkerID.
     */
    inline uint8_t
    getWorkerId() const
    {
        return mWorkerId;
    }

    /*
     * Get counter.
     */
    inline uint8_t
    getCounter() const
    {
        return mCounter;
    }

    /*
     * Get packet type.
     */
    inline uint8_t
    getType() const
    {
        return mType;
    }

    /*
     * Get payload data.
     */
    inline outpost::Slice<uint8_t>
    getPayloadData()
    {
        return mBuffer.subSlice(sip::payloadStartPosition, mPayloadDataLength);
    }

private:
    uint16_t mLength;
    uint8_t mWorkerId;
    uint8_t mCounter;
    uint8_t mType;
    uint16_t mPayloadDataLength;
    uint16_t mCrc;
    outpost::Slice<uint8_t> mBuffer;

    bool
    deserialize();

    uint16_t
    checkCrc();
};

}  // namespace sip
}  // namespace outpost
#endif /* OUTPOST_SIP_PAKCET_READER_H_ */
