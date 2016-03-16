/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.5.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_BOUNDED_ARRAY_H
#define COBC_BOUNDED_ARRAY_H

#include <stddef.h>

#include "meta.h"

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
    typedef typename cobc::remove_const<T>::type NonConstType;
    typedef const NonConstType ConstType;

    friend class BoundedArray<NonConstType>;
    friend class BoundedArray<ConstType>;

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

    inline
    BoundedArray(const BoundedArray& rhs) :
        mData(rhs.mData),
        mNumberOfElements(rhs.mNumberOfElements)
    {
    }

    inline BoundedArray&
    operator=(const BoundedArray& rhs)
    {
        mData = rhs.mData;
        mNumberOfElements = rhs.mNumberOfElements;
        return *this;
    }

    /**
     * Generate an empty array.
     */
    static inline BoundedArray
    empty()
    {
        BoundedArray array(0, 0);
        return array;
    }


    /**
     * Initialize from a pointer to an array.
     *
     * Example:
     * \code
     * uint8_t array[7];
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
     * \code
     * BoundedArray<uint8_t> array(...);
     *
     * for (size_t i = 0; i < array.getNumberOfElements(); ++i)
     * {
     *     array[i] = i;
     * }
     * \endcode
     *
     * \warning
     *      No out-of-bound error checking is performed.
     */
    inline T&
    operator[](size_t index) const
    {
        return mData[index];
    }

    inline T*
    begin() const
    {
        return &mData[0];
    }

    inline T*
    end() const
    {
        return &mData[mNumberOfElements];
    }

    inline
    operator BoundedArray<const T>() const
    {
        return BoundedArray<const T>(mData, mNumberOfElements);
    }

private:
    /// Pointer to the array
    T* mData;

    /// Size of array
    size_t mNumberOfElements;
};

template <typename T, size_t N>
static inline cobc::BoundedArray<T>
toArray(T (&array)[N])
{
    cobc::BoundedArray<T> a(array);
    return a;
}

}

#endif // COBC_BOUNDED_ARRAY_H
