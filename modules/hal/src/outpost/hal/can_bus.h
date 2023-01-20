/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Andre Nahrwold (DLR SC-SRV)
 */

#ifndef OUTPOST_HAL_CAN_BUS_H
#define OUTPOST_HAL_CAN_BUS_H

#include <outpost/base/slice.h>
#include <outpost/time/duration.h>

#include <stdint.h>

#include <cstddef>

namespace outpost
{
namespace hal
{
/**
 * CAN bus Interface.
 *
 * \author  Andre Nahrwold
 */
class CanBus
{
public:
    enum class ReturnCode
    {
        success = 0,

        notInitialized,
        idInvalid,
        dataInvalid,
        countInvalid,
        bufferEmpty,
        bufferFull,

        failure
    };

    class CanFrame
    {
    public:
        /**
         * @brief Construct an empty can frame object
         *
         */
        CanFrame() : mId(0), mExtended(false), mRequest(false), mLength(0), mRawData{{}}
        {
        }

        /**
         * @brief Construct a can frame with specific characteristics.
         *
         * @param id ID of the frame - may be extended if extended bit is set. Additional bits are
         * cut off.
         * @param data Optional data to be stored in the frame.
         * @param extended Extended bit flag to chose CAN protocol. False by default.
         * @param request Request flag bit to send frame requests. False by default.
         */
        explicit CanFrame(
                uint32_t id,
                const outpost::Slice<const uint8_t>& data = outpost::Slice<const uint8_t>::empty(),
                bool extended = false,
                bool request = false) :
            mId(extended ? (id & 0x1FFFFFFF) : (id & 0x7FF)),
            mExtended(extended),
            mRequest(request),
            mLength(0),
            mRawData{{}}
        {
            writeData(data);
        }

        /**
         * @brief Construct a new can frame from an exsiting frame, copying its data and
         * characteristics.
         *
         * @param other Frame to copy from.
         */
        CanFrame(const CanFrame& other) :
            mId(other.mId),
            mExtended(other.mExtended),
            mRequest(other.mRequest),
            mLength(0),
            mRawData{{}}
        {
            writeData(other.getData());
        }

        /**
         * @brief Assignment between frames. Copyies data and characteristics.
         *
         * @param other Frame to copy from.
         * @return CanFrame& Copy of the rhs frame.
         */
        CanFrame&
        operator=(const CanFrame& other)
        {
            mId = other.mId;
            mExtended = other.mExtended;
            mRequest = other.mRequest;
            writeData(other.getData());
            return *this;
        }

        /**
         * @brief Helper function to create requests.
         *
         * @param id Request frame ID.
         * @param length Amount of data to be requested.
         * @param extended true by default for CAN2.0(B) frame, false for CAN2.0(A) frame.
         * @return CanFrame Request frame with given ID and length but no data.
         */
        static CanFrame
        makeRequestFrame(uint32_t id, uint8_t length, bool extended = true)
        {
            CanFrame request(id, outpost::Slice<uint8_t>::empty(), extended, true);
            request.mLength = length;
            return request;
        }

        /**
         * @brief Get the frame ID. May be 29-bit if extended bit is set, 11-bit otherwise.
         *
         * @return uint32_t Frame ID.
         */
        uint32_t
        getId() const
        {
            return mId;
        }

        /**
         * @brief Check if the frame is extended.
         *
         * @return true Is CAN2.0(B) frame with 29-bit identifier.
         * @return false Is CAN2.0(A) frame with 11-bit identifier.
         */
        bool
        isExtended() const
        {
            return mExtended;
        }

        /**
         * @brief Check if the frame is a request for sending.
         *
         * Request frames can be used to signal a remote node that it should send data.
         *
         * @return true Is a request frame (data should be ignored).
         * @return false Is a data frame.
         */
        bool
        isRequest() const
        {
            return mRequest;
        }

        /**
         * @brief Returns data length without actually returning data.
         *
         * Useful for request frames that have no data.
         *
         * @return uint8_t Length of the data or request.
         */
        uint8_t
        getLength() const
        {
            return mLength;
        }

        /**
         * @brief Change the characteristics of the the frame
         *
         * @param id ID of the frame - may be extended if extended bit is set. Additional bits are
         * cut off.
         * @param extended Extended bit flag to chose CAN protocol. False by default.
         * @param request Request flag bit to send frame requests. False by default.
         */
        void
        setHeader(uint32_t id, bool extended = false, bool request = false)
        {
            mId = extended ? (id & 0x1FFFFFFF) : (id & 0x7FF);
            mExtended = extended;
            mRequest = request;
        }

        /**
         * @brief Copy data into the frame.
         *
         * @warning Data greater than 8 bytes will be cut off.
         *
         * @param data To be copied into the frame.
         */
        void
        writeData(const outpost::Slice<const uint8_t> data)
        {
            auto trimmedData = data.first(8);
            outpost::asSlice(mRawData).copyFrom(trimmedData);
            mLength = trimmedData.getNumberOfElements();
        }

        /**
         * @brief Get read-only access to the data in the frame.
         *
         * @return const outpost::Slice<const uint8_t> Constant slice view of the data.
         */
        const outpost::Slice<const uint8_t>
        getData() const
        {
            return outpost::asSlice(mRawData).first(mLength);
        }

        /**
         * @brief Get read and write access to the data in the frame.
         *
         * @return const outpost::Slice<uint8_t> Slice view of the data.
         */
        const outpost::Slice<uint8_t>
        dataSlice()
        {
            return outpost::asSlice(mRawData).first(mLength);
        }

    private:
        // frame characteristics
        uint32_t mId;
        bool mExtended;
        bool mRequest;

        uint8_t mLength;
        // memory is owned by the frame
        std::array<uint8_t, 8> mRawData;
    };

public:
    /**
     * Checks the can id value (if maximal 29 bits are used)
     *
     * \param id the can id to test
     *
     * \retval true
     *      Valid can id
     * \retval false
     *      Invalid can id
     */
    static inline bool
    checkId(uint32_t id)
    {
        return ((id & 0xE0000000) == 0);
    }

public:
    /**
     *  Construct a new CanBus instance
     */
    CanBus() = default;

    /*
     *  Destory a CanBus instance.
     */
    virtual ~CanBus() = default;

    /**
     * Open the CAN bus device
     *
     * \return
     *      A return code
     */
    virtual ReturnCode
    open() = 0;

    /**
     * Close the CAN bus device
     *
     * \return
     *      A return code
     */
    virtual ReturnCode
    close() = 0;

    /**
     * Read the next frame from the input queue.
     *
     * \param frame
     *      CanFrame reference to store the incoming frame into.
     * \param timeout
     *      Implementations that use blocking calls will return after this timeout.
     *
     * \return
     *      A return code
     */
    virtual ReturnCode
    read(CanFrame& frame, outpost::time::Duration timeout = outpost::time::Duration::myriad()) = 0;

    /**
     * Write a frame to the output queue.
     *
     * Requests are sent by sending appropritate frames.
     *
     * \param frame
     *      CanFrame reference to send through the CanBus.
     * \return
     *      A return code
     */
    virtual ReturnCode
    write(const CanFrame& frame) = 0;

    /**
     * Clears the receive buffer and discards every message.
     *
     * May be implemented by reading until nothing is left.
     *
     * \return
     *      A return code
     */
    virtual ReturnCode
    clearReceiveBuffer() = 0;

    /**
     * @brief Check if data is available for reading.
     *
     * May be implemented by actually reading (and caching) a frame.
     *
     * @return true Data is available to be read.
     * @return false No pending data available.
     */
    virtual bool
    isDataAvailable() = 0;
};

}  // namespace hal
}  // namespace outpost

#endif