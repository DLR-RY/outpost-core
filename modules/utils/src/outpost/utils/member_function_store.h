/*
 * Copyright (c) 2016-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2016-2017, Fabian Greif (DLR RY-AVS)
 * - 2016, Benjamin Weps (DLR SC-SRV)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_MEMBER_FUNCTION_STORE_H
#define OUTPOST_UTILS_MEMBER_FUNCTION_STORE_H

#include <stddef.h>
#include "callable.h"

namespace outpost
{

template <typename Signature>
class MemberFunctionSignature;

template <typename R>
class MemberFunctionSignature<R()>
{
public:
    template <typename C>
    struct MemberFunction
    {
        typedef R (C::*Type)();
    };

    typedef R (Callable::*RawFunctionType)();

    typedef R ReturnType;
};

template <typename R, typename Arg1>
class MemberFunctionSignature<R(Arg1)>
{
public:
    template <typename C>
    struct MemberFunction
    {
        typedef R (C::*Type)(Arg1);
    };

    typedef R (Callable::*RawFunctionType)(Arg1);

    typedef R ReturnType;
    typedef Arg1 Arg1Type;
};

template <typename R, typename Arg1, typename Arg2>
class MemberFunctionSignature<R(Arg1, Arg2)>
{
public:
    template <typename C>
    struct MemberFunction
    {
        typedef R (C::*Type)(Arg1, Arg2);
    };

    typedef R (Callable::*RawFunctionType)(Arg1, Arg2);

    typedef R ReturnType;
    typedef Arg1 Arg1Type;
    typedef Arg1 Arg2Type;
};

/**
 * Storage class for member functions.
 *
 * \author  Fabian Greif
 */
template <size_t N, typename Signature>
class MemberFunctionStore
{
public:
    typedef typename MemberFunctionSignature<Signature>::RawFunctionType RawFunctionType;

    /**
     * Register a new function.
     *
     * No range checking is performed.
     */
    template <typename C>
    inline void
    registerFunction(size_t index,
                     C* object,
                     typename MemberFunctionSignature<Signature>::template MemberFunction<C>::Type function)
    {
        mObjects[index] = reinterpret_cast<Callable*>(object);
        mFunctions[index] = reinterpret_cast<RawFunctionType>(function);
    }

    /**
     * Call a previously registered function.
     *
     * Calling a function index which has not be registered before is
     * not allowed and yields undefined behaviour.
     */
    inline typename MemberFunctionSignature<Signature>::ReturnType
    callFunction(size_t index)
    {
        return (mObjects[index]->*mFunctions[index])();
    }

    template <typename T>
    inline typename MemberFunctionSignature<Signature>::ReturnType
    callFunction(size_t index,
                 typename MemberFunctionSignature<T>::Arg1Type arg1)
    {
        return (mObjects[index]->*mFunctions[index])(arg1);
    }

    template <typename T>
    inline typename MemberFunctionSignature<Signature>::ReturnType
    callFunction(size_t index,
                 typename MemberFunctionSignature<T>::Arg1Type arg1,
                 typename MemberFunctionSignature<T>::Arg2Type arg2)
    {
        return (mObjects[index]->*mFunctions[index])(arg1, arg2);
    }

private:
    Callable* mObjects[N];
    RawFunctionType mFunctions[N];
};

}

#endif
