/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_SLICE_H
#define OUTPOST_SLICE_H

#include <stddef.h>

#include <gsl/span>

#include <outpost/utils/meta.h>

namespace outpost
{

/**
 * Slices are a dynamically-sized view into a contiguous sequence of memory.
 *
 * Slices are a view into a block of memory represented as a pointer and
 * a length. The implementation here is inspired by [Rust slices][1] and
 * the [`span<T>` type][2] defined in the [C++ Core Guidelines][3].
 *
 * [1]: https://doc.rust-lang.org/std/slice/
 * [2]: http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-views
 * [3]: http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
 *
 * \author  Fabian Greif
 */
template <typename T>
class Slice
{
public:
    typedef typename outpost::remove_const<T>::type NonConstType;
    typedef const NonConstType ConstType;

    friend class Slice<NonConstType>;
    friend class Slice<ConstType>;

    inline
    Slice(gsl::span<T> span) :
        mData(span.data()),
        mNumberOfElements(span.size())
    {
    }

    Slice(const Slice& rhs) = default;

    Slice(Slice&& rhs) = default;

    Slice&
    operator=(const Slice& rhs) = default;

    Slice&
    operator=(Slice&& rhs) = default;

    /**
     * Generate an empty array.
     */
    static inline Slice
    empty()
    {
        return Slice(nullptr, 0);
    }

    /**
     * Initialize from a pointer to an array.
     *
     * \warning
     *      This operation is unsafe and should be avoided if possible. Create
     *      a Slice directly from the original array and pass that around
     *      instead. The slice can be split into smaller chunks by using
     *      first() and last().
     *
     * Example:
     * \code
     * uint8_t* ptr = ...;
     * auto slice = Slice<uint8_t>::unsafe(ptr, 7);
     * \endcode
     *
     * \param array
     *      Pointer to the first element of the array.
     * \param numberOfElements
     *      Number of elements in the array.
     */
    static inline Slice
    unsafe(T* array, size_t numberOfElements)
    {
        return Slice(array, numberOfElements);
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
     * Slice<uint8_t> array(...);
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

    /**
     * Create a sub-slice from the beginning of the slice.
     */
    inline Slice
    first(size_t firstElements)
    {
        if (firstElements > mNumberOfElements)
        {
            // Limit to available number of elements
            // TODO this should be an error!
            return Slice(mData, mNumberOfElements);
        }
        else
        {
            return Slice(mData, firstElements);
        }
    }

    /**
     * Create a sub-slice from the end of the slice.
     */
    inline Slice
    last(size_t lastElements)
    {
        if (lastElements > mNumberOfElements)
        {
            // Limit to available number of elements
            // TODO this should be an error!
            return Slice(mData, mNumberOfElements);
        }
        else
        {
            return Slice(mData + (mNumberOfElements - lastElements),
                         lastElements);
        }
    }

    inline
    operator Slice<const T>() const
    {
        return Slice<const T>(mData, mNumberOfElements);
    }

private:
    inline
    Slice(T* array, size_t numberOfElements) :
        mData(array),
        mNumberOfElements(numberOfElements)
    {
    }

    /// Pointer to the array
    T* mData;

    /// Size of array
    size_t mNumberOfElements;
};

/**
 * Initialize from a pointer to an array.
 *
 * \warning
 *      This operation is unsafe and should be avoided if possible. Create
 *      a Slice directly from the original array and pass that around
 *      instead. The slice can be split into smaller chunks by using
 *      first() and last().
 */
template <class ElementType>
Slice<ElementType>
asUnsafeSlice(ElementType* ptr, typename Slice<ElementType>::index_type count)
{
    return Slice<ElementType>::unsafe(ptr, count);
}

template <class ElementType>
Slice<ElementType>
asSlice(ElementType* firstElem, ElementType* lastElem)
{
    return Slice<ElementType>(firstElem, lastElem);
}

template <class ElementType, size_t N>
Slice<ElementType>
asSlice(ElementType (&arr)[N])
{
    return Slice<ElementType>(arr);
}

template <class Container>
Slice<typename Container::value_type>
asSlice(Container &cont)
{
    return Slice<typename Container::value_type>(cont);
}

template <class Container>
Slice<const typename Container::value_type>
asSlice(const Container &cont)
{
    return Slice<const typename Container::value_type>(cont);
}

template <class Ptr>
Slice<typename Ptr::element_type>
asSlice(Ptr& cont, std::ptrdiff_t count)
{
    return Slice<typename Ptr::element_type>(cont, count);
}

template <class Ptr>
Slice<typename Ptr::element_type>
asSlice(Ptr& cont)
{
    return Slice<typename Ptr::element_type>(cont);
}

}

#endif
