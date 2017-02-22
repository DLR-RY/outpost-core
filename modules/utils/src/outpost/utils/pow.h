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

#ifndef OUTPOST_POW_H
#define OUTPOST_POW_H

namespace outpost
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

#endif
