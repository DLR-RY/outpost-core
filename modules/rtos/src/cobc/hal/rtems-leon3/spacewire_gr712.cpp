/*
 * spacewire_gr712.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: Muhammad Bassam
 */
#include <cstring>
#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "spacewire_gr712.h"

const char * devStates[6] = {
		"Error-reset",
		"Error-wait",
		"Ready",
		"Started",
		"Connecting",
		"Run"
};

/* Constructor of the device object */
cobc::leon3::SpaceWireGR712::SpaceWireGR712(uint8_t i, char * name):
	node(i),
	devHandle(0),
	firstTransmit(true),
	txBuff(txBuffData),
	txCount(0),
	rxCount(0),
	opened(false)
{
	strcpy(devName, name);
	devName[19] = '\0';
	std::memset(rxBuffData, 0, sizeof(rxBuffData));
}

cobc::leon3::SpaceWireGR712::~SpaceWireGR712(){
	// Close the device port if it is still open
	if(opened) {
		this->close();
	}
}

bool cobc::leon3::SpaceWireGR712::open(){

	/* Preventing for opening the device twice */
	if(opened == true)
		return false;


	/* Opening the device by standard RTEMS I/O call */
	devHandle = ::open(devName, O_RDWR);
	if(devHandle < 0){
		printf("open failed: %d\n", devHandle);
		return false;
	}


	/* Configure the device */
	/* Make driver use the system clock during initialization */
	if(::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_COREFREQ, 0) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_COREFREQ on %s\n", devName);
		return false;
	}

	/* For SpW-USB IF  	->	SPW_CLK = TX_CLK / CLKDIV + 1=>100/9+1=10MHz */
	/* For IFF SpW 		->  SPW_CLK = TX_CLK / CLKDIV + 1=>100/3+1=25MHz */
	if(::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_CLKDIV, 3) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_CLKDIV on %s, %d\n", devName, errno);
		return false;
	}

	if(::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_CLKDIVSTART, 9) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_CLKDIVSTART on %s, %d\n", devName, errno);
		return false;
	}

	/* Set the size of the packets, buffers etc. */
	devPacketSize.rxsize = 4500;		// Same as rxBuffData
	devPacketSize.txdsize = 4500;		// Same as txBuffData
	devPacketSize.txhsize = 4;

	if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_PACKETSIZE, &devPacketSize) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_NODEADDR  on %s, %d\n", devName, errno);
		return false;
	}

	if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_NODEADDR, node) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_NODEADDR on %s, %d\n", devName, errno);
		return false;
	}

	/* Change blocking mode for transmitter */
	if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_TXBLOCK, 0) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_TXBLOCK on %s, %d\n", devName, errno);
		return false;
	}

	/* Change blocking mode for receiver, todo: integrate with original blocking and non-blocking descriptors */
	if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_RXBLOCK, 0) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_RXBLOCK on %s, %d\n", devName, errno);
		return false;
	}

	/* Keep the source address in the received data packets, only needed to talk to AMAP on office model */
	if (::ioctl(devHandle, SPACEWIRE_IOCTRL_SET_KEEP_SOURCE, 1) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_SET_KEEP_SOURCE on %s, %d\n", devName, errno);
		return false;
	}


	/* Getting device configuration */
	if (::ioctl(devHandle, SPACEWIRE_IOCTRL_GET_CONFIG, &devConfig) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_GET_CONFIG on %s, %d\n", devName, errno);
		return false;
	}

	/* Print the configuration */
	//print_config();

	printf("Device opened successfully...\n");
	opened = true;
	return true;
}

void cobc::leon3::SpaceWireGR712::close(){
	int retval;

	if(opened){
		retval = ::close(devHandle);
		if(retval >= 0) {
			opened = false;
		}
		devHandle = 0;
		printf("Device %s closed\n", devName);
	}
}

bool cobc::leon3::SpaceWireGR712::isOpened(){
	return opened;
}

void cobc::leon3::SpaceWireGR712::up(Blocking blockingMode){
	/* 1. Set the link to start
	 * 2. Repeat to bring the linkWait if required for link to be up
	 * */
	int lstatus;
	int counter = 500;		// Will wait for 5 secs in blocking mode

	printf("Trying to bring link up\n");

	::ioctl(devHandle, SPACEWIRE_IOCTRL_LINKSTART, 0);

	if(blockingMode == blocking){
		do{
			::ioctl(devHandle, SPACEWIRE_IOCTRL_GET_LINK_STATUS, &lstatus);

			if(lstatus != 5){
				::ioctl(devHandle, SPACEWIRE_IOCTRL_LINKSTART, 0);
			}
			else{
				::ioctl(devHandle, SPACEWIRE_IOCTRL_START, -1);	// 3rd argument is timeout in ticks, -1: default timeout of 10 ms
				break;
			}
			counter--;
		}while(counter != 0);		//TODO: Loop should be bounded with timing parameters
	}
	else{
		/* Bring the system in running mode, starting transmitter and receiver */
		if (::ioctl(devHandle, SPACEWIRE_IOCTRL_START, 0) == -1) {
			printf("ioctl failed: SPACEWIRE_IOCTRL_START\n");
			return;
		}
	}
}

void cobc::leon3::SpaceWireGR712::down(Blocking blockingMode){

	printf("Disabling first transmitter and receiver, then link\n");

	/* Stop the transmitter and receiver */
	::ioctl(devHandle, SPACEWIRE_IOCTRL_STOP, 0);
	::ioctl(devHandle, SPACEWIRE_IOCTRL_LINKDISABLE, 0);

	if(blockingMode == blocking){
	//TODO: Extend the implementation with the help of Link err interrupt generation
	}
}

bool cobc::leon3::SpaceWireGR712::isUp(){
	int lstatus;

	/* Getting link status */
	if(::ioctl(devHandle, SPACEWIRE_IOCTRL_GET_LINK_STATUS, &lstatus) == -1) {
		printf("ioctl failed: SPACEWIRE_IOCTRL_GET_LINK_STATUS\n");
		return false;
	}
	printf("Spacewire link \"%s\" is in %s state\n", devName, devStates[lstatus]);

	return (lstatus != 5 ? false : true);		//TODO: Give useful conditional return value here for link status
}

cobc::hal::SpaceWire::Result cobc::leon3::SpaceWireGR712::send(TransmitBuffer * buffer){
	int ret;

	/* Write data to the peripheral */
	if((ret = ::write(devHandle, buffer->data, buffer->length)) <= 0 ){
		printf("Write failed, erno: %d\n", errno);
		return failure;
	}
	txCount += ret;

	return success;

}

cobc::hal::SpaceWire::Result cobc::leon3::SpaceWireGR712::receive(ReceiveBuffer& buffer, Blocking blockingMode){
	(void) blockingMode;
	int ret;

	/* Read data to the peripheral */
	if((ret = ::read(devHandle, rxBuffData, buffer.length)) <= 0)	// Store the data from descriptors in internal buffer
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
cobc::hal::SpaceWire::Result cobc::leon3::SpaceWireGR712::requestBuffer(TransmitBuffer *& buffer, Blocking blockingMode){
	(void) blockingMode;

	// Simply passing reference of the internal buffer, TODO buffer management
	txBuff.length = 0;
	txBuff.end = eop;
	buffer = &txBuff;

	return success;
}

void cobc::leon3::SpaceWireGR712::releaseBuffer(const ReceiveBuffer& buffer){
	(void) buffer;
}

void cobc::leon3::SpaceWireGR712::flushReceiveBuffer(){

}

void cobc::leon3::SpaceWireGR712::print_config(){
	printf("\n");
	printf(" ******** CONFIG ********  \n");
	printf("Node Address: %d\n", devConfig.nodeaddr);
	printf("Destination Key: %d\n", devConfig.destkey);
	printf("Clock Divider: %d\n", devConfig.clkdiv);
	printf("Rx Maximum Packet: %d\n", devConfig.rxmaxlen);
	printf("Timer: %d\n", devConfig.timer);
	printf("Linkdisabled: %d\n", devConfig.linkdisabled);
	printf("Linkstart: %d\n", devConfig.linkstart);
	printf("Disconnect: %d\n", devConfig.disconnect);
	printf("Promiscuous: %d\n", devConfig.promiscuous);
	printf("RMAP Enable: %d\n", devConfig.rmapen);
	printf("RMAP Buffer Disable: %d\n", devConfig.rmapbufdis);
	printf("Check Rmap Error: %d\n", devConfig.check_rmap_err);
	printf("Remove Protocol ID: %d\n", devConfig.rm_prot_id);
	printf("Blocking Transmit: %d\n", devConfig.tx_blocking);
	printf("Disable when Link Error: %d\n", devConfig.disable_err);
	printf("Link Error IRQ Enabled: %d\n", devConfig.link_err_irq);
	printf("Link Error Event Task ID: %lu\n", static_cast<rtems_id>(devConfig.event_id));
	printf("RMAP Available: %d\n", devConfig.is_rmap);
	printf("RMAP CRC Available: %d\n", devConfig.is_rmapcrc);
	printf("Unaligned Receive Buffer Support: %d\n", devConfig.is_rxunaligned);
	printf("\n");

}

void cobc::leon3::SpaceWireGR712::print_statistics(){

	/* Get device stats */
	::ioctl(devHandle, SPACEWIRE_IOCTRL_GET_STATISTICS, &devStatus);

	printf("\n");
	printf(" ******** STATISTICS ********  \n");
	printf("Transmit link errors: %d\n", devStatus.tx_link_err);
	printf("Receiver RMAP header CRC errors: %d\n", devStatus.rx_rmap_header_crc_err);
	printf("Receiver RMAP data CRC errors: %d\n", devStatus.rx_rmap_data_crc_err);
	printf("Receiver EEP errors: %d\n", devStatus.rx_eep_err);
	printf("Receiver truncation errors: %d\n", devStatus.rx_truncated);
	printf("Parity errors: %d\n", devStatus.parity_err);
	printf("Escape errors: %d\n", devStatus.escape_err);
	printf("Credit errors: %d\n", devStatus.credit_err);
	printf("Disconnect errors: %d\n", devStatus.disconnect_err);
	printf("Write synchronization errors: %d\n", devStatus.write_sync_err);
	printf("Early EOP/EEP: %d\n", devStatus.early_ep);
	printf("Invalid Node Address: %d\n", devStatus.invalid_address);
	printf("Packets transmitted: %d\n", devStatus.packets_sent);
	printf("Packets received: %d\n", devStatus.packets_received);
}
