/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */


#ifndef DEVICE_PRINTER_H_
#define DEVICE_PRINTER_H_

#include <stdint.h>
#include <cstddef>

#include <cobc/utils.h>

#include "spacewire_gr712.h"
#include "uart.h"

namespace cobc
{
	namespace leon3
	{
		/**
		 * Device Status and Configuration printer class, mainly used for
		 * debugging.
		 *
		 * @author	Muhammad Bassam
		 */
		class DevPrinter
		{
		public:
			/**
			 * Creates the device printer object
			 */
			DevPrinter(){}

			/**
			 * Destroy the object and close the device if opened
			 */
			~DevPrinter(){}

			/**
			 * Print the configuration of the SpaceWire device for
			 * debugging purpose only
			 *
			 * \param [obj]
			 * 	Reference to the spacewire object
			 */
			void
			get_spw_config(SpaceWireGR712& obj);

			/**
			 * Print the statistics of the SpaceWire device for
			 * debugging purpose only
			 *
			 * \param [obj]
			 * 	Reference to the spacewire object
			 */
			void
			get_spw_stat(SpaceWireGR712& obj);

			/**
			 * Print the statistics of the UART device for
			 * debugging purpose only
			 *
			 * \param [obj]
			 * 	Reference to the spacewire object
			 */
			void
			get_spw_stat(UART& obj);

		};
	}
}




#endif /* DEVICE_PRINTER_H_ */
