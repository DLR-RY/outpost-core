
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "uart.h"

cobc::leon3::UART::UART(char * path, uint32_t baud, uint8_t blocking, serial_parity_t par){
    this->devHandle = 0;
    strcpy(devName, path);
    devName[19] = '\0';
    this->baud = baud;
    this->devState = CLOSE;
    this->parity = par;
    txCount = 0;
    rxCount = 0;
    blockingMode = blocking;
}

cobc::leon3::UART::~UART(){
    this->dev_disable();
    this->close();
}

void cobc::leon3::UART::open(){
    devHandle = ::open(devName, O_RDWR);
    if(devHandle < 0){
        printf("Device not opened, %s:errno-> %d\n", devName, errno);
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
    printf("%s opened, %d\n", devName, devState);
}

void cobc::leon3::UART::close(){
    int retval = 0;
    if(devState == DISABLED) {
        retval = ::close(devHandle);
        if(retval >= 0) {
            devState = CLOSE;
            printf("%s closed, %d\n", devName, devState);
        }
    }
}

bool cobc::leon3::UART::isAvailable(void){
    if(devState == OPEN || devState == ENABLED)
        return true;
    return false;
}

std::size_t cobc::leon3::UART::read(uint8_t* data, std::size_t length, time::Duration timeout){
    std::size_t retval = 0;
    void *src = (void*)data;

    if(devState != ENABLED || src == NULL)
        return 0;

    // Reading the byte buffer up till supplied length 
    if((retval = ::read(devHandle, src, length)) <= 0)
        return retval;

    this->rxCount += retval;
    return retval;
}

std::size_t cobc::leon3::UART::write(const uint8_t* data, std::size_t length, time::Duration timeout){
    std::size_t ret = 0;
    uint8_t *src = (uint8_t*) data;

    if(devState != ENABLED || src == NULL)
        return 0;

    // Write block of data to the device handle 
    if((ret += ::write(devHandle, src, length)) <= 0)
        return ret;

    this->txCount += ret;
    return ret;
}

void cobc::leon3::UART::flush(){
    fflush(NULL);
}

int cobc::leon3::UART::dev_enable(){
    int retval = 0;
    if(devState == OPEN || devState == DISABLED) {
        retval = ::ioctl(devHandle, APBUART_START, 0);
        if(retval >= 0) {
            devState = ENABLED;
            printf("%s enabled, %d\n", devName, devState);
        }
    }
    return retval;
}

int cobc::leon3::UART::dev_disable(){
    int retval = 0;
    if(devState != DISABLED){
        retval = ::ioctl(devHandle, APBUART_STOP, 0);
        if(retval >= 0) {
            devState = DISABLED;
            printf("%s disabled, %d\n", devName, devState);
        }
    }
    return retval;
}

void cobc::leon3::UART::print_status(){
    // Get device stats 
    ::ioctl(devHandle, APBUART_GET_STATS, &devStatus);

    printf("\n");
    printf(" ******** STATISTICS ********  \n");
    printf("HW data overrun error: %i\n", devStatus.hw_dovr);
    printf("HW framing error: %i\n", devStatus.hw_frame);
    printf("HW parity error: %i\n", devStatus.hw_parity);
    printf("SW data overrun error: %i\n", devStatus.sw_dovr);
    printf("TX count: %i\n", devStatus.tx_cnt);
    printf("RX count: %i\n", devStatus.rx_cnt);
}


