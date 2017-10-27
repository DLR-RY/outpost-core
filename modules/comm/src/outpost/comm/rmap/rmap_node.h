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
/**
 * RMAP target node.
 *
 * Provides the RMAP object level information for the listed RMAP targets.
 *
 * \author  Muhammad Bassam
 */
class RmapTargetNode
{
public:
    RmapTargetNode();
    RmapTargetNode(const char* name,
                   uint8_t id,
                   uint8_t targetLogicalAddress,
                   uint8_t key);
    ~RmapTargetNode();

    /**
     * Sets the reply address of the target to the given value
     *
     * \param replyAddress
     *      Reply address to be set
     *
     * \return
     *      True for successful, false for wrong size parameter
     *
     * */
    bool
    setReplyAddress(outpost::BoundedArray<uint8_t> replyAddress);

    /**
     * Sets the SpW target addresses to the given value
     *
     * \param targetSpaceWireAddress
     *      SpW addresses to be set
     *
     * \return
     *      True for successful, false for wrong size parameter
     *
     * */
    bool
    setTargetSpaceWireAddress(outpost::BoundedArray<uint8_t> targetSpaceWireAddress);

    //--------------------------------------------------------------------------
    inline uint8_t
    getKey() const
    {
        return mKey;
    }

    inline outpost::BoundedArray<uint8_t>
    getReplyAddress()
    {
        return outpost::BoundedArray<uint8_t>(mReplyAddress, mReplyAddressLength);
    }

    inline uint8_t
    getTargetLogicalAddress() const
    {
        return mTargetLogicalAddress;
    }

    inline outpost::BoundedArray<uint8_t>
    getTargetSpaceWireAddress()
    {
        return outpost::BoundedArray<uint8_t>(mTargetSpaceWireAddress, mTargetSpaceWireAddressLength);
    }

    inline void
    setKey(uint8_t defaultKey)
    {
        mKey = defaultKey;
    }

    inline void
    setTargetLogicalAddress(uint8_t targetLogicalAddress)
    {
        mTargetLogicalAddress = targetLogicalAddress;
    }

    inline const char*
    getName() const
    {
        return mName;
    }

    inline uint8_t
    getId() const
    {
        return mId;
    }

private:
    uint8_t mTargetSpaceWireAddressLength;
    uint8_t mTargetSpaceWireAddress[rmap::maxAddressLength];
    uint8_t mReplyAddressLength;
    uint8_t mReplyAddress[rmap::maxAddressLength];
    uint8_t mTargetLogicalAddress;
    uint8_t mKey;
    char mName[rmap::maxNodeNameLength];
    uint8_t mId;
};

//------------------------------------------------------------------------------
/**
 * RMAP target node list.
 *
 * Provides the list for RMAP targets to be used by the RMAP initiator class.
 *
 * \author  Muhammad Bassam
 */
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
    getTargetNode(const char* name);

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

    //--------------------------------------------------------------------------
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
    RmapTargetNode* mNodes[rmap::maxAddressLength];
    uint8_t mSize;
};
}
}

#endif /* OUTPOST_COMM_RMAP_NODE_H_ */
