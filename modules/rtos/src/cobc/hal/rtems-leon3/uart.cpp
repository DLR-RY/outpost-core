/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "uart.h"

#define DEBUG_EN
#ifdef DEBUG_EN
#define DEBUG	printf
#else
#define DEBUG(...)
#endif

cobc::leon3::UART::UART(uint8_t id, uint32_t baudrate, uint8_t blocking) :
	devHandle(0),
	devId(id),
	baud(baudrate),
	devState(CLOSE),
	txCount(0),
	rxCount(0),
	blockingMode(blocking)
{
}

cobc::leon3::UART::~UART(){
    this->close();
}

void
cobc::leon3::UART::open()
{
	devHandle = ::open(cobc::leon3::uartPath[this->devId], O_RDWR);
	if(devHandle < 0){
		DEBUG("Device not opened, %s:errno-> %d\n", cobc::leon3::uartPath[this->devId], errno);
		return;
	}

    // Configure Device
    if(::ioctl(devHandle, APBUART_SET_BAUDRATE, baud) < 0) return;
    if(::ioctl(devHandle, APBUART_SET_ASCII_MODE, 0) < 0) return;
    if(::ioctl(devHandle, APBUART_CLR_STATS, 0) < 0) return;
    if(::ioctl(devHandle, APBUART_SET_RXFIFO_LEN, 64) < 0) return;
    if(::ioctl(devHandle, APBUART_SET_TXFIFO_LEN, 64) < 0) return;
    if(::ioctl(devHandle, APBUART_SET_BLOCKING, blockingMode) < 0) return;

    this->devState = OPEN;
    this->dev_enable();
	DEBUG("%s opened, %d\n", cobc::leon3::uartPath[this->devId], devState);
}

void cobc::leon3::UART::close(){
	int retval = 0;
	this->dev_disable();
	if(devState == DISABLED) {
		retval = ::close(devHandle);
		if(retval >= 0) {
			devState = CLOSE;
			DEBUG("%s closed, %d\n", cobc::leon3::uartPath[this->devId], devState);
		}
	}

}

std::size_t
cobc::leon3::UART::read(uint8_t* data, std::size_t length, time::Duration timeout){
	(void) timeout;
    std::size_t retval = 0;
    void *src = static_cast<void*> (data);

    if(devState != ENABLED || src == NULL)
        return 0;

    // Reading the byte buffer up till supplied length
    if((retval = ::read(devHandle, src, length)) <= 0)
        return retval;

    this->rxCount += retval;
    return retval;
}

std::size_t
cobc::leon3::UART::write(const uint8_t* data, std::size_t length, time::Duration timeout){
	(void) timeout;
	std::size_t ret = 0;
	uint8_t *src = const_cast<uint8_t*> (data);

	if(devState != ENABLED || src == NULL)
		return 0;

	/* Write block of data to the device handle */
	if((ret += ::write(devHandle, src, length)) <= 0)
		return ret;

	this->txCount += ret;
	return ret;
}

void
cobc::leon3::UART::flush(){
    fflush(NULL);		//FIXME: All output streams will be flushed
}

int
cobc::leon3::UART::dev_enable(){
    int retval = 0;
    if(devState == OPEN || devState == DISABLED) {
        retval = ::ioctl(devHandle, APBUART_START, 0);
        if(retval >= 0) {
            devState = ENABLED;
			DEBUG("%s enabled, %d\n", cobc::leon3::uartPath[this->devId], devState);
        }
    }
    return retval;
}

int
cobc::leon3::UART::dev_disable(){
    int retval = 0;
    if(devState != DISABLED){
        retval = ::ioctl(devHandle, APBUART_STOP, 0);
        if(retval >= 0) {
            devState = DISABLED;
			DEBUG("%s disabled, %d\n", cobc::leon3::uartPath[this->devId], devState);
        }
    }
    return retval;
}
