/*

Harry Beadle
D4 Thames
Telemetry (telemetry.c)

Collects data from IMU and RC and outputs over a Bluetooth serial 
connection to the hosted application.

** Inputs
UART1 Serial from PPM Decoder (RC Data)
	TODO
SPI   SPI from the IMU
	MOSI Data from IMU
	SCK  Clock from IMU
	~SS  Pull LowUSART0_Rx_vect
USART0_Rx_vectUSART0_Rx_vect
** Outputs
UART0 Serial to Bluetooth Module
	TODO

*/

#include <stdint.h>
//#include "/usr/avr/include/avr/iom644pa.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buffer.h"


#define DDR_SPI DDRB
#define DD_MISO DDB5


buffer8 b;

ISR(USART0_RX_vect)
{
	// RC Data Rx,S put it in the buffer.
	buffer8_add(b, UDR0);
}

ISR(SPI_STC_vect)
{
	// SPI Data Rx, put it in the buffer.
	buffer8_add(b, SPDR);
}

void init_SPI(void)
{
	// Set MISO as an Input
	DDR_SPI = _BV(DD_MISO);
	// Enable SPI
	SPCR =_BV(SPE);
}

void init_UART0(unsigned int baud)
{
	// Set Baud Rate
	UBRR0H = (unsigned char) baud >> 8;
	UBRR0L = (unsigned char) baud;
	// Enable Tx and Rx
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	// Set frame: 8 Data, 2 Stop
	UCSR0C = _BV(USBS0) | (3 << UCSZ00);
}

void init_UART1(unsigned int baud)
{
	// Set Baud Rate
	UBRR1H = (unsigned char) baud >> 8;
	UBRR1L = (unsigned char) baud;
	// Enable Tx and Rx
	UCSR1B = _BV(RXEN1) | _BV(TXEN1);
	// Set frame: 8 Data, 2 Stop
	UCSR1C = _BV(USBS1) | (3 << UCSZ10);
}

int main(void)
{
	/* NOTE we may need to start UART1 in 9600 to initialise the 
	/* Bluetooth module. For testing with a hosted platform directly
	/* however, we don't need this. */

	// Initialise Communications
	init_SPI();
	init_UART1(14400);
	init_UART0(9600);
	// Loop Forever
	while (1) {
		// If the buffer is empty and the UART is not busy then
		// output the next item from the buffer though UART0.
		if (buffer8_rdy(b) && (UCSR0A & _BV(RXC0)))
		{
			UDR0 = buffer8_pop(b);
		}
		// Since
	}
	return 0; // Unreachable
}