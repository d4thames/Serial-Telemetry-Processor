#include <avr/io.h>
#include <avr/interrupt.h>
#include "buffer.h"

#define DEBUG_BAUD 9600
#define F_CPU 12000000

buffer8 b;
//char x;

ISR(USART0_RX_vect)
{
	buffer8_add(&b, UDR0);
}

int main(void)
{
	//x = 'y';
	// Init UART0
	UBRR0H = (F_CPU/(DEBUG_BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/(DEBUG_BAUD*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0) | _BV(TXCIE0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	sei();

	while (1) {
		while (!buffer8_rdy(&b));
		while (!(UCSR0A & _BV(UDRE0)));
		UDR0 = buffer8_pop(&b);
		//UDR0 = x;
	}
}