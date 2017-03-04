/*

Harry Beadle
D4 Thames
Telemetry (telemetry.h)

Header file for telemetry system.

*/

#ifndef _TELEMETRY_H_
	#define _TELEMETRY_H_

		#include <avr/io.h>
		#include <avr/interrupt.h>

		#include "buffer.h"
		#include "MPU9250.h"
		#include "spi.h"

		#define BAUD 9600
		#define F_CPU 12000000

		buffer8 output_buffer;
		buffer8 control_buffer;

#endif
