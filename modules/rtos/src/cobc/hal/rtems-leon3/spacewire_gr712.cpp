/*
 * Copyright (c) 2014, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <cstring>
#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "spacewire_gr712.h"

static const char * spwPath[] = { "/dev/grspw0", "/dev/grspw1", "/dev/grspw2",
    "/dev/grspw3", "/dev/grspw4", "/dev/grspw5" };

#define DEBUG_EN
#ifdef DEBUG_EN
#include <cstdio>
#define DEBUG	printf
static const char * devStates[6] = { "Error-reset", "Error-wait", "Ready",
    "Started", "Connecting", "Run" };
#else
#define DEBUG(...)
#endif

// Constructor of the device object
cobc::leon3::SpaceWireGR712::SpaceWireGR712(uint8_t i, uint8_t id) :
        node(i), devId(id), devHandle(0), firstTransmit(true),
        txBuff(txBuffData), txCount(0), rxCount(0), opened(false)
{
    memset(rxBuffData, 0, sizeof(rxBuffData));
}

cobc::leon3::SpaceWireGR712::~SpaceWireGR712()
{
    // Close the device port if it is still open
    if (opened)
    {
        this->close();
    }
}

bool
cobc::leon3::SpaceWireGR712::open()
{

    unsigned int tmp;

    // Preventing for opening the device twice
    if (opened == true)
        return false;

    // Enable clock for SpW 2 - 5 cores
    // 1. Unlock the specific bit corresponding to the core
    // 2. Asserting reset to the core clock
    // 3. Enabling the core clock
    // 4. De-asserting reset to the core clock
    // 5. Locking core clock register
    if (this->devId > 1)
    {
        tmp = readReg(clockGatingUnlockReg);
        tmp |= (1 << (this->devId + 1));
        writeReg(clockGatingUnlockReg, tmp);

        tmp = readReg(clockGatingResetReg);
        tmp |= (1 << (this->devId + 1));
        writeReg(clockGatingResetReg, tmp);

        tmp = readReg(clockGatingEnableReg);
        tmp |= (1 << (this->devId + 1));
        writeReg(clockGatingEnableReg, tmp);

        tmp = readReg(clockGatingResetReg);
        tmp &= (0 << (this->devId + 1));
        writeReg(clockGatingResetReg, tmp);

        tmp = readReg(clockGatingUnlockReg);
        tmp &= (0 << (this->devId + 1));
        writeReg(clockGatingUnlockReg, tmp);
    }

    // Opening the device by standard RTEMS I/O call
    devHandle = ::open(spwPath[this->devId], O_RDWR);
    if (devHandle < 0)
    {
        DEBUG("open failed: %d\n", devHandle);
        return false;
    }

    // Configure the device
    ::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_COREFREQ, 0);  // Make driver use the system clock during initialization

    // For SpW-USB IF      ->    SPW_CLK = TX_CLK / CLKDIV + 1=>100/9+1=10MHz
    // For IFF SpW         ->  SPW_CLK = TX_CLK / CLKDIV + 1=>100/3+1=25MHz
    if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_CLKDIV, 3) == -1)
    {
        DEBUG("ioctl failed: SPACEWIRE_IOCTRL_SET_CLKDIV on %s, %d\n",
                spwPath[this->devId], errno);
        return false;
    }

    if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_CLKDIVSTART, 9) == -1)
    {
        DEBUG("ioctl failed: SPACEWIRE_IOCTRL_SET_CLKDIVSTART on %s, %d\n",
                spwPath[this->devId], errno);
        return false;
    }

    // Set the size of the packets, buffers etc.
    devPacketSize.rxsize = 4500;        // Same as rxBuffData
    devPacketSize.txdsize = 4500;        // Same as txBuffData
    devPacketSize.txhsize = 4;

    if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_PACKETSIZE, &devPacketSize)
            == -1)
    {
        DEBUG("ioctl failed: SPACEWIRE_IOCTRL_SET_NODEADDR  on %s, %d\n",
                spwPath[this->devId], errno);
        return false;
    }

    if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_NODEADDR, node) == -1)
    {
        DEBUG("ioctl failed: SPACEWIRE_IOCTRL_SET_NODEADDR on %s, %d\n",
                spwPath[this->devId], errno);
        return false;
    }

    // Change blocking mode for transmitter
    ::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_TXBLOCK, 0);  // Transmitter is non blocking by default

    // Keep the source address in the received data packets, only needed to talk to AMAP on office model
    ::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_KEEP_SOURCE, 1);

    // Read timeout in [ms]
    ::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_READ_TIMEOUT, rTimeout);  // Sets the timeout for the read blocking mode

    DEBUG("Device opened successfully...\n");
    opened = true;
    return true;
}

void
cobc::leon3::SpaceWireGR712::close()
{
    int retval;
    unsigned int tmp;

    if (opened)
    {
        // Disable clock for SpW 2 - 5 cores
        // 1. Unlock the specific bit corresponding to the core
        // 2. Asserting reset to the core clock
        // 3. Disabling the core clock
        // 4. Locking core clock register
        if (this->devId > 1)
        {
            tmp = readReg(clockGatingUnlockReg);
            tmp |= (1 << (this->devId + 1));
            writeReg(clockGatingUnlockReg, tmp);

            tmp = readReg(clockGatingResetReg);
            tmp |= (1 << (this->devId + 1));
            writeReg(clockGatingResetReg, tmp);

            tmp = readReg(clockGatingEnableReg);
            tmp &= (0 << (this->devId + 1));
            writeReg(clockGatingEnableReg, tmp);

            tmp = readReg(clockGatingUnlockReg);
            tmp &= (0 << (this->devId + 1));
            writeReg(clockGatingUnlockReg, tmp);
        }
        retval = ::close(devHandle);
        if (retval >= 0)
        {
            opened = false;
        }
        devHandle = 0;
        DEBUG("Device %s closed\n", spwPath[this->devId]);
    }
}

void
cobc::leon3::SpaceWireGR712::up(Blocking blockingMode)
{
    // 1. Set the link to start
    // 2. Repeat to bring the linkWait if required for link to be up
    int lstatus;
    int counter = 500;        // Will wait for 5 secs in blocking mode

    DEBUG("Trying to bring link up\n");

    ::ioctl(devHandle, SPACEWIRE_IOCTRL_LINKSTART, 0);

    if (blockingMode == blocking)
    {
        do
        {
            ::ioctl(devHandle, SPACEWIRE_IOCTRL_GET_LINK_STATUS, &lstatus);

            if (lstatus != 5)
            {
                ::ioctl(devHandle, SPACEWIRE_IOCTRL_LINKSTART, 0);
            }
            else
            {
                ::ioctl(devHandle, SPACEWIRE_IOCTRL_START, -1);  // 3rd argument is timeout in ticks, -1: default timeout of 10 ms
                break;
            }
            counter--;
        }
        while (counter != 0);  //TODO: Loop should be bounded with timing parameters
    }
    else
    {
        // Bring the system in running mode, starting transmitter and receiver
        if (::ioctl(devHandle, SPACEWIRE_IOCTRL_START, 0) == -1)
        {
            printf("ioctl failed: SPACEWIRE_IOCTRL_START\n");
            return;
        }
    }
}

void
cobc::leon3::SpaceWireGR712::down(Blocking blockingMode)
{

    printf("Disabling first transmitter and receiver, then link\n");

    // Stop the transmitter and receiver
    ::ioctl(devHandle, SPACEWIRE_IOCTRL_STOP, 0);
    ::ioctl(devHandle, SPACEWIRE_IOCTRL_LINKDISABLE, 0);

    if (blockingMode == blocking)
    {
        //TODO: Extend the implementation with the help of Link err interrupt generation
    }
}

bool
cobc::leon3::SpaceWireGR712::isUp()
{
    int lstatus;

    // Getting link status
    if (::ioctl(devHandle, SPACEWIRE_IOCTRL_GET_LINK_STATUS, &lstatus) == -1)
    {
        printf("ioctl failed: SPACEWIRE_IOCTRL_GET_LINK_STATUS\n");
        return false;
    }
    printf("SpaceWire link \"%s\" is in %s state\n",
            spwPath[this->devId], devStates[lstatus]);

    return (lstatus != 5 ? false : true);
}

cobc::hal::SpaceWire::Result
cobc::leon3::SpaceWireGR712::send(TransmitBuffer * buffer)
{
    int ret;

    // Write data to the peripheral
    if ((ret = ::write(devHandle, buffer->data, buffer->length)) <= 0)
    {
        printf("Write failed, erno: %d\n", errno);
        return failure;
    }
    txCount += ret;

    return success;

}

cobc::hal::SpaceWire::Result
cobc::leon3::SpaceWireGR712::receive(ReceiveBuffer& buffer,
                                     Blocking blockingMode)
{
    (void) blockingMode;
    int ret;

    // Sets the blocking mode for the read transaction
    ::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_RXBLOCK, blockingMode);

    // Specify maximum receiver length
    buffer.length = sizeof(rxBuffData);

    // Read data to the peripheral
    if ((ret = ::read(devHandle, rxBuffData, buffer.length)) <= 0)  // Store the data from descriptors in internal buffer
    {
        printf("Read failed, erno: %d\n", errno);
        buffer.length = ret;
        return failure;
    }

    buffer.data = rxBuffData;
    buffer.length = ret;
    buffer.end = eop;
    rxCount += ret;

    return success;
}

// No buffer is needed to be reclaimed from the driver, since it is handled already in it
cobc::hal::SpaceWire::Result
cobc::leon3::SpaceWireGR712::requestBuffer(TransmitBuffer *& buffer,
                                           Blocking blockingMode)
{
    (void) blockingMode;

    // Simply passing reference of the internal buffer, TODO buffer management
    txBuff.length = 0;
    txBuff.end = eop;
    buffer = &txBuff;

    return success;
}

void
cobc::leon3::SpaceWireGR712::releaseBuffer(const ReceiveBuffer& buffer)
{
    (void) buffer;
}

void
cobc::leon3::SpaceWireGR712::flushReceiveBuffer()
{

}
