
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
Channel::append(const uint8_t * data, size_t numberOfBytes)
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
Channel::hasPackets() const
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
	if (hasPackets()) {
		return packets.front().size();
	}

	return 0;
}

std::vector<uint8_t>
Channel::getPacket() const
{
	return packets.front();
}

size_t
Channel::getPacket(uint8_t * data, size_t numberOfBytes) const
{
	auto length = std::min(numberOfBytes, packets.front().size());

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
