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
		#include "inc/buffer.h"

		#define BAUD 9600
		#define F_CPU 12000000

		buffer8 b;

#endif