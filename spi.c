#include "inc/spi.h"

void spi_write(uint8_t address, uint8_t data)
{
	buffer8_add(&spi_tx_buffer, address);
	buffer8_add(&spi_tx_buffer, data);
}