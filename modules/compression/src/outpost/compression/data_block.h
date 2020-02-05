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
 * 2020, Jan-Gerd Mess (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_COMPRESSION_DATA_BLOCK_H_
#define OUTPOST_UTILS_COMPRESSION_DATA_BLOCK_H_

#include <outpost/base/fixpoint.h>
#include <outpost/base/slice.h>
#include <outpost/compression/nls_encoder.h>
#include <outpost/time.h>
#include <outpost/utils/container/shared_buffer.h>
#include <outpost/utils/storage/bitfield.h>
#include <outpost/utils/storage/bitstream.h>
#include <outpost/utils/storage/serializable_object.h>

namespace outpost
{
namespace compression
{
enum class SamplingRate : uint8_t
{
    disabled = 0,
    hz0033 = 1,
    hz01 = 2,
    hz05 = 3,
    hz1 = 4,
    hz2 = 5,
    hz5 = 6,
    hz10 = 7,
};

enum class Blocksize : uint8_t
{
    disabled = 0,
    bs16 = 1,
    bs128 = 2,
    bs256 = 3,
    bs512 = 4,
    bs1024 = 5,
    bs2048 = 6,
    bs4096 = 7,
};

enum class CompressionScheme : uint8_t
{
    raw = 0,
    waveletNLS = 1,
};

inline uint16_t
toUInt(Blocksize bs)
{
    switch (bs)
    {
        case Blocksize::bs16: return 16U;
        case Blocksize::bs128: return 128U;
        case Blocksize::bs256: return 256U;
        case Blocksize::bs512: return 512U;
        case Blocksize::bs1024: return 1024U;
        case Blocksize::bs2048: return 2048U;
        case Blocksize::bs4096: return 4096U;
        case Blocksize::disabled:
        default: return 0U;
    }
}

class DataBlock
{
public:
    static constexpr size_t headerSize = 11U;

    DataBlock() :
        mSampleCount(0),
        mParameterId(0),
        mStartTime(),
        mSamplingRate(SamplingRate::disabled),
        mBlocksize(Blocksize::disabled),
        mScheme(CompressionScheme::raw),
        mPointer(),
        mSampleBuffer(nullptr),
        mCoefficientBuffer(nullptr),
        mIsTransformed(false),
        mIsEncoded(false)
    {
    }

    DataBlock(outpost::utils::SharedBufferPointer p,
              uint16_t parameterId,
              outpost::time::GpsTime startTime,
              SamplingRate rate,
              Blocksize bs) :
        mSampleCount(0),
        mParameterId(parameterId),
        mStartTime(startTime),
        mSamplingRate(rate),
        mBlocksize(bs),
        mScheme(CompressionScheme::waveletNLS),
        mPointer(p),
        mSampleBuffer(reinterpret_cast<Fixpoint*>(&p[headerSize])),
        mCoefficientBuffer(reinterpret_cast<int16_t*>(&p[headerSize])),
        mIsTransformed(false),
        mIsEncoded(false)
    {
    }

    ~DataBlock()
    {
    }

    inline uint16_t
    getParameterId() const
    {
        return mParameterId;
    }

    inline outpost::time::GpsTime
    getStartTime()
    {
        return mStartTime;
    }

    inline uint16_t
    getSampleCount() const
    {
        return mSampleCount;
    }

    inline SamplingRate
    getSamplingRate() const
    {
        return mSamplingRate;
    }

    inline Blocksize
    getBlocksize() const
    {
        return mBlocksize;
    }

    void
    applyWaveletTransform();

    inline bool
    isTransformed()
    {
        return mIsTransformed;
    }

    inline bool
    isEncoded()
    {
        return mIsEncoded;
    }

    inline bool
    isComplete()
    {
        return mSampleCount > 0 && mSampleCount == toUInt(mBlocksize);
    }

    inline bool
    isValid()
    {
        return (mPointer.isValid()
                && mPointer.getLength() >= (toUInt(mBlocksize) * sizeof(Fixpoint) + headerSize));
    }

    inline bool
    push(Fixpoint f)
    {
        if (!isComplete() && isValid() && mPointer.getLength() >= mSampleCount * sizeof(Fixpoint))
        {
            mSampleBuffer[mSampleCount++] = f;
            return true;
        }
        return false;
    }

    bool
    encode(DataBlock& b, NLSEncoder& encoder);

    outpost::Slice<Fixpoint>
    getSamples();

    outpost::Slice<int16_t>
    getCoefficients();

    outpost::Slice<uint8_t>
    getEncodedDataBlock();

    inline size_t
    getMaximumSize()
    {
        return mPointer.getLength();
    }

private:
    uint16_t mSampleCount;

    uint16_t mParameterId;
    outpost::time::GpsTime mStartTime;
    SamplingRate mSamplingRate;
    Blocksize mBlocksize;
    CompressionScheme mScheme;

    outpost::utils::SharedBufferPointer mPointer;
    outpost::Fixpoint* mSampleBuffer;
    int16_t* mCoefficientBuffer;

    bool mIsTransformed;
    bool mIsEncoded;
};

}  // namespace compression
}  // namespace outpost

#endif /* MODULES_DAQ_SRC_MU_COMMON_PACKETS_DAQ_SENSOR_DATA_PACKET_H_ */
