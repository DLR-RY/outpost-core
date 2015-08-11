/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.4.
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

#ifndef COBC_FIXED_SIZE_ARRAY_H
#define COBC_FIXED_SIZE_ARRAY_H

#include <stddef.h>
#include <string.h>     // for memcpy

#include "../utils.h"

namespace cobc
{

/**
 * Wrapper for C style arrays with a fixed length.
 *
 * Similar to std::array in C++11.
 *
 * \author  Fabian Greif
 */
template <typename T, size_t N>
class FixedSizeArray
{
public:
    typedef typename cobc::remove_const<T>::type NonConstType;
    typedef const NonConstType ConstType;

    friend class FixedSizeArray<const T, N>;

    FixedSizeArray()
    {
    }

    /**
     * Initialize directly from a C style array.
     *
     * The array needs to still have all the type information attached and
     * must not have degraded to a pointer type.
     *
     * Example:
     * \code
     * uint8_t array[7];
     * FixedSizeArray<uint8_t, 7> wrappedArray(array);
     * \endcode
     *
     * \param array
     *      Array with should be wrapped.
     */
    explicit inline
    FixedSizeArray(T (&array)[N])
    {
        memcpy(mData, array, sizeof(mData));
    }

    // This constructor is non-explicit to allow for a conversion from
    // const to non-const
    inline
    FixedSizeArray(const FixedSizeArray<NonConstType, N>& rhs)
    {
        memcpy(mData, rhs.mData, sizeof(mData));
    }

    /**
     * Initialize from a pointer to an array.
     *
     * Example:
     * \code
     * uint8_t array[7];
     * FixedSizeArray<uint8_t, 4> wrappedArray = FixedSizeArray<uint8_t, 4>::fromArray(array);
     * \endcode
     *
     * \param array
     *      Pointer to the first element of the array.
     * \param numberOfElements
     *      Number of elements in the array.
     */
    static inline FixedSizeArray<T, N>
    fromArray(NonConstType* array)
    {
        FixedSizeArray a;
        memcpy(a.mData, array, sizeof(a.mData));
        return a;
    }

    static inline FixedSizeArray<T, N>
    fromArray(ConstType* array)
    {
        FixedSizeArray a;
        memcpy(a.mData, array, sizeof(a.mData));
        return a;
    }

    /**
     * Get number of elements in the array.
     */
    inline size_t
    getNumberOfElements() const
    {
        return N;
    }

    /**
     * Access elements of the array.
     *
     * \code
     * FixedSizeArray<uint8_t, ...> array(...);
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
    T mData[N];
};

}

#endif // COBC_FIXED_SIZE_ARRAY_H
