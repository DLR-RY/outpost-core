/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef I2C_NANOMIND_H_
#define I2C_NANOMIND_H_

#include "../i2c.h"

namespace cobc
{
namespace nanomind
{
class I2c : public hal::I2c
{
public:
    /** NanoMind712D UART driver interface
     *
     * author Norbert Toth
     */

    /**
     * \param handle        Which I2C bus
     * \param speed         Bus speed in kbps
     * \param masterAddress Own slave address
     */
    I2c(uint8_t handle, uint16_t speed, uint8_t masterAddress);

    /**
     * \brief Virtual I2C interface destructor
     *
     * Destroys the object and free resources.
     */
    virtual
    ~I2c();

    /**
     * Init I2C interface
     */
    virtual void
    init();

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
                      uint16_t timeout);

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
    getInputBuffer(uint8_t* buffer, size_t length, uint16_t timeout);

    /**
     *
     * \return handle number
     */
    virtual inline uint8_t
    getHandle()
    {
        return devHandle;
    }

private:
    uint8_t devHandle;
    uint16_t devSpeed;
    uint8_t devMasterAddress;
};
}
}

#endif // I2C_NANOMIND_H_
