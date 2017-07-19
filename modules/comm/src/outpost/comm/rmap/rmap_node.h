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

#ifndef OUTPOST_COMM_RMAP_NODE_H_
#define OUTPOST_COMM_RMAP_NODE_H_

#include <string.h>
#include <stdint.h>
#include <outpost/utils/bounded_array.h>

#include "rmap_common.h"

namespace outpost
{
namespace comm
{

//------------------------------------------------------------------------------
class RmapTargetNode
{
public:

    RmapTargetNode();
    RmapTargetNode(const char *name,
                   uint8_t id,
                   outpost::BoundedArray<uint8_t> spwTargets,
                   outpost::BoundedArray<uint8_t> replyAddress,
                   uint8_t targetLogicalAddress,
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
        return outpost::BoundedArray<uint8_t>(mReplyAddress,
                mReplyAddressLength);
    }

    inline uint8_t
    getTargetLogicalAddress() const
    {
        return mTargetLogicalAddress;
    }

    inline outpost::BoundedArray<uint8_t>
    getTargetSpaceWireAddress()
    {
        return outpost::BoundedArray<uint8_t>(mTargetSpaceWireAddress,
                mTargetSpaceWireAddressLength);
    }

    inline void
    setKey(uint8_t defaultKey)
    {
        mKey = defaultKey;
    }

    inline bool
    setReplyAddress(outpost::BoundedArray<uint8_t> replyAddress)
    {
        bool result = false;
        if (replyAddress.getNumberOfElements() <= maxAddressLength)
        {
            memcpy(mReplyAddress, replyAddress.begin(),
                replyAddress.getNumberOfElements());
            result = true;
        }
        return result;
    }

    inline void
    setTargetLogicalAddress(uint8_t targetLogicalAddress)
    {
        mTargetLogicalAddress = targetLogicalAddress;
    }

    inline bool
    setTargetSpaceWireAddress(outpost::BoundedArray<uint8_t> targetSpaceWireAddress)
    {
        bool result = false;
        if (targetSpaceWireAddress.getNumberOfElements() <= maxAddressLength)
        {
            memcpy(mTargetSpaceWireAddress, targetSpaceWireAddress.begin(),
                    targetSpaceWireAddress.getNumberOfElements());
            mTargetSpaceWireAddressLength =
                    targetSpaceWireAddress.getNumberOfElements();
            result = true;
        }
        return result;
    }

    const char*
    getName() const
    {
        return mName;
    }

    uint8_t
    getId() const
    {
        return mId;
    }

private:
    uint8_t mTargetSpaceWireAddressLength;
    uint8_t mTargetSpaceWireAddress[maxAddressLength];
    uint8_t mReplyAddressLength;
    uint8_t mReplyAddress[maxAddressLength];
    uint8_t mTargetLogicalAddress;
    uint8_t mKey;

    char mName[maxNodeNameLength];
    uint8_t mId;
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
    getTargetNodes()
    {
        return outpost::BoundedArray<RmapTargetNode*>(mNodes, mSize);
    }

private:
    RmapTargetNode* mNodes[maxAddressLength];
    uint8_t mSize;
};
}
}

#endif /* OUTPOST_COMM_RMAP_NODE_H_ */
