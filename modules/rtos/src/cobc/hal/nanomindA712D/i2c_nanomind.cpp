/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#include "i2c_nanomind.h"
#include <freertos/FreeRTOS.h>

extern "C"
{
#include <dev/i2c.h>
#include <util/error.h>
}

//**************************************************************************

cobc::nanomind::I2c::I2c(uint8_t handle, uint16_t speed, uint8_t masterAddress) :
        devHandle(handle), devSpeed(speed), devMasterAddress(masterAddress)
{

}

//**************************************************************************

cobc::nanomind::I2c::~I2c()
{

}

//**************************************************************************

void
cobc::nanomind::I2c::init()
{
    i2c_init(devHandle, I2C_MASTER, devMasterAddress, devSpeed, 15, 15, NULL);
}

//**************************************************************************

bool
cobc::nanomind::I2c::masterTransaction(uint8_t address,
                                       uint8_t* outBuffer,
                                       size_t outLength,
                                       uint8_t* inBuffer,
                                       size_t inLength,
                                       time::Duration timeout)
{

    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ)
            / 1000;

    if (i2c_master_transaction(devHandle, address, outBuffer, outLength,
            inBuffer, inLength, ticks) != E_NO_ERR)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//**************************************************************************

bool
cobc::nanomind::I2c::getInputBuffer(uint8_t* buffer,
                                    size_t length,
                                    time::Duration timeout)
{

    const portTickType ticks = (timeout.milliseconds() * configTICK_RATE_HZ)
            / 1000;

    if (i2c_master_ret(devHandle, buffer, length, ticks) == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
