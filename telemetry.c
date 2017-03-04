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
	// UART0 Rx from Bluetooth Host
	// Add control tuning data to control buffer.
	buffer8_add(&control_buffer, UDR0);
}

ISR(USART1_RX_vect)
{
	// UART1 RX from PPM Decoder
	// Add remote control data to output buffer.
	buffer8_add(&output_buffer, UDR1);
}

ISR(SPI_STC_vect)
{
	// Pull PB4 Low
	PORTB &= ~_BV(PB4);
	// Put Rx'd byte in rx buffer
	uint8_t spi_data = SPDR;
	if (spi_data) {
		buffer8_add(&spi_rx_buffer, SPDR);
	}
}

int main(void)
{
	// Initialise SPI
	DDRB = _BV(PB4) | _BV(PB5) | _BV(PB7);
	SPCR = _BV(SPE) | _BV(SPIE) | _BV(MSTR) | _BV(SPR0);

	// Initalise UART0 and UART1
	// Set Baud Rate.
	UBRR0H = (F_CPU/(BAUD*16L)-1) >> 8;
	UBRR1H = (F_CPU/(BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/( BAUD*16L)-1);
	UBRR1L = (F_CPU/( BAUD*16L)-1);
	// Enable Tx, Rx, Tx Interupt, Rx Interupt.
	UCSR0B = _BV(RXEN0) | _BV(RXCIE0) | _BV(TXEN0) | _BV(TXCIE0);
	UCSR1B = _BV(RXEN1) | _BV(RXCIE1) | _BV(TXEN1) | _BV(TXCIE1);
	// 1 Start, 8 Data and 1 Stop bit.
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
	UCSR1C = _BV(UCSZ10) | _BV(UCSZ11);
	// Gloally Enable interrupts.
	sei();

	// Initalise the SPI device
	spi_write(WRITE & MPUREG_USER_CTRL, BIT_I2C_IF_DIS);
	spi_write(WRITE & MPUREG_INT_PIN_CFG, 0x22);
	spi_write(WRITE & MPUREG_PWR_MGMT_1, BIT_H_RESET);
	spi_write(WRITE & MPUREG_PWR_MGMT_1, 0x01);
	spi_write(READ & MPUREG_WHOAMI, 0x00);

	// Loop Forever
	while (1) {
		// Wait for the output buffer to be ready.
		if (buffer8_rdy(&output_buffer)) {
			// Wait wile the UART is busy.
			while (!(UCSR0A & _BV(UDRE0)));
			// Output the contents of the buffer.
			UDR0 = buffer8_pop(&output_buffer);
		}
		if (buffer8_rdy(&spi_rx_buffer)) {
			while (!(UCSR0A & _BV(UDRE0)));
			UDR0 = buffer8_pop(&spi_rx_buffer);
		}
		if (!(SPSR & _BV(SPIF)) && buffer8_rdy(&spi_tx_buffer)) {
			PORTB |= _BV(PB4);
			SPDR = buffer8_pop(&spi_tx_buffer);
		}	
	}
}