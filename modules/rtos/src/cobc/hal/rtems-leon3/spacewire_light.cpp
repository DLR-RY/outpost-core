/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include <stdio.h>

#include "spacewire_light.h"

// ----------------------------------------------------------------------------
cobc::leon3::SpaceWireLight::SpaceWireLight(uint32_t index) :
	index(index), handle(0), firstTransmit(true),
	txSync(1),
	txBuffer(txBufferData)
{
}

cobc::leon3::SpaceWireLight::~SpaceWireLight()
{
	// Close port if it is still open
	if (handle != 0) {
		close();
	}
}

// ----------------------------------------------------------------------------
bool
cobc::leon3::SpaceWireLight::open()
{
	const struct spwl_options options =
	{
	    4,		// Number of receive buffers to allocate.
	    4,		// Number of transmit buffers to allocate.
	    4500,	// Size of each receive buffer in bytes.
	    4500	// Size of each allocated transmit buffer in bytes.
	};

	rtems_status_code status = spwl_open(&handle, index, &options);
	if (status != RTEMS_SUCCESSFUL) {
		goto error;
	}

	// TODO
	// This function sets the TX bit rate to
	//   (txclkfreq / (scaler + 1)) bits per second;
	//   where txclkfreq is determined by the hardware configuration of the core.
	spwl_set_linkspeed(handle, 1);

	return true;
error:
	return false;
}

void
cobc::leon3::SpaceWireLight::close()
{
	spwl_close(handle);
	handle = 0;
}

// ----------------------------------------------------------------------------
void
cobc::leon3::SpaceWireLight::up(Blocking blocking)
{
	spwl_set_linkmode(handle, SPWL_LINKMODE_START);

	if (blocking == BLOCKING) {
		unsigned int mask = SPWL_COND_LINKUP;
		spwl_wait(handle, &mask, 0);
	}
}

void
cobc::leon3::SpaceWireLight::down(Blocking blocking)
{
	spwl_set_linkmode(handle, SPWL_LINKMODE_DISABLE);

	if (blocking == BLOCKING) {
		unsigned int mask = SPWL_COND_LINKDOWN;
		spwl_wait(handle, &mask, 0);
	}
}

bool
cobc::leon3::SpaceWireLight::isUp()
{
	spwl_linkstatus status;
	unsigned int errors;

	spwl_get_linkstatus(handle, &status, &errors);

	return (status == SPWL_LINK_RUN);
}

// ----------------------------------------------------------------------------
cobc::hal::SpaceWire::Result
cobc::leon3::SpaceWireLight::requestBuffer(TransmitBuffer *& buffer, Blocking blocking)
{
//	if (!txSync.acquire()) {
//		return FAILURE;
//	}
	(void) blocking;	// TODO

	// TODO buffer management
	if (!firstTransmit) {
		struct spwl_txbuf * reclaimedBuffer;
		rtems_status_code status = spwl_reclaim_txbuf(handle, &reclaimedBuffer, SPWL_WAIT);
		if (status != RTEMS_SUCCESSFUL) {
			return FAILURE;
		}
	}
	else {
		firstTransmit = false;
	}

	txBuffer.length = 0;
	txBuffer.end = EOP;
	buffer = &txBuffer;

	return SUCCESS;
}

cobc::hal::SpaceWire::Result
cobc::leon3::SpaceWireLight::send(TransmitBuffer * buffer)
{
	spwlBuffer.data = buffer->data;
	spwlBuffer.nbytes = buffer->length;
	spwlBuffer.eop = eopFlags(buffer->end);

	rtems_status_code status = spwl_send_txbuf(handle, &spwlBuffer, SPWL_WAIT);

	if (status == RTEMS_SUCCESSFUL) {
		return SUCCESS;
	}
	else {
		return FAILURE;
	}
}

cobc::hal::SpaceWire::Result
cobc::leon3::SpaceWireLight::receive(ReceiveBuffer& buffer, Blocking blocking)
{
	(void) blocking;	// TODO

	void * data;
	uint16_t bytesReceived;
	unsigned int eopFlags;

	rtems_status_code status =
			spwl_recv_rxbuf(handle, &data, &bytesReceived, &eopFlags, SPWL_WAIT);

	if (status == RTEMS_SUCCESSFUL) {
		buffer.data = static_cast<const uint8_t *>(data);
		buffer.length = bytesReceived;
		buffer.end = eopMarker(eopFlags);
		return SUCCESS;
	}
	else {
		return FAILURE;
	}
}

void
cobc::leon3::SpaceWireLight::releaseBuffer(const ReceiveBuffer& buffer)
{
	// Const cast needed here because the spwl interface requires a
	// writeable void pointer. The const is constraint is introduced by
	// this class so we can safely remove it.
	spwl_release_rxbuf(handle, const_cast<uint8_t*>(buffer.data));
}

void
cobc::leon3::SpaceWireLight::flushReceiveBuffer()
{

}
