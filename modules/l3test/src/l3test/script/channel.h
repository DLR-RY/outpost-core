/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef L3TEST_SCRIPT_CHANNEL_H
#define L3TEST_SCRIPT_CHANNEL_H

#include <vector>
#include <list>
#include <memory>

#include <stddef.h>
#include <stdint.h>

namespace l3test
{
namespace script
{
/**
 * Communication channel.
 *
 * Channels are used to communicate between Lua and C++. They provide a packet
 * based communication of raw (binary) data. Channels are implemented as Fifos.
 *
 * \author  Fabian Greif
 */
class Channel
{
public:
    /// Shared pointer to a channel object
    typedef std::shared_ptr<l3test::script::Channel> Ptr;
    typedef std::vector<uint8_t> Packet;

	Channel();

	~Channel();

	/**
	 * Append data to the current packet.
	 *
	 * \param data
	 *     Data to append.
	 * \param numberOfBytes
	 *     Number of bytes to append.
	 */
	void
	append(const uint8_t* data, size_t numberOfBytes);

	/**
	 * Close the current packet and make it available for reception.
	 */
	void
	finishPacket();

	bool
	hasPacket() const;

	/**
	 * Get the number of packets currently stored in the channel.
	 *
	 * \return  Number of packets.
	 */
	size_t
	getNumberOfPackets() const;

	/**
	 * Get the length of the current packet.
	 *
	 * Only valid when hasPacket() returns \c true.
	 *
	 * \return Number of bytes in the current packet.
	 */
	size_t
	getPacketLength() const;

	Packet&
	getPacket();

	/**
	 * Copy packet data in the supplied array.
	 *
	 * \param data
	 * \param
	 * \return
	 */
	size_t
	getPacket(uint8_t* data, size_t numberOfBytes) const;

	/**
	 * Drop the current packet and allow access to the next packet.
	 */
	void
	nextPacket();

private:
	Packet currentPacket;
	std::list< Packet > packets;
};
}
}

#endif // L3TEST_SCRIPT_CHANNEL_H
