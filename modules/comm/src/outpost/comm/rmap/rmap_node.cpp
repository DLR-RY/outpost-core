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

#include "rmap_status.h"
#include "rmap_packet.h"
#include "rmap_node.h"


using namespace outpost::comm;

//------------------------------------------------------------------------------
RmapTargetNode::RmapTargetNode() :
        mTargetSpaceWireAddressLength(0), mReplyAddressLength(0),
        mTargetLogicalAddress(defaultLogicalAddress), mKey(0), mId(0)
{
    strcpy(mName, "Default");
    memset(mTargetSpaceWireAddress, 0, sizeof(mTargetSpaceWireAddress));
    memset(mReplyAddress, 0, sizeof(mReplyAddress));
}

RmapTargetNode::RmapTargetNode(const char *name,
                               uint8_t id,
                               outpost::BoundedArray<uint8_t> spwTargets,
                               outpost::BoundedArray<uint8_t> replyAddress,
                               uint8_t targetLogicalAddress,
                               uint8_t key) :
        mTargetSpaceWireAddressLength(spwTargets.getNumberOfElements()),
        mReplyAddressLength(replyAddress.getNumberOfElements()),
        mTargetLogicalAddress(targetLogicalAddress), mKey(key), mId(id)
{
    if(strlen(name) < maxNodeNameLength)
    {
        strcpy(mName, name);
    }
    else
    {
        strcpy(mName, "Default");
    }
    memcpy(mTargetSpaceWireAddress, spwTargets.begin(),
            spwTargets.getNumberOfElements());
    memcpy(mReplyAddress, replyAddress.begin(),
            replyAddress.getNumberOfElements());
}

RmapTargetNode::~RmapTargetNode()
{

}

//------------------------------------------------------------------------------
RmapTargetsList::RmapTargetsList() :
        mNodes(), mSize(0)
{

}

RmapTargetsList::~RmapTargetsList()
{

}

RmapTargetsList::RmapTargetsList(outpost::BoundedArray<RmapTargetNode*> rmapTargetNodes)
{
    addTargetNodes(rmapTargetNodes);
}

bool
RmapTargetsList::addTargetNode(RmapTargetNode* node)
{
    bool result = false;

    if (mSize < maxAddressLength)
    {
        mNodes[mSize++] = node;
        result = true;
    }
    return result;
}

bool
RmapTargetsList::addTargetNodes(outpost::BoundedArray<RmapTargetNode*> nodes)
{
    bool result = false;
    size_t listElements = nodes.getNumberOfElements();

    if (listElements <= mSize)
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
RmapTargetsList::getTargetNode(const char *name)
{
    RmapTargetNode* rt = nullptr;

    if (mSize != 0)
    {
        for (uint8_t i = 0; i < mSize; i++)
        {
            if (!strcmp(mNodes[i]->getName(), name))
            {
                rt = mNodes[i];
                break;
            }
        }
    }

    return rt;
}

RmapTargetNode*
RmapTargetsList::getTargetNode(uint8_t logicalAddress)
{
    RmapTargetNode* rt = NULL;

    if (mSize != 0)
    {
        for (uint8_t i = 0; i < mSize; i++)
        {
            if (mNodes[i]->getTargetLogicalAddress() == logicalAddress)
            {
                rt = mNodes[i];
                break;
            }
        }
    }
    else
    {
        console_out("Database empty\n");
    }

    if (!rt)
    {
        console_out("Error: No such RMAP target node\n");
    }

    return rt;
}
