/*
UART0 Bluetooth
UART1 PPM Decoder
SPI   IMU
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "buffer.h"

// SPI Pin Definitions
#define DDR_SPI DDRB
#define DD_MISO DDB5

buffer8 b;

ISR(USART1_RX_vect)
{
	buffer8_add(b, UDR10);
}

ISR(SPI_STC_vect)
{
	buffer8_add(b, SPDR);
}

// TODO
// vo		// if (0) { //(buffer8_rdy(b)) {
		// 	// Wait for the device to be ready.
		// 	while (!(UCSR0A & _BV(UDRE0)));
		// 	// Pop and output.
		// 	UDR0 = buffer8_pop(b);
		// 	// DEBUG wait for a bit...
		// 	_delay_ms(100);
		// }		// if (0) { //(buffer8_rdy(b)) {
		// 	// Wait for the device to be ready.
		// 	while (!(UCSR0A & _BV(UDRE0)));
		// 	// Pop and output.
		// 	UDR0 = buffer8_pop(b);
		// 	// DEBUG wait for a bit...
		// 	_delay_ms(100);
		// }id init_SPI(void)
// {
// 	// Set MOSI as an Input
// 	DDR_SPI = _BV(DD_MISO);
// 	// Enable SPI
// 	SPCR =_BV(SPE);
// }

#define BAUD 9600
#define F_CPU 12000000

void init_UART0(void)
{
	// Set Baud Rate
	UBRR0H = (F_CPU/(BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/(BAUD*16L)-1);
	// Enable Tx and Rx
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	// Enable Interrupts
	//UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0);
	// Set frame: 8 Data, 2 Stop
	UCSR1C = _BV(UCSZ01) | _BV(UCSZ00);
}

void init_UART1(void)
{
	// Set Baud Rate
	UBRR1H = (F_CPU/(BAUD*16L)-1) >> 8;
	UBRR1L = (F_CPU/(BAUD*16L)-1);
	// Enable Tx and Rx
	UCSR1B = _BV(RXEN1) | _BV(TXEN1);
	// Enable Interrupts
	//UCSR0B |= _BV(RXCIE0) | _BV(TXCIE0);
	// Set frame: 8 Data, 2 Stop
	UCSR1C = _BV(UCSZ11) | _BV(UCSZ10);
}

int main(void)
{
	// Initalise UART Devices
	init_UART0();
	//init_UART1(9600);

	// Enable Interrupts
	//sei();

	// Loop Forever
	while (1) {
		// If there is somthing in the buffer, then
		// ouput it over UART1.
		if (buffer8_rdy(b)) { //(buffer8_rdy(b)) {
			// Wait for the device to be ready.
			while (!(UCSR0A & _BV(UDRE0)));
			// Pop and output.
			UDR0 = 'y'; //(char) buffer8_pop(b);
			// DEBUG wait for a bit...
			_delay_ms(100);
		}
	}
}