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
 * - 2014-2017, Muhammad Bassam (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------
#include <outpost/utils/crc.h>

#include "rmap_packet.h"

using namespace outpost::comm;

RmapPacket::RmapPacket() :
        mNumOfSpwTargets(0), mTargetLogicalAddress(defaultLogicalAddress),
        mInstruction(), mDestKey(0),
        mInitiatorLogicalAddress(defaultLogicalAddress), mExtendedAddress(0),
        mTransactionIdentifier(0), mAddress(0), mDataLength(0), mStatus(0),
        mHeaderLength(0), mData(0), mHeaderCRC(0), mDataCRC(0)

{
    memset(mSpwTargets, 0, 32);
    memset(mReplyAddress, 0, 12);
}

RmapPacket::RmapPacket(outpost::BoundedArray<uint8_t> spwTargets,
                       uint8_t targetLogicalAddress,
                       Instruction::ReplyAddressLength rplyAddrLen,
                       uint8_t key,
                       uint8_t* replyAddress,
                       uint8_t initiatorLogicalAddress,
                       uint32_t address,
                       uint32_t dataLength) :
        mNumOfSpwTargets(spwTargets.getNumberOfElements()),
        mTargetLogicalAddress(targetLogicalAddress), mInstruction(),
        mDestKey(key), mInitiatorLogicalAddress(initiatorLogicalAddress),
        mExtendedAddress(0), mTransactionIdentifier(0), mAddress(address),
        mDataLength(dataLength), mStatus(0), mHeaderLength(0), mData(0),
        mHeaderCRC(0), mDataCRC(0)
{
    memcpy(mSpwTargets, spwTargets.begin(), spwTargets.getNumberOfElements());
    memcpy(mReplyAddress, replyAddress, rplyAddrLen * 4);
}

RmapPacket::RmapPacket(uint8_t targetLogicalAddress,
                       uint8_t key,
                       uint8_t initiatorLogicalAddress,
                       uint32_t address,
                       uint32_t dataLength) :
        mNumOfSpwTargets(0), mTargetLogicalAddress(targetLogicalAddress),
        mInstruction(), mDestKey(key),
        mInitiatorLogicalAddress(initiatorLogicalAddress), mExtendedAddress(0),
        mTransactionIdentifier(0), mAddress(address), mDataLength(dataLength),
        mStatus(0), mHeaderLength(0), mData(0), mHeaderCRC(0), mDataCRC(0)
{
    memset(mSpwTargets, 0, 32);
    memset(mReplyAddress, 0, 12);
}

RmapPacket::~RmapPacket()
{

}

void
RmapPacket::reset()
{
    memset(mSpwTargets, 0, 12);
    mTargetLogicalAddress = 0;
    mInstruction.reset();
    mDestKey = 0;
    memset(mReplyAddress, 0, 12);
    mInitiatorLogicalAddress = 0;
    mExtendedAddress = 0;
    mTransactionIdentifier = 0;
    mAddress = 0;
    mDataLength = 0;
    mStatus = RmapReplyStatus::commandExecutedSuccessfully;
    mHeaderCRC = 0;
    mDataCRC = 0;
}

bool
RmapPacket::constructPacket(outpost::BoundedArray<uint8_t> buffer,
                            outpost::BoundedArray<uint8_t> data)
{
    outpost::Serialize stream(buffer);

    console_out("RMAP-Packet: Constructing packet\n");

    // Construct header first
    constructHeader(stream);

    // Data length must be less then total buffer length supported by SpW driver
    if ((buffer.getNumberOfElements() - 4)
            < static_cast<size_t>(stream.getPosition() + mDataLength))
    {
        console_out(
                "RMAP-Packet: Trying to send larger packet than available buffer space\n");
        return false;
    }

    // Append data and CRC only if packet is write command
    if (isWrite())
    {
        mDataCRC = outpost::Crc8CcittReversed::calculate(data);

        console_out("DataCRC %x\n", mDataCRC);

        stream.storeBuffer(data.begin(), data.getNumberOfElements());
        stream.store<uint8_t>(mDataCRC);
    }

    return true;
}

bool
RmapPacket::extractPacket(outpost::BoundedArray<const uint8_t> &data,
                          uint8_t ila)
{

    if (data.getNumberOfElements() < minimumReplySize)
    {
        console_out("RMAP-Packet: packet size less then minimum\n");
        return false;
    }

    outpost::Deserialize stream(data);
    ptrdiff_t currentStreamPos;
    uint8_t packetHeaderCRC;
    uint8_t calculatedHeaderCRC;
    uint8_t *dataStartPointer;
    uint8_t calculatedDataCRC;
    uint8_t packetDataCRC;

    // Extract path addresses from the packet. Maximum SpW nodes supported in the
    // network is 32. So max value of the SpW node ID would be less then 32
    uint8_t spwPathAddr[32];
    uint8_t validSpWPathAddr = 0;

    memset(spwPathAddr, 0, 32);
    memset(mSpwTargets, 0, 32);

    for (uint8_t i = 0; i < 32; i++)
    {
        if (data[i] == ila)
        {
            console_out("ILA @ %u\n", i);
            break;
        }
        else if (data[i] < 32)
        {
            spwPathAddr[i] = stream.read<uint8_t>();
            validSpWPathAddr++;
        }
    }

    uint8_t initiatoraLogicalAddress = stream.read<uint8_t>();

    if (initiatoraLogicalAddress != ila)
    {
        console_out("RMAP-Packet: Initiator logical address doesn't match\n");
        return false;
    }

    uint8_t protocolIdentifiter = stream.read<uint8_t>();

    if (protocolIdentifiter != protocolIdentifier)
    {
        console_out("RMAP-Packet: Protocol ID is not RMAP\n");
        return false;
    }

    mInstruction.setInstruction(stream.read<uint8_t>());

    // If reply packet is received
    if (isReplyPacket())
    {
        memcpy(mReplyAddress, spwPathAddr, validSpWPathAddr);

        mInitiatorLogicalAddress = initiatoraLogicalAddress;
        mStatus = stream.read<uint8_t>();

        mTargetLogicalAddress = stream.read<uint8_t>();
        mTransactionIdentifier = stream.read<uint16_t>();

        // Write command reply
        if (mInstruction.isWrite())
        {
            currentStreamPos = stream.getPosition();
            packetHeaderCRC = stream.read<uint8_t>();

            calculatedHeaderCRC = outpost::Crc8CcittReversed::calculate(
                    outpost::BoundedArray<uint8_t>(
                            const_cast<uint8_t*>(stream.getPointer()),
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
                    outpost::BoundedArray<uint8_t>(
                            const_cast<uint8_t*>(stream.getPointer()),
                            static_cast<size_t>(currentStreamPos)));

            if (calculatedHeaderCRC != packetHeaderCRC)
            {
                console_out("RMAP-Packet: Invalid packet header CRC\n");
                return false;
            }
            mHeaderCRC = packetHeaderCRC;

            dataStartPointer =
                    const_cast<uint8_t*>(stream.getPointerToCurrentPosition());

            // Check data length
            if (static_cast<size_t>(stream.getPosition() + mDataLength + 1)
                    != data.getNumberOfElements())
            {
                console_out("RMAP-Packet: data length mismatch\n");
                return false;
            }

            // Skip the data byte to calculate data CRC
            stream.skip(mDataLength);

            packetDataCRC = stream.read<uint8_t>();

            calculatedDataCRC = outpost::Crc8CcittReversed::calculate(
                    outpost::BoundedArray<uint8_t>(dataStartPointer,
                            mDataLength));
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
RmapPacket::setTargetInformation(RmapTargetNode *rmapTargetNode)
{
    // Set packet target logical address field according to the RMAP target node
    mTargetLogicalAddress = rmapTargetNode->getTargetLogicalAddress();

    // Set packet reply address field according to the RMAP target node
    outpost::BoundedArray<uint8_t> rplyAddr = rmapTargetNode->getReplyAddress();
    memcpy(mReplyAddress, &rplyAddr[0], rplyAddr.getNumberOfElements());
    mInstruction.setReplyAddrLength(rplyAddr.getNumberOfElements() / 4);

    // Set packet target path SpW address field according to the RMAP target node
    setTargetSpaceWireAddress(rmapTargetNode->getTargetSpaceWireAddress());

    // Set packet key field according to the RMAP target node
    setKey(rmapTargetNode->getKey());

    // Set packet initiator logical address field according to the RMAP target node
    if (rmapTargetNode->isInitiatorLogicalAddressSet())
    {
        setInitiatorLogicalAddress(
                rmapTargetNode->getInitiatorLogicalAddress());
    }
}

void
RmapPacket::getData(uint8_t *buffer, size_t maxLength)
{
    size_t length = mDataLength;

    if (maxLength < length)
    {
        console_out("Insufficient buffer size\n");
    }
    else
    {
        for (uint32_t i = 0; i < length; i++)
        {
            buffer[i] = mData[i];
        }
    }
}

//=============================================================================
void
RmapPacket::constructHeader(outpost::Serialize &stream)
{
    if (isCommandPacket())
    {
        // Command packet

        for (uint8_t i = 0; i < mNumOfSpwTargets; i++)
            stream.store<uint8_t>(mSpwTargets[i]);

        stream.store<uint8_t>(mTargetLogicalAddress);
        stream.store<uint8_t>(protocolIdentifier);
        stream.store<uint8_t>(mInstruction.getInstruction());
        stream.store<uint8_t>(mDestKey);
        if (mInstruction.getReplyAddrLength() > 0)
        {
            for (size_t i = 0; i < mInstruction.getReplyAddrLength(); i++)
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
    else
    {
        // Reply packet
        for (size_t i = 0; i < (mInstruction.getReplyAddrLength() * 4); i++)
        {
            stream.store<uint32_t>(mReplyAddress[i]);
        }
        stream.store<uint8_t>(mInitiatorLogicalAddress);
        stream.store<uint8_t>(protocolIdentifier);
        stream.store<uint8_t>(mInstruction.getInstruction());
        stream.store<uint8_t>(mStatus);
        stream.store<uint8_t>(mTargetLogicalAddress);
        stream.store<uint16_t>(mTransactionIdentifier);

        if (isRead())
        {
            stream.store<uint8_t>(0);
            stream.store24(mDataLength);
        }
    }

    mHeaderLength = stream.getPosition();

    mHeaderCRC = outpost::Crc8CcittReversed::calculate(
            outpost::BoundedArray<uint8_t>(stream.getPointer(),
                    stream.getPosition()));
    stream.store<uint8_t>(mHeaderCRC);
}
