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

template <typename T>
class BoundedArray
{
public:
    template <size_t N>
    inline
    BoundedArray(T (&array)[N]) :
        mData(array),
        mNumberOfElements(N)
    {
    }

    inline
    BoundedArray(T* array, size_t numberOfElements) :
        mData(array),
        mNumberOfElements(numberOfElements)
    {
    }

    inline size_t
    getSize() const
    {
        return mNumberOfElements;
    }

    inline T&
    operator[](size_t index)
    {
        return mData[index];
    }

    inline const T&
    operator[](size_t index) const
    {
        return mData[index];
    }

private:
    T* mData;
    size_t mNumberOfElements;
};

}

#endif // COBC_BOUNDED_ARRAY_H
