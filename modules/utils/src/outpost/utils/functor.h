/*
 * Copyright (c) 2017-2022, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2017-2018, Fabian Greif (DLR RY-AVS)
 * - 2022, Tobias Pfeffer (DLR RY-AVS)
 */

#ifndef OUTPOST_UTILS_FUNCTOR_H
#define OUTPOST_UTILS_FUNCTOR_H

#include <outpost/base/callable.h>
#include <outpost/utils/operation_result.h>

namespace outpost
{
template <typename Signature>
class Functor;

/**
 * Definition of a function which is executable by telecommands.
 */
template <typename R, typename... As>
class Functor<R(As...)>
{
    typedef R (Callable::*FunctionBaseType)(As...);

    typedef R ReturnType;

    using OperationResult = outpost::utils::OperationResult;

public:
    template <typename C>
    struct FunctionType
    {
        typedef R (C::*Type)(As...);
    };

    inline Functor() : mObject(nullptr), mFunction(nullptr)
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
    template <typename C>
    inline Functor(C& object, typename FunctionType<C>::Type function) :
        mObject(reinterpret_cast<Callable*>(&object)),
        mFunction(reinterpret_cast<FunctionBaseType>(function))
    {
    }

    /**
     * Checks if the functor is valid.
     *
     * @return True: if object or member function pointer are not set, otherwise false
     */
    bool
    isEmpty() const
    {
        return (mObject == nullptr || mFunction == nullptr);
    }

    /**
     * Execute the contained function.
     * The first parameter is used to check if the result is a valid value.
     *
     * @param valid True if call was valid.
     */
    inline ReturnType
    execute(OperationResult& valid, As... args) const
    {
        if (!isEmpty())
        {
            valid = OperationResult::success;
            return (mObject->*mFunction)(args...);
        }
        valid = OperationResult::invalid;
        return ReturnType();
    }

/**
 * Execute the contained function.
 *
 * @warning Legacy interface that does not NOT check if the functor is valid.
 */
#if __cplusplus >= 201402L
    [[deprecated("Use result checking version instead.")]]
#endif
    inline ReturnType
    execute(As... args) const
    {
        return (mObject->*mFunction)(args...);
    }

    /**
     * \see execute() with check parameter
     */
    inline ReturnType
    operator()(OperationResult& valid, As... args) const
    {
        return execute(valid, args...);
    }

/**
 * \see execute()
 */
#if __cplusplus >= 201402L
    [[deprecated("Use result checking version instead.")]]
#endif
    inline ReturnType
    operator()(As... args) const
    {
        return execute(args...);
    }

protected:
    Callable* mObject;
    FunctionBaseType mFunction;
};

}  // namespace outpost

#endif
