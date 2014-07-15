/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_CALLABLE_H
#define COBC_UTILS_CALLABLE_H

namespace cobc
{

/**
 * Empty Base class for classes with member function callbacks.
 *
 * \see    rtos::Timer
 */
class Callable
{
public:
    inline
    Callable()
    {
    }

    inline
    ~Callable()
    {
    }

    inline
    Callable(const Callable&)
    {
    }

    inline Callable&
    operator =(const Callable&)
    {
        return *this;
    }
};

}

#endif // COBC_UTILS_CALLABLE_H
