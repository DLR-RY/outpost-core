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

#ifndef OUTPOST_COMM_RMAP_NODE_H_
#define OUTPOST_COMM_RMAP_NODE_H_

#include <string.h>
#include <stdint.h>
#include <outpost/utils/bounded_array.h>


namespace outpost
{
namespace comm
{

//------------------------------------------------------------------------------
class RmapNode
{
public:
    // Maximum nodes is specified from the number of SpW target addresses that
    // can be accommodated into a single RMAP packet (see ECSS-E-ST-50-52C pg. 24)
    static const uint8_t maxNodes = 12;

    RmapNode() :
            mName(), mId(0)
    {
    }
    ~RmapNode()
    {

    }

    char*
    getName() const
    {
        return const_cast<char*>(mName);
    }

    uint8_t
    getId() const
    {
        return mId;
    }

    void
    setName(const char *name)
    {
        strcpy(mName, name);
    }

    void
    setID(uint8_t id)
    {
        mId = id;
    }

protected:
    char mName[20];
    uint8_t mId;
};

//------------------------------------------------------------------------------
class RmapTargetNode : public RmapNode
{

public:
    static const uint8_t DefaultLogicalAddress = 0xFE;
    static const uint8_t DefaultKey = 0x20;

    RmapTargetNode();
    RmapTargetNode(const char *name,
                   uint8_t id,
                   uint8_t numOftargetSpWAddr,
                   uint8_t *targetSpWAddr,
                   uint8_t numOfRplyAddr,
                   uint8_t *rplyAddress,
                   uint8_t targetLogicalAddress,
                   uint8_t initiatorLogicalAddress,
                   uint8_t key);
    ~RmapTargetNode();

    inline uint8_t
    getKey() const
    {
        return mKey;
    }

    inline outpost::BoundedArray<uint8_t>
    getReplyAddress()
    {
        return outpost::BoundedArray<uint8_t>(mReplyAddress, mNumReplyAddrLength);
    }

    inline uint8_t
    getTargetLogicalAddress() const
    {
        return mTargetLogicalAddress;
    }

    inline outpost::BoundedArray<uint8_t>
    getTargetSpaceWireAddress()
    {
        return outpost::BoundedArray<uint8_t>(mTargetSpaceWireAddress, mNumOfTargetSpaceWireAddress);
    }

    inline void
    setKey(uint8_t defaultKey)
    {
        mKey = defaultKey;
    }

    inline void
    setReplyAddress(outpost::BoundedArray<uint8_t> replyAddress)
    {
        memcpy(mReplyAddress, replyAddress.begin(),
                replyAddress.getNumberOfElements());
    }

    inline void
    setTargetLogicalAddress(uint8_t targetLogicalAddress)
    {
        mTargetLogicalAddress = targetLogicalAddress;
    }

    inline void
    setTargetSpaceWireAddress(outpost::BoundedArray<uint8_t> targetSpaceWireAddress)
    {
        memcpy(mTargetSpaceWireAddress, targetSpaceWireAddress.begin(),
                targetSpaceWireAddress.getNumberOfElements());
        mNumOfTargetSpaceWireAddress =
                targetSpaceWireAddress.getNumberOfElements();
    }

    inline void
    setInitiatorLogicalAddress(uint8_t initiatorLogicalAddress)
    {
        mIsInitiatorLogicalAddressSet = true;
        mInitiatorLogicalAddress = initiatorLogicalAddress;
    }

    inline void
    unsetInitiatorLogicalAddress()
    {
        mIsInitiatorLogicalAddressSet = false;
        mInitiatorLogicalAddress = 0xFE;
    }

    inline bool
    isInitiatorLogicalAddressSet()
    {
        return mIsInitiatorLogicalAddressSet;
    }

    inline uint8_t
    getInitiatorLogicalAddress()
    {
        return mInitiatorLogicalAddress;
    }

    void
    printTarget();

private:
    uint8_t mNumOfTargetSpaceWireAddress;
    uint8_t mTargetSpaceWireAddress[maxNodes];
    uint8_t mNumReplyAddrLength;
    uint8_t mReplyAddress[maxNodes];
    uint8_t mTargetLogicalAddress;
    uint8_t mInitiatorLogicalAddress;
    uint8_t mKey;

    bool mIsInitiatorLogicalAddressSet;
};

//------------------------------------------------------------------------------
class RmapTargetsList
{
public:
    RmapTargetsList();
    RmapTargetsList(outpost::BoundedArray<RmapTargetNode*> rmapTargetNodes);
    ~RmapTargetsList();

    /**
     * Add a predefined RMAP target node into the list of pointers. A node should
     * be statically defined which is not destroyed after any context switch.
     *
     * \param node
     *      Pointer to the RMAP target node specifications
     *
     * \return
     *      True: node added to the list. False: List is already full
     * */
    bool
    addTargetNode(RmapTargetNode* node);

    /**
     * Add a predefined RMAP target nodes into the list
     *
     * \param nodes
     *      Array object of pointers to the RMAP target nodes
     *
     * \return
     *      True: nodes added to the list. False: too many nodes supplied
     * */
    bool
    addTargetNodes(outpost::BoundedArray<RmapTargetNode*> nodes);

    /**
     * Get RMAP target node from the list
     *
     * \param name
     *      A simple string name of the node, specified earlier
     *
     * \return
     *      Pointer to the node if found, otherwise nullptr
     * */
    RmapTargetNode*
    getTargetNode(const char *name);

    /**
     * Get RMAP target node from the list
     *
     * \param name
     *      Target logical address
     *
     * \return
     *      Pointer to the node if found, otherwise nullptr
     * */
    RmapTargetNode*
    getTargetNode(uint8_t logicalAddress);

    inline uint8_t
    getSize() const
    {
        return mSize;
    }

    inline outpost::BoundedArray<RmapTargetNode*>
    getAllRMAPTargetNodes()
    {
        return outpost::BoundedArray<RmapTargetNode*>(mNodes, mSize);
    }

private:
    RmapTargetNode* mNodes[RmapNode::maxNodes];
    uint8_t mSize;
};
}
}

#endif /* OUTPOST_COMM_RMAP_NODE_H_ */
