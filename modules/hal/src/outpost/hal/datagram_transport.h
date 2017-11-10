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
 * - 2017, Jan Sommer (DLR SC-SRV)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_HAL_DATAGRAM_TRANSPORT_H
#define OUTPOST_HAL_DATAGRAM_TRANSPORT_H

#include <array>

#include <outpost/time/duration.h>

#include <outpost/utils/container/fixed_size_array.h>
#include <outpost/utils/container/slice.h>

namespace outpost 
{
namespace hal
{

/* 
 * Interface class for datagram based IP communication (e.g. UDP)
 *
 * This class provides a generic interface for network communication
 * using the internet protocol (ATM only version 4). It corresponds to
 * the Transport Layer (Layer 4) of the OSI model with the focus on 
 * a packet/datagram style of communication (e.g. UDP).
 *
 */
class DatagramTransport
{
public:
    class IpAddress
    {
    public:
        /**
         * Default constructor.
         *
         * Set the Ip address to zero.
         */
        constexpr
        IpAddress() :
            mIpAddress {{ 0, 0, 0, 0 }}
        {
        }

        /**
         * Constructor
         *
         * \param ipAddress IPv4 Address provided in host-byte-order
         */
        constexpr
        IpAddress(std::array<uint8_t, 4> ipAddress) :
            mIpAddress(ipAddress)
        {
        }

        /**
         * Constructor
         *
         * Allows to provide the IP-Address more conveniently, e.g.:
         * \code
         * IpAddress myIp(192, 168, 0, 1);
         * \endcode
         *
         * \param byte1 First byte (left most) of an IPv4 Address
         * \param byte2 Second  byte of an IPv4 Address
         * \param byte3 Third byte of an IPv4 Address
         * \param byte4 Fourth byte (right most) of an IPv4 Address
         */
        constexpr
        IpAddress(uint8_t byte1,
                  uint8_t byte2,
                  uint8_t byte3,
                  uint8_t byte4) :
           // store the IP address in network-byte-order
           mIpAddress {{ byte1, byte2, byte3, byte4 }}
        {
        }

        /**
         * Returns the internal byte array holding the ip-address in
         * network-byte-order.
         *
         * \return Array holding the IP-Address
         */
        inline std::array<uint8_t, 4>
        getArray() const
        {
            return mIpAddress;
        }

        inline constexpr uint8_t
        operator[](size_t index) const
        {
            return mIpAddress[index];
        }

    private:
        std::array<uint8_t, 4> mIpAddress;
    };

    /**
     * Representation of an datagram address composed of an IP and a port number.
     */
    class Address
    {
    public:
        /**
         * Constructor
         */
        constexpr
        Address() :
            mIpAddress(),
            mPort(0)
        {
        }

        inline constexpr
        Address(IpAddress ip, uint16_t port) :
            mIpAddress(ip),
            mPort(port)
        {
        }

        /**
         * \return The port in host-byte-order
         */
        inline uint16_t
        getPort() const
        {
            return mPort;
        }

        inline IpAddress
        getIpAddress() const
        {
            return mIpAddress;
        }
        
    protected:
        IpAddress mIpAddress;
        uint16_t mPort;
    };

    /**
     * Destructor
     */
    virtual 
    ~DatagramTransport();

    /**
     * Set up the connection for sending and receiving
     */
    virtual bool
    connect() = 0;
    
    /** 
     * Close the Ethernet device
     */                
    virtual void 
    close() = 0;

    /** 
     * Return the address of the device
     */ 
    virtual Address 
    getAddress() const = 0;

    /**
     * Sets the address of the device and disconnects listening clients
     *
     * Will reset the connection causing all active calls to \ref sendTo and
     * \ref receiveFrom to return immediately. The internal address is set to
     * newAddress. The connection is not initialized automatically. A call to
     * \ref connect is needed before any data can be received or send again.
     *
     * \param newAddress The new address to use
     */
    virtual void
    setAddress(const Address& newAddress) = 0;

    /**      
     * Check if a new datagram is available
     *      
     * \retval true   Data is available and can be read via receiveFrom(...).      
     * \retval false  No data available.      
     */
    virtual bool
    isAvailable() = 0;

    /**      
     * Returns the number of bytes of the next available datagram, if any.      
     * \return  Number of bytes in the buffer     
     */         
    virtual size_t
    getNumberOfBytesAvailable() = 0;

    /**
     *    Returns the maximum number of bytes which can be send in one datagram
     *  
     *  Trying to send a larger amount of data in one \ref sendTo command might 
     *  cause data loss.
     */
    virtual size_t
    getMaximumDatagramSize() const = 0;
    
    /**      
     * Send a datagram with \p data to \p address
     *      
     * \param data
     *      Buffer containing the data to send. Should not exceed
     *      \ref getMaximumDatagramSize.
     * \param address
     *      Address of the remote terminal to send the data to.
     * \param timeout
     *      Function will return after that time even is not all data sent;
     *      default blocking call.
     * \return
     *      Number of bytes which could be sent, maximal `data.getNumberOfElements`
     */
    virtual size_t
    sendTo(outpost::Slice<const uint8_t> data, 
           const Address& address,
           outpost::time::Duration timeout = outpost::time::Duration::maximum()) = 0;

    /**
     * Read a datagram received from a remote address with timeout.
     *
     * Reads the next datagram received or waits until timeout occurs.
     * If `data.getNumberOfElements()` is smaller than the length of the next
     * datagram only `data.getNumberOfElements()` bytes will be read back and
     * the remaining information of the datagram will be lost. Use
     * \ref getNumberOfBytesAvailable to check how long the next datagram
     * will be and make sure to supply a sufficient data buffer.
     *  
     * \param data
     *      Buffer to write the received data to.
     * \param address
     *      Will contain the address from which the datagram was received,
     *      if available.
     * \param timeout
     *      Function will return after that time if no datagram was received;
     *      default blocking call.
     * \return
     *      Number of bytes written to data, maximal data.getNumberOfElements()
     */
    virtual size_t
    receiveFrom(outpost::Slice<uint8_t>& data, 
                Address& address, 
                outpost::time::Duration timeout = outpost::time::Duration::maximum()) = 0;

    /** 
     * Drop all datagrams which are currently in the receive buffer
     */
    virtual void
    clearReceiveBuffer() = 0;
};

} 
}

#endif // OUTPOST_HAL_DATAGRAM_TRANSPORT_H

