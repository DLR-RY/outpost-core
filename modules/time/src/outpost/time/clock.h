/*
 * Copyright (c) 2014-2017, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2014-2017, Fabian Greif (DLR RY-AVS)
 */
// ----------------------------------------------------------------------------

#ifndef COBC_TIME_CLOCK_H
#define COBC_TIME_CLOCK_H

#include "time_point.h"

namespace outpost
{
namespace time
{

/**
 * Clock interface.
 *
 * \author  Fabian Greif
 */
class Clock
{
public:
    Clock();

    virtual
    ~Clock();

    virtual SpacecraftElapsedTime
    now() const = 0;

private:
    // disable copy constructor
    Clock(const Clock&);

    // disable assignment operator
    Clock&
    operator=(const Clock&);
};

}
}

#endif // COBC_TIME_CLOCK_H
