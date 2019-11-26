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
 * - 2015-2019, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_STORAGE_BITSTREAM_H_
#define OUTPOST_UTILS_STORAGE_BITSTREAM_H_

#include <outpost/utils/storage/serializable_object.h>

namespace outpost
{
/**
 * A class for pushing single bits to and reading from a stream of data.
 */
class Bitstream : outpost::SerializableObject
{
private:
    outpost::Slice<uint8_t>& mData;  // Buffer for storing the bitstream
    uint16_t bytePointer;
    int8_t bitPointer;
    static constexpr uint8_t headerSize = sizeof(bytePointer) + sizeof(bitPointer);

public:
    Bitstream(outpost::Slice<uint8_t>& byteArray) :
        mData(byteArray),
        bytePointer(headerSize),
        bitPointer(7)
    {
    }

    ~Bitstream() = default;

    bool
    isFull() const
    {
        return mData.getNumberOfElements() <= bytePointer;
    }

    /**
     * Pushes one bit to the stream
     * \param b
     *     Bit value to push to the stream
     */
    void
    pushBit(bool b)
    {
        if (!isFull())
        {
            if (b)
            {
                mData[bytePointer] |= (1 << bitPointer);
            }
            else
            {
                mData[bytePointer] &= ~(1 << bitPointer);
            }

            bitPointer--;

            if (bitPointer == -1)
            {
                bitPointer = 7;
                bytePointer++;
            }
        }
    }

    /**
     * Grants bit-level access to the stream
     *
     * \param n
     *     Bit in the stream to be accessed
     * \retval True
     *    If the nth bit of the stream is set
     * \retval False
     *    If the nth bit of the stream is not set or n is out of bounds
     */
    inline bool
    getBit(uint32_t n) const
    {
        uint32_t offsetBytePointer = bytePointer - headerSize;
        if (n < ((offsetBytePointer << 3) + (7 - bitPointer)))
        {
            return ((mData[(n >> 3) + headerSize] & (1 << (7 - (n & 7)))) > 0);
        }

        return 0;
    }

    /**
     * Grants byte-level access to the stream
     *
     * \param n
     *     Byte to access
     * \return Byte value at position n or 0 if n is out of range.
     */
    inline uint8_t
    getByte(uint16_t n) const
    {
        if (n < bytePointer - headerSize)
        {
            return mData[n + headerSize];
        }
        return 0;
    }

    /**
     * The current size of the stream in bytes
     * \return The current buffer length
     */
    inline uint16_t
    getSize() const
    {
        return bytePointer + (bitPointer < 7) - headerSize;
    }

    /**
     * Get the size of the parameter.
     *
     * \return  Size of the parameter in bytes.
     */
    virtual size_t
    getSerializedSize() const override
    {
        return getSize() + headerSize;
    }

    /**
     * Write parameter value onto the output stream.
     *
     * \param stream
     *      Output stream
     */
    virtual void
    serialize(Serialize& stream) const override
    {
        serialize(stream, bytePointer);
    }

    /**
     * Write parameter value onto the output stream.
     *
     * \param stream
     *      Output stream
     * \param maxLength
     *      Maximum length of the resulting stream
     */
    virtual void
    serialize(Serialize& stream, uint16_t maxLength) const
    {
        if (maxLength > bytePointer)
        {
            maxLength = bytePointer;
        }

        stream.store(maxLength);
        stream.store(bitPointer);

        if (stream.getPointer() != &mData[0])
        {
            stream.store(mData.skipFirst(headerSize).first(maxLength - headerSize));
        }
        else
        {
            stream.skip(maxLength - headerSize);
        }
    }

    /**
     * Read parameter value from stream.
     *
     * \param stream
     *      Input stream
     * \retval  true    Parameter could be read and validated.
     * \retval  false   Parameter validation failed.
     */
    virtual bool
    deserialize(Deserialize& stream) override
    {
        uint16_t bytePointer_tmp = stream.read<uint16_t>();
        uint16_t bitPointer_tmp = stream.read<int8_t>();
        if (bytePointer_tmp < mData.getNumberOfElements())
        {
            bytePointer = bytePointer_tmp;
            bitPointer = bitPointer_tmp;
            if (stream.getPointer() != &mData[0])
            {
                stream.readBuffer(&mData[headerSize], bytePointer - headerSize);
            }
            else
            {
                stream.skip(bytePointer - headerSize);
            }
            return true;
        }
        return false;
    }
};

}  // namespace outpost

#endif /*OUTPOST_UTILS_STORAGE_BITSTREAM_H_ */
