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
#include "rmap_status.h"
#include "rmap_packet.h"
#include "rmap_node.h"

using namespace outpost::comm;

//------------------------------------------------------------------------------
RmapTargetNode::RmapTargetNode() :
    mTargetSpaceWireAddressLength(0),
    mTargetSpaceWireAddress(),
    mReplyAddressLength(0),
    mReplyAddress(),
    mTargetLogicalAddress(rmap::defaultLogicalAddress),
    mKey(0),
    mId(0)
{
    strcpy(mName, "Default");
    memset(mTargetSpaceWireAddress, 0, sizeof(mTargetSpaceWireAddress));
    memset(mReplyAddress, 0, sizeof(mReplyAddress));
}

RmapTargetNode::RmapTargetNode(const char* name,
                               uint8_t id,
                               uint8_t targetLogicalAddress,
                               uint8_t key) :
    mTargetSpaceWireAddressLength(0),
    mReplyAddressLength(0),
    mTargetLogicalAddress(targetLogicalAddress),
    mKey(key),
    mId(id)
{
    if (strlen(name) < rmap::maxNodeNameLength)
    {
        strcpy(mName, name);
    }
    else
    {
        strcpy(mName, "Default");
    }

    memset(mTargetSpaceWireAddress, 0, sizeof(mTargetSpaceWireAddress));
    memset(mReplyAddress, 0, sizeof(mReplyAddress));
}

RmapTargetNode::~RmapTargetNode()
{

}

bool
RmapTargetNode::setReplyAddress(outpost::Slice<uint8_t> replyAddress)
{
    bool result = false;
    if (replyAddress.getNumberOfElements() <= sizeof(mReplyAddress))
    {
        memcpy(mReplyAddress, replyAddress.begin(), replyAddress.getNumberOfElements());
        result = true;
    }
    return result;
}

bool
RmapTargetNode::setTargetSpaceWireAddress(outpost::Slice<uint8_t> targetSpaceWireAddress)
{
    bool result = false;
    if (targetSpaceWireAddress.getNumberOfElements() <= sizeof(mTargetSpaceWireAddress))
    {
        memcpy(mTargetSpaceWireAddress, targetSpaceWireAddress.begin(), targetSpaceWireAddress.getNumberOfElements());
        mTargetSpaceWireAddressLength = targetSpaceWireAddress.getNumberOfElements();
        result = true;
    }
    return result;
}

//------------------------------------------------------------------------------
RmapTargetsList::RmapTargetsList() :
    mNodes(),
    mSize(0)
{

}

RmapTargetsList::~RmapTargetsList()
{

}

RmapTargetsList::RmapTargetsList(outpost::Slice<RmapTargetNode*> rmapTargetNodes)
{
    if (rmapTargetNodes.getNumberOfElements() <= rmap::maxAddressLength)
    {
        mSize = rmapTargetNodes.getNumberOfElements();
        addTargetNodes(rmapTargetNodes);
    }
}

bool
RmapTargetsList::addTargetNode(RmapTargetNode* node)
{
    bool result = false;

    if (mSize < rmap::maxAddressLength)
    {
        mNodes[mSize++] = node;
        result = true;
    }
    return result;
}

bool
RmapTargetsList::addTargetNodes(outpost::Slice<RmapTargetNode*> nodes)
{
    bool result = false;
    size_t listElements = nodes.getNumberOfElements();

    if ((listElements + mSize) < rmap::maxAddressLength)
    {
        for (size_t i = 0; i < listElements; i++)
        {
            addTargetNode(nodes[i]);
        }
        result = true;
    }
    return result;
}

RmapTargetNode*
RmapTargetsList::getTargetNode(const char* name)
{
    RmapTargetNode* rt = nullptr;

    for (uint8_t i = 0; i < mSize; i++)
    {
        if (!strncmp(mNodes[i]->getName(), name, rmap::maxNodeNameLength))
        {
            rt = mNodes[i];
            break;
        }
    }

    return rt;
}

RmapTargetNode*
RmapTargetsList::getTargetNode(uint8_t logicalAddress)
{
    RmapTargetNode* rt = nullptr;

    for (uint8_t i = 0; i < mSize; i++)
    {
        if (mNodes[i]->getTargetLogicalAddress() == logicalAddress)
        {
            rt = mNodes[i];
            break;
        }
    }

    if (!rt)
    {
        console_out("Error: No such RMAP target node\n");
    }

    return rt;
}
