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

#include "data_block.h"

#include "legall_wavelet.h"

namespace outpost
{
namespace compression
{
constexpr size_t DataBlock::headerSize;

outpost::Slice<Fixpoint>
DataBlock::getSamples()
{
    if (isTransformed() || isEncoded())
    {
        return outpost::Slice<Fixpoint>::empty();
    }
    else
    {
        return outpost::Slice<Fixpoint>::unsafe(mSampleBuffer, mSampleCount);
    }
}

outpost::Slice<int16_t>
DataBlock::getCoefficients()
{
    if (isTransformed() && !isEncoded())
    {
        return outpost::Slice<int16_t>::unsafe(mCoefficientBuffer, mSampleCount);
    }
    else
    {
        return outpost::Slice<int16_t>::empty();
    }
}

outpost::Slice<uint8_t>
DataBlock::getEncodedDataBlock()
{
    if (isEncoded())
    {
        outpost::Serialize stream(&mPointer[0]);
        stream.store<uint16_t>(mParameterId);
        stream.store<uint64_t>(mStartTime.timeSinceEpoch().milliseconds());

        {
            uint8_t* pos = stream.getPointerToCurrentPosition();
            outpost::Bitfield::write<0, 2>(pos, static_cast<uint8_t>(mSamplingRate));
            outpost::Bitfield::write<3, 5>(pos, static_cast<uint8_t>(mBlocksize));
            outpost::Bitfield::write<6, 7>(pos, static_cast<uint8_t>(mScheme));
        }

        return outpost::Slice<uint8_t>::unsafe(&mPointer[0], headerSize + mSampleCount);
    }
    else
    {
        return outpost::Slice<uint8_t>::empty();
    }
}

void
DataBlock::applyWaveletTransform()
{
    if (!isTransformed() && !isEncoded() && mSampleCount > 0)
    {
        outpost::Slice<Fixpoint> samples = this->getSamples();
        LeGall53Wavelet::forwardTransformInPlace(samples);
        LeGall53Wavelet::reorder(samples);
        mIsTransformed = true;
    }
}

bool
DataBlock::encode(DataBlock& b, NLSEncoder& encoder)
{
    if (isTransformed() && b.getMaximumSize() >= getMaximumSize())
    {
        outpost::Slice<uint8_t> slice = b.mPointer.asSlice().skipFirst(headerSize);
        outpost::Bitstream bitstream(slice);
        encoder.encode(getCoefficients(), bitstream);
        outpost::Serialize stream(slice);
        bitstream.serialize(stream);
        b.mSampleCount = bitstream.getSerializedSize();
        b.mIsEncoded = true;
        return true;
    }
    return false;
}

}  // namespace compression
}  // namespace outpost
