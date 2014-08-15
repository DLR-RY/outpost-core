
#include "channel.h"

using namespace l3test::script;

Channel::Channel()
{
}

Channel::~Channel()
{
}

// ----------------------------------------------------------------------------
void
Channel::append(const uint8_t* data,
                size_t numberOfBytes)
{
	currentPacket.reserve(currentPacket.size() + numberOfBytes);

	for (size_t i = 0; i < numberOfBytes; ++i) {
		currentPacket.push_back(data[i]);
	}
}

void
Channel::finishPacket()
{
	packets.push_back(currentPacket);
	currentPacket.clear();
}

// ----------------------------------------------------------------------------
bool
Channel::hasPacket() const
{
	return (packets.size() > 0);
}

size_t
Channel::getNumberOfPackets() const
{
    return packets.size();
}

size_t
Channel::getPacketLength() const
{
	return packets.front().size();
}

Channel::Packet&
Channel::getPacket()
{
	return packets.front();
}

size_t
Channel::getPacket(uint8_t* data,
                   size_t numberOfBytes) const
{
	auto length = std::min(numberOfBytes, getPacketLength());

	for (size_t i = 0; i < length; ++i) {
		data[i] = packets.front()[i];
	}

	return length;
}

void
Channel::nextPacket()
{
	packets.pop_front();
}
