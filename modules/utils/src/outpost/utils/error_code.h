/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2013-2018, Fabian Greif (DLR RY-AVS)
 * - 2015, Janosch Reinking (DLR RY-AVS)
 * - 2018, Rhea Rinaldo (DLR RY-AVS)
 */

#ifndef ERROR_CODE_H
#define ERROR_CODE_H

#include <stdint.h>

namespace outpost
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
public:
    /**
     * Indicate a successful operation.
     */
    static inline constexpr ErrorCode
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
    static inline constexpr ErrorCode
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
    static inline constexpr ErrorCode
    error(int16_t errorCode)
    {
        return ErrorCode(errorCode);
    }

    constexpr ErrorCode(const ErrorCode& other) : mErrorCode(other.mErrorCode)
    {
    }

    /**
     * This class can be directly used in a boolean expression.
     * A positive value (true) indicates a successful operation.
     */
    inline explicit constexpr operator bool() const
    {
        return isSuccess();
    }

    /**
     * Check if the operation was successful.
     *
     * \retval \c true  if the operation was successful.
     * \retval \c false if the operation failed.
     */
    inline constexpr bool
    isSuccess() const
    {
        return (mErrorCode >= 0);
    }

    inline constexpr bool
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
    inline constexpr int16_t
    getCode() const
    {
        return mErrorCode;
    }

    inline constexpr bool
    operator==(const ErrorCode& other) const
    {
        return (mErrorCode == other.mErrorCode);
    }

    inline constexpr bool
    operator!=(const ErrorCode& other) const
    {
        return (mErrorCode != other.mErrorCode);
    }

    inline constexpr bool operator!() const
    {
        return !isSuccess();
    }

protected:
    explicit constexpr ErrorCode(int16_t code) : mErrorCode(code)
    {
    }

private:
    int16_t mErrorCode;
};
}  // namespace outpost

#endif  // ERROR_CODE_H
