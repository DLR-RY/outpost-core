/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef I2C_H_
#define I2C_H_

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
     * Init I2C interface
     */
    virtual void
    init() = 0;

    /**
     *
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
                      time::Duration timeout = time::Duration::max()) = 0;

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
    getInputBuffer(uint8_t* buffer, size_t length, time::Duration timeout =
                           time::Duration::max()) = 0;

    /**
     *
     * \return handle number
     */
    virtual inline uint8_t
    getHandle() = 0;

};
}
}

#endif // I2C_H_
