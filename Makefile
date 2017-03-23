COMPILE = avr-gcc -std=c99 -Wall -pedantic -Os -Iusbdrv -I. -mmcu=atmega168p -DF_CPU=1000000UL

OBJECTS = takehito.o

AVRDUDE = avrdude -p m168 -P usb -c usbasp -U flash:w:takehito.hex
#-U hfuse:w:0xDF:m -U lfuse:w:0xEF:m

# symbolic targets:
all:	takehito.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE)

# ATMEGA88 fuse bytes (remember that they are active low)

# Fuse high byte:
# 0xDF = 1 1 0 1   1 1 1 1 <-- BODLEVEL0
#        ^ ^ ^ ^   ^ ^ ^------ BODLEVEL1
#        | | | |   | +-------- BODLEVEL2
#        | | | |   + --------- EESAVE
#        | | | +-------------- WDTON
#        | | +---------------- SPIEN
#        | +------------------ DWEN
#        +-------------------- RSTDISBL
# Fuse low byte:
# 0xEF = 1 1 1 0   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0
#        | |  +--------------- SUT 1..0
#        | +------------------ CKOUT
#        +-------------------- CKDIV8

clean:
	rm -f takehito.hex .lst takehito.obj takehito.cof takehito.list takehito.map takehito.eep.hex takehito.bin *.o usbdrv/*.o takehito.s usbdrv/*.s

# file targets:
takehito.bin:	$(OBJECTS)
	$(COMPILE) -o takehito.bin $(OBJECTS)

takehito.hex:	takehito.bin
	rm -f takehito.hex takehito.eep.hex
	avr-objcopy -j .text -j .data -O ihex takehito.bin takehito.hex
#	./checksize takehito.bin
# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:	takehito.bin
	avr-objdump -d takehito.bin

#cpp:
#	$(COMPILE) -E takehito.c
