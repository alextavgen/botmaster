CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS= -g -mmcu=attiny45 -I.

.c.x :
	avr-gcc -c ${CFLAGS} -S -o $@ $<


rom.hex : main.out
	$(OBJCOPY) -j .text -O ihex main.out rom.hex
main.out : main.o 
	$(CC) $(CFLAGS) -o main.out -Wl,-Map,main.map main.o 
main.o : main.c
	$(CC) $(CFLAGS) -O -c main.c
	
all: rom.hex

clean: 
	rm *.o rom.hex main.map main.out

loadold: all
	avrdude -p m88  -c pony-stk200 -e -Uflash:w:rom.hex

loadmkII: all
	avrdude -p m88  -c avrispmkII -P usb -b 115200 -e -Uflash:w:rom.hex

load: all
	sudo avrdude -p attiny45  -c usbtiny  -e -Uflash:w:rom.hex
