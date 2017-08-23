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
// ----------------------------------------------------------------------------
#include <outpost/utils/crc.h>

#include "rmap_packet.h"

using namespace outpost::comm;

RmapPacket::RmapPacket() :
    mNumOfSpwTargets(0),
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
    mData(0),
    mHeaderCRC(0),
    mDataCRC(0)

{
    memset(mSpwTargets, 0, sizeof(mSpwTargets));
    memset(mReplyAddress, 0, sizeof(mReplyAddress));
}

RmapPacket::RmapPacket(outpost::BoundedArray<uint8_t> spwTargets,
                       uint8_t targetLogicalAddress,
                       InstructionField::ReplyAddressLength rplyAddrLen,
                       uint8_t key,
                       uint8_t* replyAddress,
                       uint8_t initiatorLogicalAddress,
                       uint32_t address,
                       uint32_t dataLength) :
    mNumOfSpwTargets(spwTargets.getNumberOfElements()),
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
    mData(0),
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
    mNumOfSpwTargets(0),
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
    mData(0),
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

bool
RmapPacket::constructPacket(outpost::BoundedArray<uint8_t> buffer, outpost::BoundedArray<const uint8_t>& data)
{
    outpost::Serialize stream(buffer);

    // Construct header first
    constructHeader(stream);

    // Data length must be less then total buffer length supported by SpW driver
    if ((buffer.getNumberOfElements() - 4) < static_cast<size_t>(stream.getPosition() + mDataLength))
    {
        console_out(
            "RMAP-Packet: Trying to send larger packet than available buffer space\n");
        return false;
    }

    // Append data and CRC only if packet is write command
    if (isWrite())
    {
        mDataCRC = outpost::Crc8CcittReversed::calculate(data);
        mData = const_cast<uint8_t*>(data.begin());

        stream.store(data);
        stream.store<uint8_t>(mDataCRC);
    }

    return true;
}

bool
RmapPacket::extractPacket(outpost::BoundedArray<const uint8_t>& data, uint8_t initiatorLogicalAddress)
{

    if (data.getNumberOfElements() < rmap::minimumReplySize)
    {
        console_out("RMAP-Packet: packet size less then minimum\n");
        return false;
    }

    outpost::Deserialize stream(data);
    ptrdiff_t currentStreamPos;
    uint8_t packetHeaderCRC;
    uint8_t calculatedHeaderCRC;
    uint8_t* dataStartPointer;
    uint8_t calculatedDataCRC;
    uint8_t packetDataCRC;

    // Extract path addresses from the packet. Maximum SpW nodes supported in the
    // network is 32. So max value of the SpW node ID would be less then 32
    uint8_t spwPathAddr[rmap::maxPhysicalRouterOutputPorts];
    uint8_t validSpWPathAddr = 0;

    memset(spwPathAddr, 0, sizeof(spwPathAddr));
    memset(mSpwTargets, 0, sizeof(mSpwTargets));

    for (uint8_t i = 0; i < rmap::maxPhysicalRouterOutputPorts; i++)
    {
        if (data[i] == initiatorLogicalAddress)
        {
            break;
        }
        else if (data[i] < 32)
        {
            spwPathAddr[i] = stream.read<uint8_t>();
            validSpWPathAddr++;
        }
    }

    uint8_t initiatoraLogicalAddress = stream.read<uint8_t>();

    if (initiatoraLogicalAddress != initiatorLogicalAddress)
    {
        console_out("RMAP-Packet: Initiator logical address doesn't match\n");
        return false;
    }

    uint8_t protocolIdentifiter = stream.read<uint8_t>();

    if (protocolIdentifiter != rmap::protocolIdentifier)
    {
        console_out("RMAP-Packet: Protocol ID is not RMAP\n");
        return false;
    }

    mInstruction.setAllRaw(stream.read<uint8_t>());

    // If reply packet is received
    if (isReplyPacket())
    {
        memcpy(mReplyAddress, spwPathAddr, validSpWPathAddr);

        mInitiatorLogicalAddress = initiatoraLogicalAddress;
        mStatus = stream.read<uint8_t>();

        mTargetLogicalAddress = stream.read<uint8_t>();
        mTransactionIdentifier = stream.read<uint16_t>();

        // Write command reply
        if (isWrite())
        {
            currentStreamPos = stream.getPosition();
            packetHeaderCRC = stream.read<uint8_t>();

            calculatedHeaderCRC = outpost::Crc8CcittReversed::calculate(
                outpost::BoundedArray<uint8_t>(const_cast<uint8_t*>(stream.getPointer()),
                    static_cast<size_t>(currentStreamPos)));

            if (calculatedHeaderCRC != packetHeaderCRC)
            {
                console_out("RMAP-Packet: invalid packet header CRC\n");
                return false;
            }
            mHeaderCRC = packetHeaderCRC;
        }
        // Read command reply
        else
        {
            // Skip the reserved byte
            stream.skip(1);

            // Read data length
            mDataLength = stream.read24();

            // Get the header CRC
            currentStreamPos = stream.getPosition();
            packetHeaderCRC = stream.read<uint8_t>();

            calculatedHeaderCRC = outpost::Crc8CcittReversed::calculate(
                outpost::BoundedArray<uint8_t>(const_cast<uint8_t*>(stream.getPointer()),
                    static_cast<size_t>(currentStreamPos)));

            if (calculatedHeaderCRC != packetHeaderCRC)
            {
                console_out("RMAP-Packet: Invalid packet header CRC\n");
                return false;
            }
            mHeaderCRC = packetHeaderCRC;

            dataStartPointer = const_cast<uint8_t*>(stream.getPointerToCurrentPosition());

            // Check data length
            if (static_cast<size_t>(stream.getPosition() + mDataLength + 1) != data.getNumberOfElements())
            {
                console_out("RMAP-Packet: data length mismatch\n");
                return false;
            }

            // Skip the data byte to calculate data CRC
            stream.skip(mDataLength);

            packetDataCRC = stream.read<uint8_t>();

            calculatedDataCRC = outpost::Crc8CcittReversed::calculate(
                outpost::BoundedArray<uint8_t>(dataStartPointer, mDataLength));
            if (packetDataCRC != calculatedDataCRC)
            {
                console_out("RMAP-Packet: Invalid packet data CRC\n");
                return false;
            }
            mDataCRC = packetDataCRC;
            mData = dataStartPointer;
        }
    }

    return true;
}

void
RmapPacket::setTargetInformation(RmapTargetNode& rmapTargetNode)
{
    // Set packet target logical address field according to the RMAP target node
    mTargetLogicalAddress = rmapTargetNode.getTargetLogicalAddress();

    // Set packet reply address field according to the RMAP target node
    outpost::BoundedArray<uint8_t> rplyAddr = rmapTargetNode.getReplyAddress();
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
    mNumOfSpwTargets = rhs.mNumOfSpwTargets;
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

        for (uint8_t i = 0; i < mNumOfSpwTargets; i++)
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
        outpost::BoundedArray<uint8_t>(stream.getPointer(), stream.getPosition()));
    stream.store<uint8_t>(mHeaderCRC);
}
