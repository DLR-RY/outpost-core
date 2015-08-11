/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
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

#ifndef COBC_POW_H
#define COBC_POW_H

namespace cobc
{

/**
 * \author  Fabian Greif
 */
template <int N>
struct PowerOfTwo
{
    static const int base = 2;

    enum
    {
        value = base * PowerOfTwo<N - 1>::value
    };
};

template <>
struct PowerOfTwo<1>
{
    enum
    {
        value = 2
    };
};

template <>
struct PowerOfTwo<0>
{
    enum
    {
        value = 1
    };
};

template <>
struct PowerOfTwo<-1>
{
    enum
    {
        value = 0
    };
};

/**
 * \author  Fabian Greif
 */
template <int Base, int Exponent>
struct Pow
{
    enum
    {
        value = Base * Pow<Base, Exponent - 1>::value
    };
};

template <int Base>
struct Pow<Base, 1>
{
    enum
    {
        value = Base
    };
};

template <int Base>
struct Pow<Base, 0>
{
    enum
    {
        value = 1
    };
};

template <int Base>
struct Pow<Base, -1>
{
    enum
    {
        value = 0
    };
};

}

#endif // COBC_POW_H
