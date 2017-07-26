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
#ifndef OUTPOST_COMM_RMAP_PACKET_H_
#define OUTPOST_COMM_RMAP_PACKET_H_

#include <string>
#include <outpost/utils/bit_access.h>
#include <outpost/utils/bounded_array.h>
#include <outpost/utils/serialize.h>

#include <outpost/comm/rmap/rmap_node.h>
#include <outpost/comm/rmap/rmap_status.h>

namespace outpost
{
namespace comm
{

/**
 * RMAP packet.
 *
 * Provides RMAP packet definitions and methods to setup the send/receive packet
 * for RMAP transaction.
 *
 * \author  Muhammad Bassam
 */
class RmapPacket
{
public:

    /**
     * RMAP packet instruction field, for reference
     * (see ECSS-E-ST-50-52C pg. 17)
     *
     * */
    struct InstructionField
    {
        enum PacketType
            : uint8_t
            {
                replyPacket = 0, commandPacket = 1
        };

        enum Operation
            : uint8_t
            {
                read = 0, write = 1
        };

        enum ReplyAddressLength
            : uint8_t
            {
                zeroBytes = 0, fourBytes = 1, eigthBytes = 2, twelveBytes = 3
        };

        constexpr
        InstructionField() :
                mField(0)
        {
        }
        ~InstructionField()
        {
        }

        void
        setOperation(Operation op)
        {
            // bit5 operation
            outpost::BitAccess::set<uint8_t, 5>(mField, op);
        }

        Operation
        getOperation() const
        {
            // bit5 operation
            return static_cast<Operation>(outpost::BitAccess::get<uint8_t, 5>(mField));
        }

        void
        setPacketType(PacketType type)
        {
            // bit7 & bit6 packet type
            outpost::BitAccess::set<uint8_t, 7, 6>(mField, type);
        }

        PacketType
        getPacketType() const
        {
            // bit7 & bit6 packet type
            return static_cast<PacketType>(outpost::BitAccess::get<uint8_t, 7, 6>(mField));
        }

        void
        enableVerify()
        {
            // bit4 verify
            outpost::BitAccess::set<uint8_t, 4>(mField, 1);
        }

        void
        disableVerify()
        {
            // bit4 verify
            outpost::BitAccess::set<uint8_t, 4>(mField, 0);
        }

        void
        enableReply()
        {
            // bit3 reply
            outpost::BitAccess::set<uint8_t, 3>(mField, 1);
        }

        void
        disableReply()
        {
            // bit3 reply
            outpost::BitAccess::set<uint8_t, 3>(mField, 0);
        }

        void
        enableIncrement()
        {
            // bit2 increment
            outpost::BitAccess::set<uint8_t, 2>(mField, 1);
        }

        void
        disableIncrement()
        {
            // bit2 increment
            outpost::BitAccess::set<uint8_t, 2>(mField, 0);
        }

        bool
        isVerifyEnabled()
        {
            // bit4 verify
            return outpost::BitAccess::get<uint8_t, 4>(mField);
        }

        bool
        isReplyEnabled()
        {
            // bit3 reply
            return outpost::BitAccess::get<uint8_t, 3>(mField);
        }

        bool
        isIncrementEnabled()
        {
            // bit2 increment
            return outpost::BitAccess::get<uint8_t, 2>(mField);
        }

        void
        setReplyAddressLength(ReplyAddressLength len)
        {
            // bit 1 & bit 0 reply length
            outpost::BitAccess::set<uint8_t, 1, 0>(mField, len);
        }

        ReplyAddressLength
        getReplyAddressLength() const
        {
            return static_cast<ReplyAddressLength>(outpost::BitAccess::get<
                    uint8_t, 1, 0>(mField));
        }

        void
        setAllRaw(uint8_t inst)
        {
            mField = inst;
        }

        uint8_t
        getRaw() const
        {
            return mField;
        }

        void
        reset()
        {
            mField = 0;
        }

        // disable copy constructor
        InstructionField(const InstructionField&) = delete;

        // disable copy-assignment operator
        InstructionField&
        operator=(const InstructionField&) = delete;
    private:
        uint8_t mField;
    };

    //--------------------------------------------------------------------------
    RmapPacket();
    RmapPacket(outpost::BoundedArray<uint8_t> spwTargets,
               uint8_t targetLogicalAddress,
               InstructionField::ReplyAddressLength rplyAddrLen,
               uint8_t key,
               uint8_t* replyAddress,
               uint8_t initiatorLogicalAddress,
               uint32_t address,
               uint32_t dataLength);

    RmapPacket(uint8_t targetLogicalAddress,
               uint8_t key,
               uint8_t initiatorLogicalAddress,
               uint32_t address,
               uint32_t dataLength);
    ~RmapPacket();

    /**
     * Reset or clear the contents of the packet. This method is being
     * used by the RMAP initiator thread for clearing the received old packet
     * contents for next incoming packets.
     *
     * */
    void
    reset();

    /**
     * Construct the RMAP packet according to the given standard. After filling
     * the packet content corresponding CRC will be calculated and inserted into
     * the packet buffer making it ready to send.
     *
     * \param buffer
     *      SpW buffer provided by the RMAP initiator
     *
     * \param data
     *      Reference to the user data for write commands, for read commands
     *      this object will be outpost::BoundedArray<uint8_t>::empty() and will
     *      be ignored
     *
     * \return
     *      True for successful integration of packet into the buffer, false for
     *      insufficient memory space in the buffer
     *
     * */
    bool
    constructPacket(outpost::BoundedArray<uint8_t> buffer,
                    outpost::BoundedArray<uint8_t> &data);

    /**
     * Extract the received RMAP packet according to the given standard by
     * checking it's content and verifying particular CRC's for packet data
     * validity checks.
     *
     * \param data
     *      Reference to the raw received data buffer from SpW
     *
     * \param initiatorLogicalAddress
     *      Used to validating if the packet is intended for the provided packet
     *      initiator
     *
     * \return
     *      True for valid RMAP packet and its extraction, otherwise false
     *
     * */
    bool
    extractPacket(outpost::BoundedArray<const uint8_t> &data,
                  uint8_t initiatorLogicalAddress);

    /**
     * Setting the RMAP target specific information into the packet, will be
     * done before initiating the transaction for sending commands.
     *
     * \param rmapTargetNode
     *      Reference to the RMAP target node
     *
     * */
    void
    setTargetInformation(RmapTargetNode &rmapTargetNode);

    RmapPacket&
    operator=(const RmapPacket& rhs);

    // Disabling copy constructor
    RmapPacket(const RmapPacket&) = delete;

    //--------------------------------------------------------------------------
    inline void
    setTargetSpaceWireAddress(outpost::BoundedArray<uint8_t> targetSpaceWireAddress)
    {
        memcpy(mSpwTargets, targetSpaceWireAddress.begin(),
                targetSpaceWireAddress.getNumberOfElements());
        mNumOfSpwTargets = targetSpaceWireAddress.getNumberOfElements();
    }

    inline outpost::BoundedArray<uint8_t>
    getTargetSpaceWireAddress()
    {
        return outpost::BoundedArray<uint8_t>(mSpwTargets, mNumOfSpwTargets);
    }

    inline outpost::BoundedArray<uint8_t>
    getReplyAddress()
    {
        return outpost::BoundedArray<uint8_t>(
                reinterpret_cast<uint8_t*>(mReplyAddress),
                mInstruction.getReplyAddressLength());
    }

    inline void
    setReplyPathAddressLength(InstructionField::ReplyAddressLength pathAddressLength)
    {
        mInstruction.setReplyAddressLength(pathAddressLength);
    }

    inline uint8_t
    getReplyPathAddressLength()
    {
        return mInstruction.getReplyAddressLength();
    }

    inline void
    setTargetLogicalAddress(uint8_t targetLogicalAddress)
    {
        mTargetLogicalAddress = targetLogicalAddress;
    }

    inline uint8_t
    getTargetLogicalAddress() const
    {
        return mTargetLogicalAddress;
    }

    inline void
    setInitiatorLogicalAddress(uint8_t initiatorLogicalAddress)
    {
        mInitiatorLogicalAddress = initiatorLogicalAddress;
    }

    inline uint8_t
    getInitiatorLogicalAddress() const
    {
        return mInitiatorLogicalAddress;
    }

    inline uint8_t
    getInstruction() const
    {
        return mInstruction.getRaw();
    }

    inline bool
    isCommandPacket()
    {
        return (mInstruction.getPacketType() == InstructionField::commandPacket);
    }

    inline void
    setCommand()
    {
        //mInstruction.setCommandPacket();
        mInstruction.setPacketType(InstructionField::commandPacket);
    }

    inline void
    setReply()
    {
        //mInstruction.setReplyPacket();
        mInstruction.setPacketType(InstructionField::replyPacket);
    }

    inline bool
    isReplyPacket()
    {
        //return mInstruction.isReplyPacket();
        return (mInstruction.getPacketType() == InstructionField::replyPacket);
    }

    inline bool
    isWrite()
    {
        //return mInstruction.isWrite();
        return (mInstruction.getOperation() == InstructionField::write);
    }

    inline void
    setWrite()
    {
        mInstruction.setOperation(InstructionField::write);
    }

    inline bool
    isRead()
    {
        //return mInstruction.isRead();
        return (mInstruction.getOperation() == InstructionField::read);
    }

    inline void
    setRead()
    {
        mInstruction.setOperation(InstructionField::read);
    }

    inline bool
    isVerifyFlagSet()
    {
        return mInstruction.isVerifyEnabled();
    }

    inline void
    setVerifyFlag()
    {
        mInstruction.enableVerify();
    }

    inline void
    unsetVerifyFlag()
    {
        mInstruction.disableVerify();
    }

    inline bool
    isReplyFlagSet()
    {
        return mInstruction.isReplyEnabled();
    }

    inline void
    setReplyFlag()
    {
        mInstruction.enableReply();
    }

    inline void
    unsetReplyFlag()
    {
        mInstruction.disableReply();
    }

    inline bool
    isIncrementFlagSet()
    {
        return mInstruction.isIncrementEnabled();
    }

    inline void
    setIncrementFlag()
    {
        mInstruction.enableIncrement();
    }

    inline void
    unsetIncrementFlag()
    {
        mInstruction.disableIncrement();
    }

    inline void
    setStatus(uint8_t status)
    {
        mStatus = status;
    }

    inline uint8_t
    getStatus() const
    {
        return mStatus;
    }

    inline void
    setTransactionID(uint16_t transactionID)
    {
        mTransactionIdentifier = transactionID;
    }

    inline uint16_t
    getTransactionID() const
    {
        return mTransactionIdentifier;
    }

    inline void
    setAddress(uint32_t address)
    {
        mAddress = address;
    }

    inline uint32_t
    getAddress() const
    {
        return mAddress;
    }

    inline void
    setExtendedAddress(uint8_t extendedAddress)
    {
        mExtendedAddress = extendedAddress;
    }

    inline uint8_t
    getExtendedAddress() const
    {
        return mExtendedAddress;
    }

    inline void
    setKey(uint8_t key)
    {
        mDestKey = key;
    }

    inline uint8_t
    getKey() const
    {
        return mDestKey;
    }

    inline uint32_t
    getHeaderLength()
    {
        return mHeaderLength;
    }

    inline uint8_t
    getHeaderCRC() const
    {
        return mHeaderCRC;
    }

    inline void
    setData(uint8_t *buffer)
    {
        if (buffer)
        {
            mData = buffer;
        }
    }
    inline uint8_t *
    getData() const
    {
        return mData;
    }

    inline uint8_t
    getDataCRC() const
    {
        return mDataCRC;
    }

    inline void
    setDataLength(uint32_t len)
    {
        mDataLength = len;
    }

    inline uint32_t
    getDataLength() const
    {
        return mDataLength;
    }

private:

    /**
     * Construct RMAP packet header.
     *
     * \param stream
     *      Reference to the serialize stream of SpW buffer
     *
     * */
    void
    constructHeader(outpost::Serialize &stream);

    //--------------------------------------------------------------------------
    uint8_t mNumOfSpwTargets;
    uint8_t mSpwTargets[rmap::maxPhysicalRouterOutputPorts];
    uint8_t mTargetLogicalAddress;
    InstructionField mInstruction;
    uint8_t mDestKey;
    uint32_t mReplyAddress[rmap::maxAddressLength/4];
    uint8_t mInitiatorLogicalAddress;
    uint8_t mExtendedAddress;
    uint16_t mTransactionIdentifier;
    uint32_t mAddress;
    uint32_t mDataLength;
    uint8_t mStatus;
    uint32_t mHeaderLength;
    uint8_t *mData;

    uint8_t mHeaderCRC;
    uint8_t mDataCRC;
};
}
}

#endif
