/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef PWM_HAL_H_
#define PWM_HAL_H_

#include <stdint.h>

namespace cobc
{
namespace hal
{
/**
 * PWM Interface
 *
 * \author  Norbert Toth
 */
class Pwm
{
public:

    enum Direction
    {
        signalSet = 1, signalClear = 0
    };

    virtual
    ~Pwm() = 0;

    /**
     *
     * \param channel PWM channel
     * \param direction direction of the PWM signal
     * \param duty duty cycle of the PWM signal
     * \param freq frequency of the PWM signal
     */
    virtual void
    set(uint8_t channel = 0,
        Direction direction = signalSet,
        uint16_t duty = 0,
        uint8_t freq = 0) = 0;

};
}
}

#endif // PWM_HAL_H_
