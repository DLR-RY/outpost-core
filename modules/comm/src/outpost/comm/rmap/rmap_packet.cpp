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
 * - 2017, Fabian Greif (DLR RY-AVS)
 */

#include "rmap_packet.h"

#include <outpost/utils/coding/crc.h>

using namespace outpost::comm;

RmapPacket::RmapPacket() :
    mSpwTargetAddressLength(0),
    mTargetLogicalAddress(rmap::defaultLogicalAddress),
    mInstruction(),
    mDestKey(0),
    mInitiatorLogicalAddress(rmap::defaultLogicalAddress),
    mExtendedAddress(rmap::defaultExtendedAddress),
    mTransactionIdentifier(0),
    mAddress(0),
    mDataLength(0),
    mStatus(RmapReplyStatus::unknown),
    mHeaderLength(0),
    mData(Slice<const uint8_t>::empty()),
    mHeaderCRC(0),
    mDataCRC(0)

{
    memset(mSpwTargets, 0, sizeof(mSpwTargets));
    memset(mReplyAddress, 0, sizeof(mReplyAddress));
}

RmapPacket::RmapPacket(outpost::Slice<uint8_t> spwTargets,
                       uint8_t targetLogicalAddress,
                       InstructionField::ReplyAddressLength rplyAddrLen,
                       uint8_t key,
                       uint8_t* replyAddress,
                       uint8_t initiatorLogicalAddress,
                       uint32_t address,
                       uint32_t dataLength) :
    mSpwTargetAddressLength(spwTargets.getNumberOfElements()),
    mTargetLogicalAddress(targetLogicalAddress),
    mInstruction(),
    mDestKey(key),
    mInitiatorLogicalAddress(initiatorLogicalAddress),
    mExtendedAddress(rmap::defaultExtendedAddress),
    mTransactionIdentifier(0),
    mAddress(address),
    mDataLength(dataLength),
    mStatus(RmapReplyStatus::unknown),
    mHeaderLength(0),
    mData(Slice<const uint8_t>::empty()),
    mHeaderCRC(0),
    mDataCRC(0)

{
    if (spwTargets.getNumberOfElements() <= sizeof(mSpwTargets))
    {
        memcpy(mSpwTargets, spwTargets.begin(), spwTargets.getNumberOfElements());
    }
    if ((rplyAddrLen * 4) <= sizeof(mReplyAddress))
    {
        memcpy(mReplyAddress, replyAddress, rplyAddrLen * 4);
    }
}

RmapPacket::RmapPacket(uint8_t targetLogicalAddress,
                       uint8_t key,
                       uint8_t initiatorLogicalAddress,
                       uint32_t address,
                       uint32_t dataLength) :
    mSpwTargetAddressLength(0),
    mTargetLogicalAddress(targetLogicalAddress),
    mInstruction(),
    mDestKey(key),
    mInitiatorLogicalAddress(initiatorLogicalAddress),
    mExtendedAddress(rmap::defaultExtendedAddress),
    mTransactionIdentifier(0),
    mAddress(address),
    mDataLength(dataLength),
    mStatus(RmapReplyStatus::unknown),
    mHeaderLength(0),
    mData(Slice<const uint8_t>::empty()),
    mHeaderCRC(0),
    mDataCRC(0)
{
    memset(mSpwTargets, 0, sizeof(mSpwTargets));
    memset(mReplyAddress, 0, sizeof(mReplyAddress));
}

RmapPacket::~RmapPacket()
{
}

void
RmapPacket::reset()
{
    RmapPacket empty;
    *this = empty;
}

void
RmapPacket::setData(const outpost::Slice<const uint8_t>& data)
{
    mData = data;
    mDataLength = data.getNumberOfElements();
    mDataCRC = outpost::Crc8CcittReversed::calculate(data);
}

bool
RmapPacket::constructPacket(outpost::Slice<uint8_t>& buffer)
{
    uint32_t headerLength =
            (rmap::readCommandOverhead + (sizeof(uint32_t) * mInstruction.getReplyAddressLength())
             + mSpwTargetAddressLength);

    if (buffer.getNumberOfElements() < headerLength)
    {
        console_out("RMAP-Packet: Trying to send larger packet than available buffer space\n");
        return false;
    }

    outpost::Serialize stream(buffer);

    // Construct header first
    constructHeader(stream);

    // Append data and CRC only if packet is write command
    if (isWrite())
    {
        // Check that the data can fit into the buffer, 1 byte for data crc
        if ((buffer.getNumberOfElements() < headerLength + mDataLength + 1))
        {
            console_out("RMAP-Packet: Trying to send larger packet than available buffer "
                        "space\n");
            return false;
        }

        // sanity check
        if (mData.getNumberOfElements() != mDataLength)
        {
            console_out("RMAP-Packet: dataLength and provided data is not equal for write "
                        "command\n");
            return false;
        }

        stream.store(mData);
        stream.store<uint8_t>(mDataCRC);
    }

    buffer = buffer.first(stream.getPosition());

    return true;
}

RmapPacket::ExtractionResult
RmapPacket::extractReplyPacket(outpost::Slice<const uint8_t>& data, uint8_t initiatorLogicalAddress)
{
    if (data.getNumberOfElements() < rmap::minimumReplySize)
    {
        console_out("RMAP-Packet: packet size less then minimum\n");
        return ExtractionResult::invalid;
    }

    outpost::Deserialize stream(data);

    const uint8_t initiatoraLogicalAddress = stream.read<uint8_t>();

    if (initiatoraLogicalAddress != initiatorLogicalAddress)
    {
        console_out("RMAP-Packet: Initiator logical address doesn't match\n");
        return ExtractionResult::incorrectAddress;
    }

    const uint8_t protocolIdentifiter = stream.read<uint8_t>();

    if (protocolIdentifiter != rmap::protocolIdentifier)
    {
        console_out("RMAP-Packet: Protocol ID is not RMAP\n");
        return ExtractionResult::invalid;
    }

    mInstruction.setAllRaw(stream.read<uint8_t>());

    // If reply packet is received
    if (isReplyPacket())
    {
        ptrdiff_t headerEndPosition;
        memset(mReplyAddress, 0, rmap::maxAddressLength);

        mInitiatorLogicalAddress = initiatoraLogicalAddress;
        mStatus = stream.read<uint8_t>();

        mTargetLogicalAddress = stream.read<uint8_t>();
        mTransactionIdentifier = stream.read<uint16_t>();

        // Write command reply
        if (isWrite())
        {
            if (data.getNumberOfElements() != rmap::writeReplyOverhead)
            {
                console_out("RMAP-Packet: Incorrect size for write reply\n");
                return ExtractionResult::invalid;
            }
            headerEndPosition = stream.getPosition();
            const uint8_t packetHeaderCRC = stream.read<uint8_t>();

            const uint8_t calculatedHeaderCRC = outpost::Crc8CcittReversed::calculate(
                    outpost::Slice<uint8_t>::unsafe(const_cast<uint8_t*>(stream.getPointer()),
                                                    static_cast<size_t>(headerEndPosition)));

            if (calculatedHeaderCRC != packetHeaderCRC)
            {
                console_out("RMAP-Packet: invalid packet header CRC\n");
                return ExtractionResult::crcError;
            }
            mHeaderCRC = packetHeaderCRC;
        }
        // Read command reply
        else
        {
            if (data.getNumberOfElements() < rmap::readReplyOverhead)
            {
                console_out("RMAP-Packet: too small read reply\n");
                return ExtractionResult::invalid;
            }

            // Skip the reserved byte
            stream.skip(1);

            // Read data length
            mDataLength = stream.readUnsigned24();

            // Get the header CRC
            headerEndPosition = stream.getPosition();
            const uint8_t packetHeaderCRC = stream.read<uint8_t>();

            const uint8_t calculatedHeaderCRC = outpost::Crc8CcittReversed::calculate(
                    outpost::Slice<uint8_t>::unsafe(const_cast<uint8_t*>(stream.getPointer()),
                                                    static_cast<size_t>(headerEndPosition)));

            if (calculatedHeaderCRC != packetHeaderCRC)
            {
                console_out("RMAP-Packet: Invalid packet header CRC\n");
                return ExtractionResult::crcError;
            }
            mHeaderCRC = packetHeaderCRC;

            // Check data length
            if (mDataLength + rmap::readReplyOverhead != data.getNumberOfElements())
            {
                console_out("RMAP-Packet: data length mismatch\n");
                return ExtractionResult::invalid;
            }

            mData = Slice<const uint8_t>::unsafe(stream.getPointerToCurrentPosition(), mDataLength);

            // Skip the data byte to calculate data CRC
            stream.skip(mDataLength);

            const uint8_t packetDataCRC = stream.read<uint8_t>();

            const uint8_t calculatedDataCRC = outpost::Crc8CcittReversed::calculate(mData);
            if (packetDataCRC != calculatedDataCRC)
            {
                console_out("RMAP-Packet: Invalid packet data CRC\n");
                return ExtractionResult::crcError;
            }
            mDataCRC = packetDataCRC;
        }
        mHeaderLength = headerEndPosition;
    }

    return ExtractionResult::success;
}

void
RmapPacket::setTargetInformation(RmapTargetNode& rmapTargetNode)
{
    // Set packet target logical address field according to the RMAP target node
    mTargetLogicalAddress = rmapTargetNode.getTargetLogicalAddress();

    // Set packet reply address field according to the RMAP target node
    outpost::Slice<uint8_t> rplyAddr = rmapTargetNode.getReplyAddress();
    memcpy(mReplyAddress, &rplyAddr[0], rplyAddr.getNumberOfElements());
    mInstruction.setReplyAddressLength(
            static_cast<InstructionField::ReplyAddressLength>(rplyAddr.getNumberOfElements() / 4));

    // Set packet target path SpW address field according to the RMAP target node
    setTargetSpaceWireAddress(rmapTargetNode.getTargetSpaceWireAddress());

    // Set packet key field according to the RMAP target node
    setKey(rmapTargetNode.getKey());
}

RmapPacket&
RmapPacket::operator=(const RmapPacket& rhs)
{
    mSpwTargetAddressLength = rhs.mSpwTargetAddressLength;
    memcpy(mSpwTargets, rhs.mSpwTargets, sizeof(mSpwTargets));
    mTargetLogicalAddress = rhs.mTargetLogicalAddress;
    mInstruction.setAllRaw(rhs.mInstruction.getRaw());
    mDestKey = rhs.mDestKey;
    memcpy(mReplyAddress, rhs.mReplyAddress, sizeof(mReplyAddress));
    mInitiatorLogicalAddress = rhs.mInitiatorLogicalAddress;
    mExtendedAddress = rhs.mExtendedAddress;
    mTransactionIdentifier = rhs.mTransactionIdentifier;
    mAddress = rhs.mAddress;
    mDataLength = rhs.mDataLength;
    mStatus = rhs.mStatus;
    mHeaderLength = rhs.mHeaderLength;

    mData = rhs.mData;

    mHeaderCRC = rhs.mHeaderCRC;
    mDataCRC = rhs.mDataCRC;
    return *this;
}

//=============================================================================
void
RmapPacket::constructHeader(outpost::Serialize& stream)
{
    // Only handling command packets
    if (isCommandPacket())
    {
        for (uint8_t i = 0; i < mSpwTargetAddressLength; i++)
        {
            stream.store<uint8_t>(mSpwTargets[i]);
        }

        stream.store<uint8_t>(mTargetLogicalAddress);
        stream.store<uint8_t>(rmap::protocolIdentifier);
        stream.store<uint8_t>(mInstruction.getRaw());
        stream.store<uint8_t>(mDestKey);
        if (mInstruction.getReplyAddressLength() > InstructionField::zeroBytes)
        {
            for (size_t i = 0; i < mInstruction.getReplyAddressLength(); i++)
            {
                stream.store<uint32_t>(mReplyAddress[i]);
            }
        }

        stream.store<uint8_t>(mInitiatorLogicalAddress);
        stream.store<uint16_t>(mTransactionIdentifier);
        stream.store<uint8_t>(mExtendedAddress);
        stream.store<uint32_t>(mAddress);
        stream.store24(mDataLength);
    }

    mHeaderLength = stream.getPosition();

    mHeaderCRC = outpost::Crc8CcittReversed::calculate(
            outpost::Slice<uint8_t>::unsafe(stream.getPointer() + mSpwTargetAddressLength,
                                            stream.getPosition() - mSpwTargetAddressLength));
    stream.store<uint8_t>(mHeaderCRC);
}
