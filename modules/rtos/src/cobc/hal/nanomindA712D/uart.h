/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#ifndef HAL_USART_2_H_
#define HAL_USART_2_H_

#include "../serial.h"

namespace cobc
{
namespace nanomind
{
class UART : public hal::Serial
{
public:
    /** NanoMind712D UART driver interface
     *
     * author Norbert Toth
     */

    /** \param handle usart handle
     * \param fcpu CPU speed in Hz
     * \param usart_baud Baud rate in bps
     */
    UART(uint8_t handle, uint32_t FCpu, uint32_t baud);

    /**
     * \brief Virtual UART interface destructor
     *
     * Destroys the object and free resources.
     */
    virtual
    ~UART();

    /**
     *
     * \param *data
     *      Pointer to a buffer big enough to storage `length` bytes
     * \param length
     *      Number of bytes to be read
     * \param timeout
     *      Timeout if the read is blocked and no data has been read within this time
     *
     * \return  Number of bytes which could be read, maximal `length`
     */
    virtual std::size_t
    read(uint8_t* data, std::size_t length, time::Duration timeout =
                 time::Duration::max());

    /**
     *
     * \param *data
     *      Pointer to a buffer big enough to storage `length` bytes
     * \param length
     *      Number of bytes to be written
     * \param timeout
     *      Timeout if the write is blocked and not enough data bytes has been written within this time
     *
     * \return  Number of bytes which could be read, maximal `length`
     */
    virtual std::size_t
    write(const uint8_t* data, std::size_t length, time::Duration timeout =
                  time::Duration::max());

    /**
     *
     * Report number of messages waiting on the RX queue of a handle
     *
     * \return Number of char's in rx buffer
     */

    virtual size_t
    getNumberOfBytesAvailable();

    /**
     * init the uart handle
     */
    virtual void
    open();

    // not used on nanomind
    virtual void
    close();
    virtual void
    flush();
    virtual bool
    isAvailable();

private:
    uint8_t devHandle;
    uint32_t FCpuClock;
    uint32_t baudrate;
};
}
}
#endif // HAL_USART_2_H_
