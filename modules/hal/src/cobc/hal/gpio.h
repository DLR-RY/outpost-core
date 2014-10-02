/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
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

    virtual
    ~Gpio() = 0;

    /**
     * Initialise IO gpio
     * \param gpio number
     * \param output 1 = set as output, 0 = set as input
     */
    virtual void
    initGpio(uint8_t gpio, uint8_t output) = 0;

    /**
     * Set IO gpio high
     * \param gpio number
     */
    virtual void
    setGpio(uint8_t gpio) = 0;

    /**
     * Set IO gpio low
     * \param gpio number
     */
    virtual void
    clearGpio(uint8_t gpio) = 0;

    /**
     * Read value of IO gpio
     * \param gpio number
     * \return value of IO gpio
     */
    virtual bool
    getGpio(uint8_t gpio) = 0;
};
}
}

#endif // COBC_HAL_GPIO_H_
