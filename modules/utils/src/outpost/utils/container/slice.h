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

#include <outpost/utils/meta.h>

#include <stddef.h>

#include <gsl/span>

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
template <typename ElementType>
class Slice
{
public:
    using IndexType = std::size_t;
    using LengthType = std::size_t;

    // constants and types for compatibility with STL/GSL
    using value_type = ElementType;
    using pointer = ElementType*;
    using reference = ElementType&;

    using Iterator = pointer;
    using ReverseIterator = std::reverse_iterator<pointer>;

    friend Slice<typename std::remove_const<ElementType>::type>;
    friend Slice<const ElementType>;

    /**
     * Create from a gsl::span.
     *
     * This allows creation from e.g. STL standard containers and C style
     * arrays.
     */
    inline Slice(gsl::span<ElementType> span) : mData(span.data()), mNumberOfElements(span.size())
    {
    }

    /**
     * Create from an iterator pair.
     */
    inline Slice(Iterator firstElement, Iterator lastElement) :
        mData(firstElement),
        mNumberOfElements(std::distance(firstElement, lastElement))
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
        return Slice(nullptr, IndexType(0));
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
    unsafe(ElementType* array, LengthType numberOfElements)
    {
        return Slice(array, numberOfElements);
    }

    /**
     * Get number of elements in the array.
     */
    inline LengthType
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
    inline ElementType& operator[](IndexType index) const
    {
        return mData[index];
    }

    inline Iterator
    begin() const
    {
        return Iterator(&mData[0]);
    }

    inline Iterator
    end() const
    {
        return Iterator(&mData[mNumberOfElements]);
    }

    inline ReverseIterator
    rbegin() const
    {
        return ReverseIterator(end());
    }

    inline ReverseIterator
    rend() const
    {
        return ReverseIterator(begin());
    }

    /**
     * Create a sub-slice from the beginning of the slice.
     */
    inline Slice
    first(LengthType firstElements)
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
    last(LengthType lastElements)
    {
        if (lastElements > mNumberOfElements)
        {
            // Limit to available number of elements
            // TODO this should be an error!
            return Slice(mData, mNumberOfElements);
        }
        else
        {
            return Slice(mData + (mNumberOfElements - lastElements), lastElements);
        }
    }

    /**
     * Create a sub-slice from a starting index and a length.
     */
    inline Slice
    subSlice(IndexType offset, LengthType length)
    {
        return first(offset + length).last(length);
    }

    /**
     * Create a sub-slice from two indices.
     *
     * \param   firstIndex
     *      First index which is included in the range.
     * \param   lastIndex
     *      Last index which **is not included** in range.
     */
    inline Slice
    subRange(IndexType firstIndex, IndexType lastIndex)
    {
        // TODO check that end is larger than start.
        return subSlice(firstIndex, lastIndex - firstIndex);
    }

    inline operator Slice<const ElementType>() const
    {
        return Slice<const ElementType>(mData, mNumberOfElements);
    }

    inline gsl::span<ElementType>
    asSpan()
    {
        return gsl::span<ElementType>(mData, mNumberOfElements);
    }

private:
    inline Slice(ElementType* array, size_t numberOfElements) :
        mData(array),
        mNumberOfElements(numberOfElements)
    {
    }

    /// Pointer to the array
    ElementType* mData;

    /// Size of array
    IndexType mNumberOfElements;
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
asSliceUnsafe(ElementType* ptr, typename Slice<ElementType>::IndexType count)
{
    return Slice<ElementType>::unsafe(ptr, count);
}

/**
 * Create a slice from an iterator pair.
 */
template <class ElementType>
Slice<ElementType>
asSlice(ElementType* firstElement, ElementType* lastElement)
{
    return Slice<ElementType>(firstElement, lastElement);
}

/**
 * Create slice from a C-style array.
 */
template <class ElementType, size_t N>
Slice<ElementType> asSlice(ElementType (&arr)[N])
{
    return Slice<ElementType>(arr);
}

/**
 * Create slice from a STL compatible container.
 */
template <class Container>
Slice<typename Container::value_type>
asSlice(Container& cont)
{
    return Slice<typename Container::value_type>(cont);
}

/**
 * Create slice from a constant STL compatible container.
 */
template <class Container>
Slice<const typename Container::value_type>
asSlice(const Container& cont)
{
    return Slice<const typename Container::value_type>(cont);
}

/**
 * Create slice from a gsl::span.
 */
template <class Ptr>
Slice<typename Ptr::element_type>
asSlice(Ptr& cont)
{
    return Slice<typename Ptr::element_type>(cont);
}

}  // namespace outpost

#endif
