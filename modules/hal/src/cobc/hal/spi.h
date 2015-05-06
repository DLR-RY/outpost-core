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

#ifndef COBC_HAL_SPI_H_
#define COBC_HAL_SPI_H_

#include <stdint.h>

namespace cobc
{
namespace hal
{

/**
 * SPI driver interface.
 *
 * \author Norbert Toth
 */
class Spi
{
public:
    /**
     *
     *
     * Destroys the object and free resources.
     */
    virtual
    ~Spi();

    virtual void
    write(uint16_t byte) = 0;

    virtual uint16_t
    read() = 0;

    virtual void
    initDev(void* spidDev) = 0;

    virtual uint8_t
    setupChip(void* spiChip) = 0;
};

}
}

#endif // COBC_HAL_SPI_H_
