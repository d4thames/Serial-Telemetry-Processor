/*
Matt Hunter
Serial Telemetry System
D4 Thames

Outputs both RC and IMU data over bluetooth 
serial.

** INPUTS
USART0 - RC Data In
SPI    - IMU Data In

** OUTPUTS
USART1 - Bluetooth Serial Out

Matt Hunter - Inital
Harry Beadle - Cleanup
*/

#include <avr/io.h>
#include <util/delay.h>

// We need a FIFO buffer
// Each time data comes into the device we add it to the buffer.
// This is handled in two interupts (USART0RX, SPIRx)
// In main we take anything from that buffer and output it over
// USART1 (Bluetooth Module)
// Note that we will also have to handle Bluetooth pairing in this
// module.

void USART0_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH0 = (unsigned char)(baud>>8);
	UBRRL0 = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRnB = (1<<RXENn)|(1<<TXENn);
	/* Set frame format: 8data, 2stop bit */
	UCSRnC = (1<<USBSn)|(3<<UCSZn0);
}

void USART1_Init( unsigned int baud )
{
	/* Set baud rate */
	UBRRH1 = (unsigned char)(baud>>8);
	UBRRL1 = (unsigned char)baud;
	/* Enable receiver and transmitter */
	UCSRnB = (1<<RXENn)|(1<<TXENn);
	/* Set frame format: 8data, 2stop bit */
	UCSRnC = (1<<USBSn)|(3<<UCSZn0);
}

ISR(USART0_RX) // ERROR this is not the correct vector.
{
	// Add UDR0 to the FIFO Buffer
}

ISR() // SPI RX vector
{
	// Handle 16-bit things...
	// Add SPI data to the FIFO Buffer
}

int main(void)
{
	// Initalise
	sei();
	USART0_Init();
	USART1_Init();

	// Pair the Bluetooth device

	while (1) {
		// If something is in the buffer,
		// output it to USART1 if it has
		// finished sending previous byte
		if (!buffer_empty()) {
			// Buffer is not empty
			if () { // USART1 Idle
				UDR1 = buffer_pop();
			}
		}
	}

	return 1; // Unreachable
}
