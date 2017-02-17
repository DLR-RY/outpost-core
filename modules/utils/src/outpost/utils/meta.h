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
 * - 2013-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_META_H
#define COBC_META_H

namespace outpost
{

/**
 * Empty class used as dummy
 */
class Nil
{
};

/**
 * Selects type Result = (flag == true) ? T : U
 */
template <bool flag, typename T, typename U>
struct Select
{
    typedef T Result;
};

template <typename T, typename U>
struct Select<false, T, U>
{
    typedef U Result;
};


template<typename T>
struct remove_const
{
    typedef T type;
};

template<typename T>
struct remove_const<T const volatile>
{
    typedef typename remove_const<T>::type volatile type;
};

template<typename T>
struct remove_const<T volatile>
{
    typedef typename remove_const<T>::type volatile type;
};

template<typename T>
struct remove_const<T const>
{
    typedef typename remove_const<T>::type type;
};

template<typename T>
struct remove_const<T&>
{
    typedef typename remove_const<T>::type& type;
};

template<typename T>
struct remove_const<T*>
{
    typedef typename remove_const<T>::type* type;
};

}

#endif
