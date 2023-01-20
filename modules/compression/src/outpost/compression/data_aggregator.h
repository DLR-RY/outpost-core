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

#ifndef OUTPOST_COMPRESSION_DATA_AGGREGATOR_H_
#define OUTPOST_COMPRESSION_DATA_AGGREGATOR_H_

#include "data_block.h"

#include <outpost/utils/container/implicit_list.h>

namespace outpost
{
template <unsigned PREC>
class FP;
typedef FP<16> Fixpoint;

namespace time
{
class Clock;
}

namespace utils
{
template <typename T>
class ReferenceQueueBase;

class SharedBufferPoolBase;
}  // namespace utils

namespace compression
{
class DataBlockSender;

/**
 * The DataAggregator is responsible for receiving samples of a single parameter (identified by its
 * ID) and handling allocation and transmission of DataBlocks using a given DataBlockSender.
 */
class DataAggregator : public ImplicitList<DataAggregator>
{
public:
    /**
     * Trivial constructor. Requires a call to initialize(..) before the aggregator can be used.
     */
    DataAggregator();

    /**
     * Constructor for a DataAggregator of a single parameter ID.
     * @param paramId ID of the parameter to be managed by the aggregator
     * @param pool SharedBufferPool to allocate the underlying memory of new DataBlocks
     * @param sender The DataAggregator's output for completed DataBlocks
     */
    DataAggregator(uint16_t paramId,
                   outpost::utils::SharedBufferPoolBase& pool,
                   DataBlockSender& sender);
    ~DataAggregator();

    /**
     * Required after using the trivial constructor.
     * @param paramId Parameter Id. May not be present in the system already.
     * @param pool SharedBufferPool to allocate the underlying memory of new DataBlocks
     * @param sender The DataAggregator's output for completed DataBlocks
     * @return Returns true iff the paramId is unique, false otherwise.
     */
    bool
    initialize(uint16_t paramId,
               outpost::utils::SharedBufferPoolBase& pool,
               DataBlockSender& sender);

    /**
     * Pushes a single Fixpoint to the current DataBlock, outputs completed DataBlocks and starts a
     * new one if needed.
     * @param fp Fixpoint number to be added to a DataBlock
     * @param currentTime Current GpsTime of the data sample
     * @return Returns true if the Fixpoint number could be stored in a DataBlock, false otherwise.
     */
    bool
    push(Fixpoint fp, const outpost::time::GpsTime& currentTime);

    /**
     * Getter for the parameter ID
     * @return Parameter ID
     */
    inline uint16_t
    getParameterId() const
    {
        return mParameterId;
    }

    /**
     * Setter for the sampling rate. This will only affect the next DataBlock being started, not the
     * current one.
     * @param sr SamplingRate for the upcoming block.
     */
    inline void
    setNextSamplingRate(SamplingRate sr)
    {
        mNextSamplingRate = sr;
    }

    /** Getter for the current sampling rate
     * @return Current SamplingRate
     */
    inline SamplingRate
    getSamplingRate() const
    {
        return mSamplingRate;
    }

    /**
     * Getter for the sampling rate of any upcoming DataBlocks
     * @return SamplingRate of the upcoming DataBlocks
     */
    inline SamplingRate
    getNextSamplingRate() const
    {
        return mNextSamplingRate;
    }

    /**
     * Determines whether the SamplingRate changes from the current to the next DataBlocks
     * @return Returns true if the SamplingRate changes, false otherwise.
     */
    inline bool
    hasSamplingRateChanged() const
    {
        return mSamplingRate != mNextSamplingRate;
    }

    /**
     * Setter for the block size. This will only affect the next DataBlock being started, not the
     * current one.
     * @param bs Block size for the upcoming block.
     */
    inline void
    setNextBlocksize(Blocksize bs)
    {
        mNextBlocksize = bs;
    }

    /**
     * Getter for the current DataBlock's block size.
     * @return Returns the current block size
     */
    inline Blocksize
    getBlocksize() const
    {
        return mBlocksize;
    }

    /**
     * Getter for the block size of any upcoming DataBlocks
     * @return Blocksize of the upcoming DataBlocks
     */
    inline Blocksize
    getNextBlocksize() const
    {
        return mNextBlocksize;
    }

    /**
     * Determines whether the Blocksize changes from the current to the next DataBlocks
     * @return Returns true if the Blocksize changes, false otherwise.
     */
    inline bool
    hasBlocksizeChanged() const
    {
        return mBlocksize != mNextBlocksize;
    }

    /**
     * Checks if acquisition for the DataAggregator is enabled.
     * @return Returns true if data acquisition is enabled, false otherwise.
     */
    inline bool
    isEnabled() const
    {
        return mEnabled;
    }

    /**
     * Checks if the underlying DataBlock is valid.
     * This serves as an indicator for whether a new DataBlock is about to start,
     * because after transmitting a completed block and allocating a new one, the private DataBlock
     * object will be invalidated.
     * @return True if the current DataBlock is valid, false otherwise.
     */
    bool
    isAtStartOfNewBlock() const;

    /**
     * Enables the acquisition using a given SamplingRate and Blocksize.
     * @param sr SamplingRate to use
     * @param bs Blocksize to use
     * @return Returns true iff the aggregator could be enabled , false otherwise.
     */
    bool
    enable(SamplingRate sr, Blocksize bs);

    /**
     * Enables the acquisition for a single block using a given SamplingRate and Blocksize.
     * @param sr SamplingRate to use
     * @param bs Blocksize to use
     * @return Returns true iff the aggregator could be enabled , false otherwise.
     */
    bool
    enableForOneBlock(SamplingRate sr, Blocksize bs);

    /**
     * Disables data acquisition immediately and invalidates the current block.
     */
    void
    disable();

    /**
     * Disables data acquisition after the current block has been completed.
     */
    inline void
    disableAfterCurrentBlock()
    {
        mDisableAfterCurrentBlock = true;
    }

    /**
     * Getter for the number of completed blocks.
     * @return Returns the number of blocks completed by the DataAggregator.
     */
    inline size_t
    getNumCompletedBlocks() const
    {
        return mNumCompletedBlocks;
    }

    /**
     * Getter for the number of lost blocks.
     * @return Returns the number of blocks that could not be sent and had to be dropped.
     */
    inline uint16_t
    getNumLostBlocks() const
    {
        return mNumLostBlocks;
    }

    /**
     * Getter for the number of lost samples.
     * @return Returns the number of samples that could not be stored to a block.
     */
    inline uint16_t
    getNumLostSamples() const
    {
        return mNumLostSamples;
    }

    /**
     * Getter for the number of acquired samples.
     * @return Returns the number of overall samples acquired by the DataAggregator.
     */
    inline size_t
    getNumOverallSamples() const
    {
        return mNumOverallSamples;
    }

    /**
     * Resets the DataAggregator's counters to 0.
     */
    inline void
    resetCounters()
    {
        mNumCompletedBlocks = 0;
        mNumOverallSamples = 0;
    }

    /**
     * Finds a DataAggregator by its parameterId
     * @param paramId Parameter Id of the DataAggregator to find.
     * @return Returns a pointer to the corresponding DataAggregator if it was found, nullptr
     * otherwise.
     */
    static DataAggregator*
    findDataAggregator(uint16_t paramId);

    /**
     * Counts all DataAggregators in the system by iterating over them.
     * @return Returns the current number of DataAggregators
     */
    static uint16_t
    numberOfAggregators();

    /**
     * Provides the list of all DataAggregators
     * @return Returns a pointer to the first DataAggregator in the list
     */
    static inline DataAggregator*
    getList()
    {
        return listOfAllDataAggregators;
    }

protected:
    static DataAggregator* listOfAllDataAggregators;

    uint16_t mParameterId;
    SamplingRate mSamplingRate;
    SamplingRate mNextSamplingRate;

    Blocksize mBlocksize;
    Blocksize mNextBlocksize;

    DataBlock mBlock;

    bool mEnabled;
    bool mDisableAfterCurrentBlock;

    outpost::utils::SharedBufferPoolBase* mMemoryPool;
    DataBlockSender* mSender;

    uint16_t mNumCompletedBlocks;
    uint16_t mNumLostBlocks;
    uint16_t mNumLostSamples;
    size_t mNumOverallSamples;
};

}  // namespace compression
}  // namespace outpost

#endif /* OUTPOST_COMPRESSION_DATA_AGGREGATOR_H_ */
