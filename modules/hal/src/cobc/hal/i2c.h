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

#ifndef I2C_HAL_H_
#define I2C_HAL_H_

#include <cstddef>
#include <stdint.h>

#include <cobc/time/duration.h>

namespace cobc
{
namespace hal
{
/**
 * I2C Interface
 *
 * \author  Norbert Toth
 */
class I2c
{
public:
    virtual
    ~I2c() = 0;

    /**
     * I2C master transaction.
     *
     * I2C master device send a block of data and read a block of data.
     *
     * \param address       I2C address of the slave node
     * \param *outBuffer    pointer to output buffer
     * \param outLength     output length
     * \param *inBuffer     pointer to input buffer
     * \param inLength      input length
     * \param timeout       timeout in milliseconds
     * \retval true         transaction success
     * \retval false        transaction failed
     */
    virtual bool
    masterTransaction(uint8_t address,
                      uint8_t* outBuffer,
                      size_t outLength,
                      uint8_t* inBuffer,
                      size_t inLength,
                      cobc::time::Duration timeout = time::Duration::maximum()) = 0;

    /**
     *  Read data from input buffer
     *
     *  \param *buffer  pointer to a buffer
     *  \param length   number of the bytes to be read
     *  \param timeout  timeout in milliseconds
     *  \retval true    read success
     *  \retval false   read failed
     */
    virtual bool
    getInputBuffer(uint8_t* buffer,
                   size_t length,
                   cobc::time::Duration timeout = time::Duration::maximum()) = 0;

};
}
}

#endif // I2C_HAL_H_
