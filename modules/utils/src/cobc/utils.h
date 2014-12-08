/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.3-pre.
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

#ifndef COBC_UTILS_H
#define COBC_UTILS_H

#include "utils/deque.h"
#include "utils/pow.h"
#include "utils/list.h"
#include "utils/implicit_list.h"


namespace cobc
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

#endif    // COBC_UTILS_H
