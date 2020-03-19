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
/**
 * SamplingRate is the efficient encoding of certain possible cadences. Has to be enforced by the
 * collecting entity.
 */
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
/**
 * Blocksize determines the number of measurements that are combined to a single block.
 * Typically, larger blocks yield better compression ratios but, obviously, require more memory.
 */
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

/**
 * For future use of additional compression schemes
 */
enum class CompressionScheme : uint8_t
{
    raw = 0,
    waveletNLS = 1,
};

/**
 * Turns an enum coded blocksize to its actual size for comparisons
 * @param bs The blocksize to be decoded
 * @return Blocksize as uint16_t
 */
uint16_t
toUInt(Blocksize bs);

/**
 * A DataBlock is the entity that holds a block of samples during acquisition that can - once the
 * block is complete - be transformed into wavelet coefficients and later an encoded bitstream. The
 * latter operation needs a second DataBlock of the same size, because the encoding cannot be done
 * in-place.
 * Note: DataBlocks use SharedBufferPointer and may, therefore, STRICTLY NOT BE USED AS POINTERS to
 * avoid memory inconsistencies! Pass them as references, only.
 */
class DataBlock
{
public:
    /**
     * headerSize comprises of:
     * 2B ParameterId
     * 8B GpsTime
     * 1B (3+3+2 Bits) for SamplingRate, Blocksize and CompressionScheme
     */
    static constexpr size_t headerSize = 11U;

    /**
     * Empty and invalid DataBlock without any underlying memory.
     * Cannot be pushed to but is only a placeholder whenever a trivial constructor is required.
     */
    DataBlock();

    /**
     * Regular constructor for a valid, usable DataBlock
     * @param p SharedBufferPointer to serve as memory for incoming samples
     * @param parameterId Unique id of the parameter to be stored in this block
     * @param startTime Timepoint in GpsTime when the first sample of a block was collected
     * @param rate SamplingRate to recover the timing of individual data points
     * @param bs Blocksize, i.e. the number of samples to be collected before this block is
     * considered complete
     */
    DataBlock(outpost::utils::SharedBufferPointer p,
              uint16_t parameterId,
              outpost::time::GpsTime startTime,
              SamplingRate rate,
              Blocksize bs);

    /**
     * Default destructor.
     */
    ~DataBlock() = default;

    /**
     * Getter for parameter identification
     * @return Unique parameter ID of the current block
     */
    inline uint16_t
    getParameterId() const
    {
        return mParameterId;
    }

    /**
     * Getter for the start time
     * @return GpsTime of the timepoint at which the first sample was collected
     */
    inline outpost::time::GpsTime
    getStartTime() const
    {
        return mStartTime;
    }

    /**
     * Getter for the current number of samples in this block
     * @return Number of collected samples, coefficients or data bytes, depending on processing step
     */
    inline uint16_t
    getSampleCount() const
    {
        return mSampleCount;
    }

    /**
     * Getter for the block's sampling rate
     * @return The block's sampling rate
     */
    inline SamplingRate
    getSamplingRate() const
    {
        return mSamplingRate;
    }

    /**
     * Getter for the block's target number of samples, i.e. blocksize
     * @return The block's target blocksize
     */
    inline Blocksize
    getBlocksize() const
    {
        return mBlocksize;
    }

    /**
     * Getter for the block's transformation state
     * @return Returns true if the block has been wavelet-transformed, false otherwise.
     */
    inline bool
    isTransformed() const
    {
        return mIsTransformed;
    }

    /**
     * Getter for the block's encoding state
     * @return Returns true if the block has been NLS encoded, false otherwise.
     */
    inline bool
    isEncoded() const
    {
        return mIsEncoded;
    }

    /**
     * Getter for the block's memory capacity
     * @return Returns the maximum number of bytes that can be stored in the DataBlock.
     */
    size_t
    getMaximumSize() const;

    /**
     * Applies the LeGall53 wavelet transform to the current block of samples
     */
    void
    applyWaveletTransform();

    /**
     * Getter for the block's completeness in terms of its blocksize
     * @return Returns true if the number of samples according to its blocksize has been pushed,
     * false otherwise
     */
    bool
    isComplete() const;

    /**
     * Getter for the block's validity
     * @return Returns true if the underlying memory is valid and sufficiently large to hold the
     * current blocksize, false otherwise.
     */
    bool
    isValid() const;

    /**
     * Pushes a single fixpoint number to the current block if it is valid and not complete yet.
     * @param f Fixpoint to be pushed to the end of block
     * @return Returns true if the number could be pushed to the block (i.e. the block is valid and
     * not complete), false otherwise.
     */
    bool
    push(Fixpoint f);

    /**
     * Encodes the block's coefficients into another block (NLSEncoding cannot be performed
     * in-place), using a given encoder holding supporting data structures. Note that the target
     * block's memory needs to be of the same size or larger than the current block's.
     * @param b Target DataBlock
     * @param encoder NLSEncoder holding supporting data structures that shall be used
     * @return Returns True if the encoding was successful, fals otherwise.
     */
    bool
    encode(DataBlock& b, NLSEncoder& encoder) const;

    /**
     * Getter for a Slice of samples (in Fixpoint format).
     * @return Returns a Slice of acquired samples or an empty slice if the block has been
     * transformed or encoded.
     */
    outpost::Slice<Fixpoint>
    getSamples() const;

    /**
     * Getter for a Slice of coefficients (in int16_t format).
     * @return Returns a Slice of transformed samples (i.e. coefficients) or an empty slice if the
     * block has been not yet been transformed or is already encoded.
     */
    outpost::Slice<int16_t>
    getCoefficients() const;

    /**
     * Getter for a Slice of encoded data bytes (uint8_t, encoded bitstream).
     * @return Returns a slice of bytes holding the encoded bitstream if the block was already
     * encoded. If it still holds samples or coefficients, the slice will be empty.
     */
    outpost::Slice<uint8_t>
    getEncodedData() const;

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
