#ifndef _SPI_H_
	#define _SPI_H_

		#include <avr/io.h>
		
		#include "buffer.h"

		#define READ 0xFF
		#define WRITE 0x7F

		buffer8 spi_tx_buffer;
		buffer8 spi_rx_buffer;

		void spi_write(uint8_t address, uint8_t data);

		#include "../spi.c"

#endif
