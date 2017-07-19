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
class RmapPacket
{
public:

    struct Instruction
    {
        enum PacketType
        {
            replyPacket = 0, commandPacket = 1
        };

        enum Operation
        {
            read = 0, write = 1
        };

        enum ReplyAddressLength
        {
            zeroBytes = 0, fourBytes = 1, eigthBytes = 2, twelveBytes = 3
        };

        Instruction() :
                mField(0)
        {
        }
        ~Instruction()
        {
        }

        void
        setRead()
        {
            // bit5 operation
            outpost::BitAccess::set<uint8_t, 5>(mField,
                    static_cast<uint8_t>(read));
        }

        bool
        isRead()
        {
            // bit5 operation
            return (outpost::BitAccess::get<uint8_t, 5>(mField)
                    == static_cast<uint8_t>(read));
        }

        void
        setWrite()
        {
            // bit5 operation
            outpost::BitAccess::set<uint8_t, 5>(mField,
                    static_cast<uint8_t>(write));
        }

        bool
        isWrite()
        {
            // bit5 operation
            return (outpost::BitAccess::get<uint8_t, 5>(mField)
                    == static_cast<uint8_t>(write));
        }

        void
        setCommandPacket()
        {
            // bit7 & bit6 packet type
            outpost::BitAccess::set<uint8_t, 7, 6>(mField,
                    static_cast<uint8_t>(commandPacket));
        }

        bool
        isCommandPacket()
        {
            // bit7 & bit6 packet type
            return (outpost::BitAccess::get<uint8_t, 7, 6>(mField)
                    == static_cast<uint8_t>(commandPacket));
        }

        void
        setReplyPacket()
        {
            // bit7 & bit6 packet type
            outpost::BitAccess::set<uint8_t, 7, 6>(mField,
                    static_cast<uint8_t>(replyPacket));
        }

        bool
        isReplyPacket()
        {
            // bit7 & bit6 packet type
            return (outpost::BitAccess::get<uint8_t, 7, 6>(mField)
                    == static_cast<uint8_t>(replyPacket));
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
        setReplyAddrLength(uint8_t len)
        {
            // bit 1 & bit 0 reply length
            outpost::BitAccess::set<uint8_t, 1, 0>(mField, len);
        }

        uint8_t
        getReplyAddrLength() const
        {
            return outpost::BitAccess::get<uint8_t, 1, 0>(mField);
        }

        void
        setInstruction(uint8_t inst)
        {
            mField = inst;
        }

        uint8_t
        getInstruction() const
        {
            return mField;
        }

        void
        reset()
        {
            mField = 0;
        }
    private:
        // disable copy constructor
        Instruction(const Instruction&);

        // disable copy-assignment operator
        Instruction&
        operator=(const Instruction&);

        uint8_t mField;
    };

    RmapPacket();
    RmapPacket(outpost::BoundedArray<uint8_t> spwTargets,
               uint8_t targetLogicalAddress,
               Instruction::ReplyAddressLength rplyAddrLen,
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

    void
    reset();

    bool
    constructPacket(outpost::BoundedArray<uint8_t> buffer,
                    outpost::BoundedArray<uint8_t> data);

    bool
    extractPacket(outpost::BoundedArray<const uint8_t> &data, uint8_t ila);

    void
    setTargetInformation(RmapTargetNode *rmapTargetNode);

    void
    getData(uint8_t *buffer, size_t maxLength);

    //--------------------------------------------------------------------------
    inline RmapPacket&
    operator=(const RmapPacket& rhs)
    {
        mNumOfSpwTargets = rhs.mNumOfSpwTargets;
        memcpy(mSpwTargets, rhs.mSpwTargets, sizeof(mSpwTargets));
        mTargetLogicalAddress = rhs.mTargetLogicalAddress;
        mInstruction.setInstruction(rhs.mInstruction.getInstruction());
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
                mInstruction.getReplyAddrLength());
    }

    inline void
    setReplyPathAddressLength(uint8_t pathAddressLength)
    {
        mInstruction.setReplyAddrLength(pathAddressLength);
    }

    inline uint8_t
    getReplyPathAddressLength()
    {
        return mInstruction.getReplyAddrLength();
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
        return mInstruction.getInstruction();
    }

    inline bool
    isCommandPacket()
    {
        return mInstruction.isCommandPacket();
    }

    inline void
    setCommand()
    {
        mInstruction.setCommandPacket();
    }

    inline void
    setReply()
    {
        mInstruction.setReplyPacket();
    }

    inline bool
    isReplyPacket()
    {
        return mInstruction.isReplyPacket();
    }

    inline bool
    isWrite()
    {
        return mInstruction.isWrite();
    }

    inline void
    setWrite()
    {
        mInstruction.setWrite();
    }

    inline bool
    isRead()
    {
        return mInstruction.isRead();
    }

    inline void
    setRead()
    {
        mInstruction.setRead();
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

    void
    constructHeader(outpost::Serialize &stream);

    uint8_t mNumOfSpwTargets;
    uint8_t mSpwTargets[32];
    uint8_t mTargetLogicalAddress;
    Instruction mInstruction;
    uint8_t mDestKey;
    uint32_t mReplyAddress[3];
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
