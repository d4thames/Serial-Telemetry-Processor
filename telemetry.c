/*

Harry Beadle
D4 Thames
Telemetry (telemetry.c)

UART0 Tx Bluetooth Host
UART0 Rx Bluetooth Host

UART1 Rx PPM Decoder (RC Data)

*/

#include "inc/telemetry.h"

ISR(USART0_RX_vect)
{
	// UART Rx, add data to buffer.
	buffer8_add(&b, UDR0);
}

int main(void)
{
	// Initalise UART0
	// Set Baud Rate.
	UBRR0H = (F_CPU/(BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/( BAUD*16L)-1);
	// Enable Tx, Rx, Tx Interupt, Rx Interupt.
	UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0) | _BV(TXCIE0);
	// 1 Start, 8 Data and 1 Stop bit.
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	// Gloally Enable interrupts.
	sei();

	// Loop Forever
	while (1) {
		// Wait for the buffer to be ready.
		while (!buffer8_rdy(&b));
		// Wait wile the UART is busy.
		while (!(UCSR0A & _BV(UDRE0)));
		// Output the contents of the buffer.
		UDR0 = buffer8_pop(&b);
	}
}