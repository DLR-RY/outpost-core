/*
 * Copyright (c) 2016, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_MEMBER_FUNCTION_STORE_H
#define COBC_UTILS_MEMBER_FUNCTION_STORE_H

#include "callable.h"

namespace cobc
{

/**
 * Simple storage class for member functions without arguments.
 *
 * \author  Fabian Greif
 */
template <size_t N>
class MemberFunctionStore
{
public:
    template <typename C>
    struct MemberFunction
    {
        typedef void (C::*Type)(void);
    };

    /**
     * Register a new function.
     *
     * No range checking is performed.
     */
    template <typename C>
    inline void
    registerFunction(size_t index,
                     C* object,
                     typename MemberFunction<C>::Type function)
    {
        mObjects[index] = reinterpret_cast<Callable*>(object);
        mFunctions[index] = reinterpret_cast<Function>(function);
    }

    /**
     * Call a previously registered function.
     *
     * Calling a function index which has not be registered before is
     * not allowed and yields undefined behaviour.
     */
    inline void
    callFunction(size_t index)
    {
        (mObjects[index]->*mFunctions[index])();
    }

private:
    typedef void (Callable::*Function)(void);

    Callable* mObjects[N];
    Function mFunctions[N];

};

}

#endif
