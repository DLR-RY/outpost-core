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

/**
 * \file
 * \brief   Failure code and failure handler class
 *
 * \author  Fabian Greif
 */

#ifndef COBC_RTOS_FAILURE_H
#define COBC_RTOS_FAILURE_H

#include <stdint.h>

namespace cobc
{
namespace rtos
{

// FIXME create an overview of the values used here
class FailureCode
{
public:
    inline static FailureCode
    resourceAllocationFailed()
    {
        FailureCode code = FailureCode(0x0100);
        return code;
    }

    inline static FailureCode
    returnFromThread()
    {
        FailureCode code = FailureCode(0x01FF);
        return code;
    }

    inline static FailureCode
    genericRuntimeError()
    {
        FailureCode code = FailureCode(0x0200);
        return code;
    }

    inline uint32_t
    getCode() const
    {
        return mCode;
    }

protected:
    explicit inline
    FailureCode(uint32_t code) :
        mCode(code)
    {
    }

private:
    const uint32_t mCode;
};

class FailureHandler
{
public:
    typedef void (*Handler)(FailureCode code);

    /**
     * Fatal failure has occurred which makes a continuation of the
     * current execution path impossible.
     *
     * If compiled using RTEMS this function will execute the RTEMS
     * Fatal Error Manager.
     *
     * \warning This function WILL NOT RETURN!
     *
     * \param code
     *         Failure code to identify the failure.
     */
    static void
    fatal(FailureCode code);

    /**
     * Set a new failure handler.
     *
     * \warning This function must not return!
     *
     * \param newHandler
     */
    static void
    setFailureHandlerFunction(Handler newHandler);

protected:
    static Handler handler;
};

}
}

#endif // COBC_RTOS_FAILURE_H
