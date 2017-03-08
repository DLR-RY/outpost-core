/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef OUTPOST_UTILS_FUNCTOR_H
#define OUTPOST_UTILS_FUNCTOR_H

#include "callable.h"

namespace outpost
{

template<typename FnType>
struct FunctionSignature;

template<typename R>
struct FunctionSignature<R()>
{
    typedef R ReturnType;
};

template<typename R, typename P1>
struct FunctionSignature<R(P1)>
{
    typedef R ReturnType;
    typedef P1 Parameter1Type;
};

template<typename R, typename P1, typename P2>
struct FunctionSignature<R(P1, P2)>
{
    typedef R ReturnType;
    typedef P1 Parameter1Type;
    typedef P2 Parameter2Type;
};

/**
 * Definition of a function which is executable by telecommands.
 */
template <typename T>
class Functor1
{
    typedef typename FunctionSignature<T>::ReturnType ReturnType;
    typedef typename FunctionSignature<T>::Parameter1Type Parameter1Type;

public:
    template <typename U>
    struct FunctionType
    {
        typedef ReturnType (U::*Type)(Parameter1Type);
    };

    inline
    Functor1() :
        mObject(0),
        mFunction(0)
    {
    }

    /**
     * Bind a member function.
     *
     * \param[in]   object
     *      Has to be subclass of outpost::Callable
     * \param[in]   function
     *      Member function pointer of the subscribing class.
     */
    template <typename U>
    inline
    Functor1(U& object,
             typename FunctionType<U>::Type function) :
        mObject(reinterpret_cast<Callable *>(&object)),
        mFunction(reinterpret_cast<FunctionBaseType>(function))
    {
    }

    /**
     * Execute the contained function.
     */
    inline ReturnType
    execute(Parameter1Type parameter1) const
    {
        return (mObject->*mFunction)(parameter1);
    }

    /**
     * \see execute()
     */
    inline ReturnType
    operator()(Parameter1Type parameter1) const
    {
        return execute(parameter1);
    }

private:
    typedef ReturnType (Callable::*FunctionBaseType)(Parameter1Type);

    Callable* mObject;
    FunctionBaseType mFunction;
};

/**
 * \see outpost::Functor1
 */
template <typename T>
class Functor0
{
    typedef typename FunctionSignature<T>::ReturnType ReturnType;

public:
    template <typename U>
    struct FunctionType
    {
        typedef ReturnType (U::*Type)();
    };

    inline
    Functor0() :
        mObject(0),
        mFunction(0)
    {
    }

    template <typename U>
    inline
    Functor0(U& object,
             typename FunctionType<U>::Type function) :
        mObject(reinterpret_cast<Callable *>(&object)),
        mFunction(reinterpret_cast<FunctionBaseType>(function))
    {
    }

    inline ReturnType
    execute() const
    {
        return (mObject->*mFunction)();
    }

    inline ReturnType
    operator()() const
    {
        return execute();
    }

private:
    typedef ReturnType (Callable::*FunctionBaseType)();

    Callable* mObject;
    FunctionBaseType mFunction;
};

/**
 * \see outpost::Functor1
 */
template <typename T>
class Functor2
{
    typedef typename FunctionSignature<T>::ReturnType ReturnType;
    typedef typename FunctionSignature<T>::Parameter1Type Parameter1Type;
    typedef typename FunctionSignature<T>::Parameter2Type Parameter2Type;

public:
    template <typename U>
    struct FunctionType
    {
        typedef ReturnType (U::*Type)(Parameter1Type, Parameter2Type);
    };

    inline
    Functor2() :
        mObject(0),
        mFunction(0)
    {
    }

    template <typename U>
    inline
    Functor2(U& object,
             typename FunctionType<U>::Type function) :
        mObject(reinterpret_cast<Callable *>(&object)),
        mFunction(reinterpret_cast<FunctionBaseType>(function))
    {
    }

    inline ReturnType
    execute(Parameter1Type parameter1, Parameter2Type parameter2) const
    {
        return (mObject->*mFunction)(parameter1, parameter2);
    }

    inline ReturnType
    operator()(Parameter1Type parameter1, Parameter2Type parameter2) const
    {
        return execute(parameter1, parameter2);
    }

private:
    typedef ReturnType (Callable::*FunctionBaseType)(Parameter1Type, Parameter2Type);

    Callable* mObject;
    FunctionBaseType mFunction;
};

}

#endif
