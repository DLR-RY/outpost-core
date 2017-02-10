/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
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

#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include <stdint.h>

namespace cobc
{
/**
 * Wrapper around C-style error codes.
 *
 * Should be used as a replacement for raw \c int return codes. Can be
 * extended by own error codes by sub-classing this class and defining
 * new static functions (like \c success() or \c error()). The name of these
 * functions define the name of the corresponding error code. The user is
 * responsible to ensure that error codes are unique.
 *
 * \author  Fabian Greif
 */
class ErrorCode
{
    // These type and member function is needed to implement the
    // safe bool idiom (see e.g. http://www.artima.com/cppsource/safeboolP.html).
    //
    // A normal cast operator to bool would enable some invalid sematics
    // for this class. For a C++11 a explicit cast operator could be used
    // instead.
    typedef void (ErrorCode::*bool_type)() const;

    // LCOV_EXCL_START
    void
    this_type_does_no_support_comparisons() const
    {
    }
    // LCOV_EXCL_STOP

public:
    /**
     * Indicate a successful operation.
     */
    static inline ErrorCode
    success()
    {
        return ErrorCode(0);
    }

    /**
     * Indicate a successful operation with a success code.
     *
     * \param  successCode
     *
     * \return
     */
    static inline ErrorCode
    success(uint16_t successCode)
    {
        return ErrorCode(successCode);
    }

    /**
     * Indicate a generic error.
     */
    static inline ErrorCode
    error()
    {
        return ErrorCode(-1);
    }

    /**
     * Indicate a error with a specific error code.
     *
     * \param errorCode
     *        Error code specifying the error that appeared. The error code
     *        must be negative, \c -1 is reserved for a generic error.
     */
    static inline ErrorCode
    error(int16_t errorCode)
    {
        return ErrorCode(errorCode);
    }

    ErrorCode(const ErrorCode& other) :
        mErrorCode(other.mErrorCode)
    {
    }

    /**
     * This class can be directly used in a boolean expression.
     * A positive value (true) indicates a successful operation.
     */
    //
    // When using C++11 this function can be rewritten as:
    //
    //     inline explicit
    //     operator bool() const
    //     {
    //         return isSuccess();
    //     }
    //
    operator bool_type() const
    {
        return (isSuccess() ? &ErrorCode::this_type_does_no_support_comparisons : 0);
    }

    /**
     * Check if the operation was successful.
     *
     * \retval \c true  if the operation was successful.
     * \retval \c false if the operation failed.
     */
    inline bool
    isSuccess() const
    {
        return (mErrorCode >= 0);
    }

    inline bool
    isError() const
    {
        return (mErrorCode < 0);
    }

    /**
     * Query the error/success code.
     *
     * Every value smaller than zero is an error. \c -1 indicates a
     * generic error.
     *
     * \return  Error code.
     */
    inline int16_t
    getCode() const
    {
        return mErrorCode;
    }

    inline bool
    operator==(const ErrorCode& other) const
    {
        return (mErrorCode == other.mErrorCode);
    }

    inline bool
    operator!=(const ErrorCode& other) const
    {
        return (mErrorCode != other.mErrorCode);
    }

    inline bool
    operator!() const
    {
        return !isSuccess();
    }

protected:
    explicit ErrorCode(int16_t code) :
        mErrorCode(code)
    {
    }

private:
    int16_t mErrorCode;
};
}

#endif // ERROR_CODE_H
