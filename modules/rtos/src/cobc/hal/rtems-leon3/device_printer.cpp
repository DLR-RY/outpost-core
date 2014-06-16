/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */


#include <stdio.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "device_printer.h"

void cobc::leon3::DevPrinter::get_spw_config(SpaceWireGR712& obj){
	/* Getting device configuration */
	::ioctl(obj.devHandle, SPACEWIRE_IOCTRL_GET_CONFIG, &obj.devConfig);

	printf("\n");
	printf(" ******** CONFIG ********  \n");
	printf("Node Address: %d\n", obj.devConfig.nodeaddr);
	printf("Destination Key: %d\n", obj.devConfig.destkey);
	printf("Clock Divider: %d\n", obj.devConfig.clkdiv);
	printf("Rx Maximum Packet: %d\n", obj.devConfig.rxmaxlen);
	printf("Timer: %d\n", obj.devConfig.timer);
	printf("Linkdisabled: %d\n", obj.devConfig.linkdisabled);
	printf("Linkstart: %d\n", obj.devConfig.linkstart);
	printf("Disconnect: %d\n", obj.devConfig.disconnect);
	printf("Promiscuous: %d\n", obj.devConfig.promiscuous);
	printf("RMAP Enable: %d\n", obj.devConfig.rmapen);
	printf("RMAP Buffer Disable: %d\n", obj.devConfig.rmapbufdis);
	printf("Check Rmap Error: %d\n", obj.devConfig.check_rmap_err);
	printf("Remove Protocol ID: %d\n", obj.devConfig.rm_prot_id);
	printf("Blocking Transmit: %d\n", obj.devConfig.tx_blocking);
	printf("Disable when Link Error: %d\n", obj.devConfig.disable_err);
	printf("Link Error IRQ Enabled: %d\n", obj.devConfig.link_err_irq);
	printf("Link Error Event Task ID: %lu\n", static_cast<rtems_id>(obj.devConfig.event_id));
	printf("RMAP Available: %d\n", obj.devConfig.is_rmap);
	printf("RMAP CRC Available: %d\n", obj.devConfig.is_rmapcrc);
	printf("Unaligned Receive Buffer Support: %d\n", obj.devConfig.is_rxunaligned);
	printf("\n");
}

void cobc::leon3::DevPrinter::get_spw_stat(SpaceWireGR712& obj){
	/* Get device stats */
	::ioctl(obj.devHandle, SPACEWIRE_IOCTRL_GET_STATISTICS, &obj.devStatus);

	printf("\n");
	printf(" ******** STATISTICS ********  \n");
	printf("Transmit link errors: %d\n", obj.devStatus.tx_link_err);
	printf("Receiver RMAP header CRC errors: %d\n", obj.devStatus.rx_rmap_header_crc_err);
	printf("Receiver RMAP data CRC errors: %d\n", obj.devStatus.rx_rmap_data_crc_err);
	printf("Receiver EEP errors: %d\n", obj.devStatus.rx_eep_err);
	printf("Receiver truncation errors: %d\n", obj.devStatus.rx_truncated);
	printf("Parity errors: %d\n", obj.devStatus.parity_err);
	printf("Escape errors: %d\n", obj.devStatus.escape_err);
	printf("Credit errors: %d\n", obj.devStatus.credit_err);
	printf("Disconnect errors: %d\n", obj.devStatus.disconnect_err);
	printf("Write synchronization errors: %d\n", obj.devStatus.write_sync_err);
	printf("Early EOP/EEP: %d\n", obj.devStatus.early_ep);
	printf("Invalid Node Address: %d\n", obj.devStatus.invalid_address);
	printf("Packets transmitted: %d\n", obj.devStatus.packets_sent);
	printf("Packets received: %d\n", obj.devStatus.packets_received);
}

void cobc::leon3::DevPrinter::get_spw_stat(UART& obj){
	/* Get device stats */
	::ioctl(obj.devHandle, APBUART_GET_STATS, &obj.devStatus);

	printf("\n");
	printf(" ******** STATISTICS ********  \n");
	printf("HW data overrun error: %i\n", obj.devStatus.hw_dovr);
	printf("HW framing error: %i\n", obj.devStatus.hw_frame);
	printf("HW parity error: %i\n", obj.devStatus.hw_parity);
	printf("SW data overrun error: %i\n", obj.devStatus.sw_dovr);
	printf("TX count: %i\n", obj.devStatus.tx_cnt);
	printf("RX count: %i\n", obj.devStatus.rx_cnt);
}

