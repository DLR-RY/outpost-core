/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
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
