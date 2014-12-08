/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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

#ifndef COBC_HAL_GPIO_H_
#define COBC_HAL_GPIO_H_

#include <stdint.h>

namespace cobc
{
namespace hal
{
/**
 * GPIO Interface
 *
 * \author  Norbert Toth
 */
class Gpio
{
public:

    enum Direction
    {
        input = 0, output = 1
    };

    virtual
    ~Gpio() = 0;

    /**
     * Initialise IO gpio
     * \param gpio number
     * \param direction set as output or set as input
     */
    virtual void
    initialize(uint8_t gpio, Direction direction) = 0;

    /**
     * Set IO gpio high
     * \param gpio number
     */
    virtual void
    set(uint8_t gpio) = 0;

    /**
     * Set IO gpio low
     * \param gpio number
     */
    virtual void
    clear(uint8_t gpio) = 0;

    /**
     * Read value of IO gpio
     * \param gpio number
     * \return value of IO gpio
     */
    virtual bool
    get(uint8_t gpio) = 0;
};
}
}

#endif // COBC_HAL_GPIO_H_
