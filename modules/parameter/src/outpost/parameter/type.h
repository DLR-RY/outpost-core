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

#ifndef OUTPOST_PARAMETER_TYPE_H_
#define OUTPOST_PARAMETER_TYPE_H_

#include <outpost/time.h>

#include <stdint.h>

#include <type_traits>

namespace outpost
{
namespace parameter
{
class Type;

template <typename T>
class Marker
{
    friend class Type;

private:
    static int mark;
};

template <typename T>
int Marker<T>::mark;

class Type
{
public:
    /**
     * Get the type by setting the template parameter
     *
     * @param T  Template parameter to choose the type
     * @return the Type for T
     */
    template <typename T>
    static Type
    getType()
    {
        typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
        Type t;
        t.markPointer = &Marker<type>::mark;
        return t;
    }

    /**
     * Get the type from a variable
     *
     * @param T  Template parameter to choose the type
     * @return the Type for T
     */
    template <typename T>
    static Type
    getType(const T& /*t*/)
    {
        typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
        Type t;
        t.markPointer = &Marker<type>::mark;
        return t;
    }

    bool
    operator==(const Type& o) const
    {
        return markPointer == o.markPointer;
    }

    bool
    operator!=(const Type& o) const
    {
        return markPointer != o.markPointer;
    }

private:
    Type() = default;
    int* markPointer = nullptr;
};

}  // namespace parameter
}  // namespace outpost

#endif
