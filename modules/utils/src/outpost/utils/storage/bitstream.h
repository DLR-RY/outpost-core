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

namespace outpost
{

class Bitstream
{
private:
    static constexpr uint8_t headerSize = 2; // used for
    outpost::Slice<uint8_t>& mData;
    uint16_t bytePointer;
    int8_t bitPointer;

public:
    Bitstream(outpost::Slice<uint8_t>& byteArray, bool init = true) :
        mData(byteArray),
        bytePointer(headerSize),
        bitPointer(7)
    {
        if (init)
        {
            outpost::Serialize stream(mData);
            stream.store<uint16_t>(bytePointer);
        }
        else
        {
            outpost::Deserialize stream(mData);
            bytePointer = stream.read<uint16_t>();
            bitPointer = 0;
        }
    }

    void
    pushBit(bool b)
    {
        if (b)
        {
            mData[bytePointer] |= (1 << bitPointer);
        }
        else
        {
            mData[bytePointer] &= ~(1 << bitPointer);
        }

        bitPointer = bitPointer - 1;

        if (bitPointer == -1)
        {
            bitPointer = 7;
            bytePointer++;
        }
    }

    inline bool
    getBit(uint16_t n) const
    {
        uint16_t tmp_byte_pointer = bytePointer + headerSize;
        if (n < ((tmp_byte_pointer << 3) + (7 - bitPointer)))
        {
            return ((mData[(n >> 3) + headerSize] & (1 << (7 - (n % 8)))) > 0);
        }

        return 0;
    }

    uint8_t
    getByte(uint16_t n) const
    {
        return mData[n + headerSize];
    }

    uint16_t
    getSize() const
    {
        return bytePointer + (bitPointer < 7);
    }

    void
    print()
    {
        for (int i = 0; i <= bytePointer - 1; i++)
        {
            std::cout << std::bitset<8>(mData[i]);
            if(i > 0 && i % 16 == 0)
            {
            	std::cout << std::endl;
            }
            std::cout << "";
        }

        for (int i = 7; i > bitPointer; i--)
        {
            std::cout << (bool(mData[bytePointer] & 1 << i) > 0);
        }

        std::cout << std::endl;
    }

    void
    serialize()
    {
        outpost::Serialize stream(mData);
        stream.store<uint16_t>(bytePointer);
    }

    void
    serialize(uint16_t maxSize)
    {
        if (maxSize < bytePointer)
        {
            bytePointer = maxSize;
            bitPointer = 7;
        }
        serialize();
    }
};

}  // namespace outpost

#endif OUTPOST_UTILS_STORAGE_BITSTREAM_H_ */
