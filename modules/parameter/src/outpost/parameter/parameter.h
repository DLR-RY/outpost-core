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

#ifndef OUTPOST_PARAMETER_PARAMETER_H_
#define OUTPOST_PARAMETER_PARAMETER_H_

#include "operation_result.h"
#include "type.h"

#include <outpost/base/slice.h>
#include <outpost/time/time_epoch.h>
#include <outpost/utils/container/implicit_list.h>

#include <stddef.h>

#include <atomic>

namespace outpost
{
namespace parameter
{
class ParameterBase;
class ParameterStore;

using IDType = uint16_t;

/**
 * Protects the Parameter Anchor from all other than the Parameter, ParameterStore and the test
 */
struct ParameterList
{
    friend class ParameterBase;
    friend class ParameterStore;

    // for testing
    friend ParameterBase*
    getAnchor(const ParameterList&);

public:
    constexpr ParameterList() = default;

private:
    ParameterBase* anchor = nullptr;

    ParameterList(const ParameterList&) = delete;
    ParameterList&
    operator=(const ParameterList&) = delete;
};

/**
 * Provides indexed anchors for Parameter
 */
template <uint16_t index>
class IndexedParameterList
{
public:
    static ParameterList&
    getList()
    {
        return list;
    }

private:
    static ParameterList list;
};

class ParameterBase : public ImplicitList<ParameterBase>
{
    // for testing
    friend std::atomic_flag&
    getFlag(ParameterBase&);

public:
    static constexpr IDType invalidID = 0;  // the one used to indicate not initialized
    static constexpr unsigned int maxReadTries =
            10;  // A read will fail when a write is commenced at the same time
                 // This is the number of tries, each read needs its own write to fail

    /**
     * Creates a new parameter, also adds to implicit list.
     * Must initialize later.
     *
     * @param list the ParameterList to add this element
     */
    ParameterBase(ParameterList& list = IndexedParameterList<0>::getList()) :
        ImplicitList(list.anchor, this),
        mID(invalidID),
        mMultipleWriters(false),
        mCurrentElementIsFirst(true),
        mAssignedIdInvalid(false)
    {
        mWriteInProgress.clear();
    }

    virtual ~ParameterBase() = default;

    /**
     * Sets the parameter in the modus to allow writing to from different threads.
     * This is not reversible, reduces write speed
     */
    void
    activateMultipleWritersSupport();

    /**
     * compares the ID with the one of another Parameter
     * @param other The Parameter to compare with
     * @return true if this parameter's ID is smaller than other's ID
     */
    bool
    operator<(const ParameterBase& other) const;

    /*
     * Get the id of the parameter
     *
     * @return returns the ID of the Parameter, or invalidID if not initialized
     */
    IDType
    getID() const;

    /**
     * Gets the type of the parameter
     *
     * @return The type stored in the parameter
     */
    virtual Type
    getType() const = 0;

    /**
     * @return true if parameter is initialized
     */
    bool
    isInitialized() const;

    /**
     * @return true if an invalid id was explicitly assigned to the parameter
     */
    bool
    hasInvalidIdAssigned() const;

    /**
     * Sorts a given Slice
     *
     * @param slice slice to sort
     */
    static void
    sort(outpost::Slice<ParameterBase*> slice);

    /**
     * Sorts a given Slice
     *
     * @param slice slice to sort
     */
    static void
    sort(outpost::Slice<const ParameterBase*> slice);

    /**
     * Does a binary search for a id
     *
     * @param slice the slice in which to search
     * @param id the id to search for
     * @return Pointer to the corresponding parameter or nullptr if not contained
     */
    static ParameterBase*
    findInSorted(outpost::Slice<ParameterBase*> slice, IDType id);

    /**
     * Does a binary search for a id
     *
     * @param slice the slice in which to search
     * @param id the id to search for
     * @return Pointer to the corresponding parameter or nullptr if not contained
     */
    static const ParameterBase*
    findInSorted(outpost::Slice<const ParameterBase*> slice, IDType id);

protected:
    IDType mID;

    bool mMultipleWriters;

    bool mCurrentElementIsFirst;

    bool mAssignedIdInvalid;

    std::atomic_flag mWriteInProgress;

    /**
     * Does a binary search for a id
     *
     * @param slice the slice in which to search
     * @param id the id to search for
     * @return index of the Parameter or -1 if not contained
     */
    static int
    findInternal(outpost::Slice<const ParameterBase*> slice, IDType id);

    /**
     * Compares two parameter given their pointer
     *
     * @param a Pointer to the first Parameter
     * @param b Pointer to the second Parameter
     * @return true if the first Parameter is smaller than the second Parameter
     */
    static bool
    comparePointedParameter(const ParameterBase* a, const ParameterBase* b)
    {
        return (*a) < (*b);
    }
};

template <typename T>
class Parameter : public ParameterBase
{
public:
    /**
     * Default Constructor, creates an not initialized Parameter.
     * Can choose the list to put the parameter in
     *
     * @param list the list in which this parameter should be put
     */
    explicit Parameter(ParameterList& list = IndexedParameterList<0>::getList()) :
        ParameterBase(list), mElements{{}}
    {
    }

    /**
     * Creates a new Parameter that is already initialized.
     *
     * @param id   The id of the parameter, must not be equal to Parameter::invalidID
     *             setting this parameter to Parameter::invalidID will result in an uninitialized
     *             Parameter
     * @param initialValue  The initial value of the parameter
     * @param time the current time
     * @param list the ParameterList to add this element
     * @param hasMultipleWriters  set true if this parameter will be written from different threads.
     *                           Will slow set operation
     */
    Parameter(IDType id,
              const T& initialValue,
              const outpost::time::SpacecraftElapsedTime& time,
              ParameterList& list = IndexedParameterList<0>::getList(),
              bool hasMultipleWriters = false);

    /**
     * Initializes the parameter
     *
     * @param id   The id of the parameter, must not be equal to Parameter::invalidID
     * @param initialValue  The initial value of the parameter
     * caught be compiler/linker
     * @param time  the current time
     * @param hasMultipleWriters  set true if this parameter will be written from different threads.
     *                           Will slow set operation
     *
     * @return success: successful
     *         invalidState: Parameter was already initialized
     *         invalidParameter: used invalidID as id
     */
    OperationResult
    initialize(IDType id,
               const T& initialValue,
               const outpost::time::SpacecraftElapsedTime& time,
               bool hasMultipleWriters = false);

    /**
     * Reads the current value of the parameter
     *
     * @param store Place to store the data, will only be changed in success case.
     * @param time  If not nullptr, place where to put the last time the parameter has set
     * @return  success: successful
     *          invalidState: initialize was not called yet
     */
    OperationResult
    getValue(T& store, outpost::time::SpacecraftElapsedTime* time = nullptr) const;

    /**
     * Set the value of the parameter
     *
     * @param data The data to write
     * @param time the current time
     * @return success:successful
     *         invalidState: initialize was not called yet
     *         concurrentWrite: Another thread was
     *                           already writing the data, can only occur if hasMultipleWriter was
     *                           true
     */
    OperationResult
    setValue(const T& data, const outpost::time::SpacecraftElapsedTime& time);

    Type
    getType() const override
    {
        return Type::getType<T>();
    }

private:
    struct Element
    {
        T data;
        outpost::time::SpacecraftElapsedTime changeTime;
    };
    Element mElements[2];
};

}  // namespace parameter
}  // namespace outpost

#include "parameter_impl.h"

#endif
