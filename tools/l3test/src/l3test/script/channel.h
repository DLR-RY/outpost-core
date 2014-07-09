
#ifndef L3TEST_SCRIPT_CHANNEL_H
#define L3TEST_SCRIPT_CHANNEL_H

#include <vector>
#include <list>
#include <memory>

#include <cstddef>
#include <cstdint>

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
 * @author  Fabian Greif <fabian.greif@dlr.de>
 */
class Channel
{
public:
    /// Shared pointer to a channel object
    typedef std::shared_ptr<l3test::script::Channel> Ptr;

	Channel();

	~Channel();

	/**
	 * Append data to the current packet.
	 *
	 * @param data
	 *     Data to append.
	 * @param numberOfBytes
	 *     Number of bytes to append.
	 */
	void
	append(const uint8_t * data, size_t numberOfBytes);

	/**
	 * Close the current packet and make it available for reception.
	 */
	void
	finishPacket();

	bool
	hasPackets() const;

	/**
	 * Get the number of packets currently stored in the channel.
	 *
	 * @return  Number of packets.
	 */
	size_t
	getNumberOfPackets() const;

	/**
	 *
	 * @return Number of bytes in the current packet. Returns 0 if no packet
	 *     is available.
	 */
	size_t
	getPacketLength() const;

	std::vector<uint8_t>
	getPacket() const;

	/**
	 * Copy packet data in the supplied array.
	 *
	 * @param data
	 * @param
	 * @return
	 */
	size_t
	getPacket(uint8_t * data, size_t numberOfBytes) const;

	/**
	 * Drop the current packet and allow access to the next packet.
	 */
	void
	nextPacket();

private:
	std::vector<uint8_t> currentPacket;
	std::list< std::vector<uint8_t> > packets;
};
}
}

#endif // L3TEST_SCRIPT_CHANNEL_H
