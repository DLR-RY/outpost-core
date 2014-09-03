/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------
#include "uart.h"

extern "C"
{
#include <dev/usart.h>
}

//**************************************************************************

cobc::nanomind::UART::UART(uint8_t handle, uint32_t FCpu, uint32_t baud) :
        devHandle(handle), FCpuClock(FCpu), baudrate(baud)
{

}

//**************************************************************************

cobc::nanomind::UART::~UART()
{

}

//**************************************************************************

void
cobc::nanomind::UART::open()
{
    usart_init(devHandle, FCpuClock, baudrate);
}

//**************************************************************************

std::size_t
cobc::nanomind::UART::read(uint8_t* data,
                           std::size_t length,
                           time::Duration timeout)
{
    char* src = reinterpret_cast<char*>(data);

    for (uint16_t i = 0; i < length; i++)
        src[i] = usart_getc(devHandle);

    return length;
}

//**************************************************************************

std::size_t
cobc::nanomind::UART::write(const uint8_t* data,
                            std::size_t length,
                            time::Duration timeout)
{
    uint8_t *temp = const_cast<uint8_t*>(data);
    char* src = reinterpret_cast<char*>(temp);

    usart_putstr(devHandle, src, length);

    return length;
}

//**************************************************************************

std::size_t
cobc::nanomind::UART::getNumberOfBytesAvailable(void)
{
    return usart_messages_waiting(devHandle);
}

//**************************************************************************

void
cobc::nanomind::UART::close()
{

}
void
cobc::nanomind::UART::flush()
{
}
bool
cobc::nanomind::UART::isAvailable()
{
    return true;
}
