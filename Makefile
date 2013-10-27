
winda.o: winda.c
	avr-gcc -mmcu=atmega168 -O -o winda.o winda.c

winda.hex: winda.o
	avr-objcopy -O ihex winda.o winda.hex

upload: winda.hex
	avrdude -P /dev/ttyUSB0 -c stk500v2 -p m168 -e -Uflash:w:winda.hex:i

erease:
	avrdude -P /dev/ttyUSB0 -c stk500v2 -p m168 -e 

clean:
	rm -rf winda.o winda.hex
