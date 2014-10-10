/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef COBC_HAL_SPI_H_
#define COBC_HAL_SPI_H_

#include <cobc/hal/spi.h>
#include <stdint.h>

namespace cobc
{
namespace hal
{
class Spi
{
public:
    /** NanoMind712D spi driver interface
     *
     * author Norbert Toth
     */

    /**
     * \brief Virtual I2C interface destructor
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
