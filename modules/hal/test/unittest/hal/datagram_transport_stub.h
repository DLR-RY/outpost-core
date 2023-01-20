#ifndef OUTPOST_HAL_DATAGRAM_TRANSPORT_MOCK_H
#define OUTPOST_HAL_DATAGRAM_TRANSPORT_MOCK_H

#include <outpost/hal/datagram_transport.h>
#include <outpost/rtos/semaphore.h>

#include <list>
#include <vector>

namespace unittest
{
namespace hal
{
class DatagramTransportStub : public outpost::hal::DatagramTransport
{
public:
    using outpost::hal::DatagramTransport::Address;

    DatagramTransportStub() : mReceiveSemaphore(0)
    {
    }

    bool
    connect() override
    {
        return true;
    }

    void
    close() override
    {
    }

    Address
    getAddress() const override
    {
        return mAddress;
    }

    void
    setAddress(const Address& newAddress) override
    {
        mAddress = newAddress;
    }

    bool
    isAvailable() override
    {
        return mReceiveBuffer.size() > 0;
    }

    size_t
    getNumberOfBytesAvailable() override
    {
        if (isAvailable())
        {
            return mReceiveBuffer.front().data.size();
        }
        return 0;
    }

    size_t
    getMaximumDatagramSize() const override
    {
        return 2048;
    }

    size_t
    sendTo(outpost::Slice<const uint8_t> data,
           const Address& address,
           outpost::time::Duration /* timeout =
                   std::numeric_limits<outpost::time::Duration>::max() */) override
    {
        Message msg{address, std::vector<uint8_t>(data.begin(), data.end())};
        mSendBuffer.push_back(msg);
        return data.getNumberOfElements();
    }

    size_t
    receiveFrom(outpost::Slice<uint8_t> data,
                Address& address,
                outpost::time::Duration timeout =
                        std::numeric_limits<outpost::time::Duration>::max()) override
    {
        mReceiveSemaphore.acquire(timeout);

        Message msg = mReceiveBuffer.front();
        address = msg.address;
        size_t i = 0;
        for (; i < std::min(data.getNumberOfElements(), msg.data.size()); ++i)
        {
            data[i] = msg.data[i];
        }
        mReceiveBuffer.pop_front();
        return i;
    }

    void
    clearReceiveBuffer() override
    {
        mReceiveBuffer.clear();
    }

    struct Message
    {
        Address address;
        std::vector<uint8_t> data;
    };

    /**
     * Add a message to the receiveBuffer which can be received via receiveFrom
     */
    void
    addMessage(Address address, outpost::Slice<uint8_t> data)
    {
        DatagramTransportStub::Message msg{address, std::vector<uint8_t>(data.begin(), data.end())};
        mReceiveBuffer.push_back(msg);
        mReceiveSemaphore.release();
    }

    void
    getLastSendMessage(outpost::Slice<uint8_t> data, Address& address)
    {
        if (mSendBuffer.empty())
        {
            address = Address();
        }
        else
        {
            Message msg = mSendBuffer.front();
            mSendBuffer.pop_front();
            address = msg.address;
            for (size_t i = 0; i < std::max(data.getNumberOfElements(), msg.data.size()); ++i)
            {
                data[i] = msg.data[i];
            }
        }
    }

    outpost::rtos::Semaphore mReceiveSemaphore;
    Address mAddress;
    std::list<Message> mReceiveBuffer;
    std::list<Message> mSendBuffer;
};

}  // namespace hal
}  // namespace unittest

#endif  // OUTPOST_HAL_DATAGRAM_TRANSPORT_MOCK_H
