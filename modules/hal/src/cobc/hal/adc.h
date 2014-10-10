/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef ADC_HAL_H_
#define ADC_HAL_H_

#include <stdint.h>

namespace cobc
{
namespace hal
{
/**
 * ADC Interface
 *
 * \author  Norbert Toth
 */
class Adc
{
public:

    virtual
    ~Adc() = 0;

    virtual bool
    get(uint8_t AdcChannel, uint16_t* AdcValue) = 0;

};
}
}

#endif // ADC_HAL_H_
