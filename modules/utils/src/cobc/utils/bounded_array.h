/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_BOUNDED_ARRAY_H
#define COBC_BOUNDED_ARRAY_H

#include "stddef.h"

namespace cobc
{

/**
 * Wrapper for C style arrays with additional length information.
 *
 * \author  Fabian Greif
 */
template <typename T>
class BoundedArray
{
public:
    /**
     * Initialize directly from a C style array.
     *
     * The array needs to still have all the type information attached and
     * must not have degraded to a pointer type.
     *
     * Example:
     * \code
     * uint8_t array[7];
     * BoundedArray<uint8_t> wrappedArray(array);
     * \endcode
     *
     * \param array
     *      Array with should be wrapped.
     */
    template <size_t N>
    explicit inline
    BoundedArray(T (&array)[N]) :
        mData(array),
        mNumberOfElements(N)
    {
    }

    /**
     * Initialize from a pointer to an array.
     *
     * Example:
     * \code
     * uint8_t data[7];
     * BoundedArray<uint8_t> wrappedArray(&array[0], 7);
     * \endcode
     *
     * \param array
     *      Pointer to the first element of the array.
     * \param numberOfElements
     *      Number of elements in the array.
     */
    inline
    BoundedArray(T* array, size_t numberOfElements) :
        mData(array),
        mNumberOfElements(numberOfElements)
    {
    }

    /**
     * Get number of elements in the array.
     */
    inline size_t
    getNumberOfElements() const
    {
        return mNumberOfElements;
    }

    /**
     * Access elements of the array.
     *
     * \warning
     *      No out-of-bound error checking is performed.
     */
    inline T&
    operator[](size_t index)
    {
        return mData[index];
    }

    /**
     * Access elements of the array.
     *
     * \warning
     *      No out-of-bound error checking is performed.
     */
    inline const T&
    operator[](size_t index) const
    {
        return mData[index];
    }

private:
    /// Pointer to the array
    T* mData;

    /// Size of array
    size_t mNumberOfElements;
};

}

#endif // COBC_BOUNDED_ARRAY_H
