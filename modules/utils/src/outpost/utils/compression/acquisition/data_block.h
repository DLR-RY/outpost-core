/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_COMPRESSION_DATA_BLOCK_H_
#define OUTPOST_UTILS_COMPRESSION_DATA_BLOCK_H_

#include <outpost/time.h>
#include <outpost/utils/container/shared_buffer.h>
#include <outpost/utils/storage/bitfield.h>
#include <outpost/utils/storage/serializable_object.h>

namespace outpost
{
namespace compression
{
enum class SamplingRate : uint8_t
{
    disabled = 0,
    hz2 = 2
};

enum class Blocksize : uint8_t
{
    disabled = 0,
    bs4 = 2,
    bs8 = 3,
    bs16 = 4,
    bs32 = 5,
    bs64 = 6,
    bs128 = 7,
    bs256 = 8,
    bs512 = 9,
    bs1024 = 10,
    bs2048 = 11,
    bs4096 = 12
};

inline uint16_t
toUInt(Blocksize bs)
{
    return 1U << static_cast<uint16_t>(bs);
}

class DataBlock : public outpost::SerializableObject
{
public:
    static constexpr size_t headerSize = 11U;

    DataBlock() :
        mParameterId(0),
        mStartTime(),
        mSamplingRate(SamplingRate::disabled),
        mBlocksize(Blocksize::disabled),
        mPointer(nullptr)
    {
    }

    DataBlock(outpost::utils::SharedBufferPointer p,
              uint16_t parameterId,
              outpost::time::GpsTime startTime,
              SamplingRate rate,
              Blocksize bs) :
        mParameterId(parameterId),
        mStartTime(startTime),
        mSamplingRate(rate),
        mBlocksize(bs),
        mPointer(p)
    {
    }

    ~DataBlock()
    {
    }

    inline void
    setParameterId(uint16_t param)
    {
        mParameterId = param;
    }

    inline uint16_t
    getParameterId() const
    {
        return mParameterId;
    }

    inline void
    setStartTime(outpost::time::GpsTime t)
    {
        mStartTime = t;
    }

    inline outpost::time::GpsTime
    getStartTime()
    {
        return mStartTime;
    }

    inline void
    setSamplingRate(SamplingRate value)  // 4 Bit
    {
        mSamplingRate = value;
    }

    inline SamplingRate
    getSamplingRate() const
    {
        return mSamplingRate;
    }

    inline void
    setBlocksize(Blocksize value)
    {
        mBlocksize = value;
    }

    inline Blocksize
    getBlocksize() const
    {
        return mBlocksize;
    }

    size_t
    getSerializedSize() const
    {
        return headerSize;
    }

    void
    serialize(outpost::Serialize&) const
    {
    	//TODO
    }

    bool
    deserialize(outpost::Deserialize&)
    {
    	//TODO
        return true;
    }

private:
    uint16_t mParameterId;
    outpost::time::GpsTime mStartTime;
    SamplingRate mSamplingRate;
    Blocksize mBlocksize;

    outpost::utils::SharedBufferPointer mPointer;
};

}  // namespace compression
}  // namespace outpost

#endif /* MODULES_DAQ_SRC_MU_COMMON_PACKETS_DAQ_SENSOR_DATA_PACKET_H_ */
