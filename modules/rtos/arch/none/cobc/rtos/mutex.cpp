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

#include "mutex.h"

#include <cobc/rtos/failure_handler.h>

// ----------------------------------------------------------------------------
cobc::rtos::Mutex::Mutex()
{
    // TODO
}

cobc::rtos::Mutex::~Mutex()
{
    // TODO
}

// ----------------------------------------------------------------------------
bool
cobc::rtos::Mutex::acquire()
{
    // TODO
    return false;
}

bool
cobc::rtos::Mutex::acquire(time::Duration timeout)
{
    (void) timeout;     // not used

    // TODO
    return false;
}
