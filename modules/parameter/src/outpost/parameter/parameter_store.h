/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#ifndef OUTPOST_PARAMETER_PARAMETER_STORE_H_
#define OUTPOST_PARAMETER_PARAMETER_STORE_H_

#include "operation_result.h"
#include "parameter.h"
#include "parameter_iterator.h"

#include <outpost/base/slice.h>
#include <outpost/time.h>

#include <stddef.h>

namespace outpost
{
namespace parameter
{
/**
 * Class to allow (read) access to Parameter
 */
class ParameterStore
{
public:
    using Iterator = ParameterIterator<ParameterStore>;

    /**
     * Constructor
     *
     * @param memory The place to store the internal required pointer
     */
    explicit ParameterStore(outpost::Slice<const ParameterBase*> memory) :
        mParameters(memory), mCount(0), mInitialized(false)
    {
    }

    /**
     * Initializes the ParameterStore with a given list of parameters.
     * All Parameters must be initialized and have distinct IDs.
     *
     * @param list  The list of Parameter
     * @return  OK: Success
     *          alreadyInitialized: ParameterStore already initialized
     *          dublicatedID: Two or more Parameter in the list have identical IDs
     *          uninitializedParameter: At least one Parameter was not initialized
     *          invalidParameter: At least one Parameter is explicitly invalid
     *          tooLarge: the list contains more than maxElements Parameter
     */
    OperationResult
    initialize(ParameterList& list = IndexedParameterList<0>::getList());

    /**
     * Initializes the ParameterStore with a number of ParameterLists.
     * All Parameters must be initialized and have distinct IDs.
     *
     * @param lists  Several list of parameters that all should be handled by the store
     * @return  OK: Success
     *          alreadyInitialized: ParameterStore already initialized
     *          dublicatedID: Two or more Parameter in the list have identical IDs
     *          uninitializedParameter: At least one Parameter was not initialized
     *          invalidParameter: At least one Parameter is explicitly invalid
     *          tooLarge: the lists together contains more than maxElements Parameter
     */
    OperationResult
    initialize(outpost::Slice<ParameterList*> lists);

    /**
     * Provides the pointer to a given Parameter
     *
     * @param id the id of the Parameter
     * @param store Place to store the Parameter pointer
     * @return OK: success
     *         notInitialized: ParameterStore not yet initialized
     *         noSuchID: The given ID could not be found
     *         invalidParameter: The ID is invalid
     */
    OperationResult
    getParameter(IDType id, const ParameterBase*& store) const;

    /**
     * Provides the pointer to a given Parameter
     *
     * @param id the id of the Parameter
     * @param store Place to store the Parameter pointer
     * @return OK: success
     *         notInitialized: ParameterStore not yet initialized
     *         noSuchID: The given ID could not be found
     *         invalidParameter: The ID is invalid
     *         incorrectType: Provided pointer does not fit the Parameter type
     */
    template <typename T>
    OperationResult
    getParameter(IDType id, const Parameter<T>*& store) const;

    /**
     * Reads the current value of the parameter
     *
     * @param id The ID of the parameter
     * @param store Place to store the data, will only be changed in success case.
     * @param time  If not nullptr, place where to put the last time the Parameter was set
     * @return  OK: successful
     *          notInitialized: initialize was not called yet
     *          incorrectType: store is not of the type used during initialize, note fully
     *                         unsupported types will be caught by compiler/linker
     *          noSuchID: The given ID could not be found
     *          invalidParameter: The ID is invalid
     */
    template <typename T>
    OperationResult
    getValue(IDType id, T& store, outpost::time::SpacecraftElapsedTime* setTime = nullptr) const;

    /**
     * Get an iterator to the first Parameter contained.
     *
     * @return Iterator to first Element
     */
    Iterator
    begin() const;

    /**
     * Returns an iterator pointing to the past-the-end Parameter.
     *
     * @return Iterator to the past-the-end Parameter
     */
    Iterator
    end() const;

    /**
     * Returns the number of elements in this store
     *
     * @return number of elements in the store
     */
    size_t
    getCount() const;

    /**
     * Checks whether the store is already initialized
     *
     * @return true if already initialized
     */
    bool
    isInitialized() const;

private:
    ParameterStore(const ParameterStore&) = delete;
    ParameterStore&
    operator=(const ParameterStore&) = delete;

    outpost::Slice<const ParameterBase*> mParameters;
    size_t mCount;
    bool mInitialized;
};

template <size_t N>
class ParameterStoreMemory
{
protected:
    const ParameterBase* mMemory[N] = {};
};

template <size_t N>
class ParameterStoreWithMemory : private ParameterStoreMemory<N>, public ParameterStore
{
public:
    ParameterStoreWithMemory() :
        ParameterStoreMemory<N>(),
        ParameterStore(outpost::asSlice(ParameterStoreMemory<N>::mMemory))
    {
    }
};

}  // namespace parameter
}  // namespace outpost

#include "parameter_store_impl.h"

#endif
