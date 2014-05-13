/**
 * @file
 * @author Muhammad Bassam
 *
 * @copyright DLR - German Aerospace Center
 *
 * @brief RTEMS SPW Interface for GR712RC LEON3-FT processor
 *
 * File presents the interface to the GRSPW driver of the LEON3-FT processor for
 * GR712RC development board. This will replace the SpaceWireLight driver interface
 * used for the office model
 *
 */

#ifndef SPACEWIRE_GR712_H_
#define SPACEWIRE_GR712_H_

#include <stdint.h>
#include <cstddef>

#include <cobc/rtos/semaphore.h>
#include <cobc/utils.h>

#include "../spacewire.h"		// SpaceWire interface object for external access to the driver

extern "C" {
#include "../../../../ext/rtems-leon3/grspw/spw.h"		//GRSPW driver modification file
}

static void drv_register(){
	extern void grspw_drv_register();
	grspw_drv_register();
}

namespace cobc
{
	namespace leon3
	{
	/*!
	 * SpaceWire Driver Interface for GR712 ASIC
	 *
	 * Uses the SpaceWire GRSPW driver for RTEMS 4.10 on LEON3 from
	 * RTEMS library
	 *
	 * @see		cobc::hal::Spacewire
	 * @author	Muhammad Bassam
	 */
	class SpaceWireGR712 : public hal::SpaceWire
	{
	public:
		/*! \fn SpaceWireGR712(uint8_t i, char * name)
		 * \brief SpW interface constructor
		 *
		 * Creates the SpW object with the device found in the
		 * driver table and assign the node ID. Receive/Transmit
		 * buffers are allocated statically.
		 *
		 * \param [node]
		 * 		Node address of the device
		 * \param [name]
		 * 		Path of the device to access through system IO calls
		 */
		SpaceWireGR712(uint8_t i, char * name);

		/**
		 * Destroy the object and close the device if opened
		 */
		virtual ~SpaceWireGR712();

		/** \fn open()
		 * \brief Opens the device for read-write accesses
		 *
		 * Device will be configured in successive steps after
		 * successfully opening it and current device configuration
		 * is being stored
		 *
		 * \return
		 * 		[True] if the device is opened successfully
		 * 		[False] error in opening
		 */
		virtual bool open();

		/** \fn close()
		 * \brief Close the device if opened
		 */
		virtual void close();

		/** \fn up(Blocking blockingMode = nonBlocking)
		 * \brief Brings the link up
		 *
		 * Default is non blocking. In blocking mode the link
		 * status has been checked and TX/RX is are forced to
		 * start for specific timeout interval
		 *
		 * \param blockingMode
		 * 		Specify if the link should block during transmission
		 * 		or reception if no data is available
		 */
		virtual void up(Blocking blockingMode = nonBlocking);

		/** \fn down(Blocking blockingMode = nonBlocking)
		 * \brief Shut down the link
		 *
		 * \param blockingMode
		 * 		Specify if the link should block during transmission
		 * 		or reception if no data is available
		 */
		virtual void down(Blocking blockingMode = nonBlocking);

		/** \fn isUp()
		 * \brief Verify if the link is up
		 *
		 * Before proceeding the transactions on the link, it has
		 * to be verified if entered in Running state. Otherwise
		 * the driver will return with error messages
		 *
		 * \param blockingMode
		 * 		Specify if the link should block during transmission
		 * 		or reception if no data is available
		 * \return
		 * 		[True] if the device is opened successfully
		 * 		[False] error in opening
		 */
		virtual bool isUp();

		/** \fn requestBuffer(TransmitBuffer *& buffer, Blocking blockingMode = blocking)
		 * \brief Request a send buffer
		 *
		 * A pointer to the transmit buffer has been passed for fill up. Currently it is
		 * a minimal implementation without any buffer management
		 *
		 * \param[out]	buffer
		 * 		Pointer reference to a send buffer
		 * \param[in]	blockingMode
		 * 		Default is blocking mode.
		 * \return
		 * 		Success
		 */
		virtual Result requestBuffer(TransmitBuffer *& buffer, Blocking blockingMode = blocking);

		/** \fn send(TransmitBuffer * buffer)
		 * \brief Send the data block
		 *
		 * Sending the data over SpW link by using RTEMS standard IO calls
		 *
		 * \param buffer
		 * 		Pointer to the transmit buffer object
		 * \return
		 * 		[Success] for sending specified data bytes
		 * 		[Failure] for failed to send specified data bytes
		 */
		virtual Result send(TransmitBuffer * buffer);

		/** \fn receive(ReceiveBuffer& buffer, Blocking blockingMode = blocking)
		 * \brief Receive the data block
		 *
		 * Receiving the data over SpW link by using RTEMS standard IO calls
		 *
		 * \param buffer
		 * 		Reference to the receive buffer object
		 * \param blockingMode
		 * 		Block the reception (Currently implemented in open() method call)
		 * \return
		 * 		[Success] for sending specified data bytes
		 * 		[Failure] for failed to send specified data bytes
		 */
		virtual Result receive(ReceiveBuffer& buffer, Blocking blockingMode = blocking);

		/** \fn releaseBuffer(const ReceiveBuffer& buffer)
		 * \brief Intended to be used for releasing the receiving buffer
		 *
		 * Not implemented. As the buffer is internally handled in the
		 * GRSPW driver provided by the vendor and provide on option for
		 * releasing it
		 *
		 * \param buffer
		 * 		Reference to the receive buffer object
		 */
		virtual void releaseBuffer(const ReceiveBuffer& buffer);

		virtual void flushReceiveBuffer();

		void print_config();
		void print_statistics();
		bool isOpened();

	private:
		/* Spacewire device access info */
		uint8_t node;										/**< SpaceWire node address */
		char devName[20];									/**< Path of SpaceWire device */
		int devHandle;										/**< SpaceWire access device handle */
		//rtos::Semaphore txLock;
		bool firstTransmit;

		/* TX & RX buffer */
		TransmitBuffer txBuff;								/**< Transmit buffer object */
		uint8_t txBuffData[4500] ATTRIBUTE_ALIGNED(4);		/**< Transmit buffer place holder */
		uint8_t rxBuffData[4500] ATTRIBUTE_ALIGNED(4);		/**< Receive buffer place holder */

		/* Status information */
		uint32_t txCount;									/**< Transmit data counter */
		uint32_t rxCount;									/**< Receive data counter */
		bool opened;
		spw_stats devStatus;								/**< Device status information */

		/* Spacewire device configuration */
		spw_config devConfig;								/**< Device configuration */
		spw_ioctl_packetsize devPacketSize;					/**< Device packet configuration */
		//spw_ioctl_pkt_send devPacketSend;
	};

	}
}




#endif /* SPACEWIRE_GR712_H_ */
