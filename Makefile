prog: telemetry.c
	avr-gcc -mmcu=atmega644p -DFCPU=12000000 -Wall -Os telemetry.c buffer.c  -o prog.elf
	avr-objcopy -O ihex prog.elf prog.hex
	sudo avrdude -c usbasp -p m644p -U flash:w:prog.hex
