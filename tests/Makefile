# $Id: Makefile,v 1.1 2014/05/02 22:33:16 david Exp $
#---------------------------------------------------------------
#             CONFIGURE THESE VARIABLES IF NEEDED
#---------------------------------------------------------------

ROOT = ${HOME}/compiladores/root
RTS_LIB_DIR = $(ROOT)/usr/lib

COMPILER=../fir
ASM=yasm

#---------------------------------------------------------------
#    PROBABLY, NO NEED TO CHANGE ANYTHING BEYOND THIS POINT
#---------------------------------------------------------------

COMPILER_FLAGS = 
LDFLAGS  = -m elf_i386 -L$(RTS_LIB_DIR) -lrts

SOURCES     = $(wildcard *.fir)
OBJECTS     = $(SOURCES:%.fir=%.o)
EXECUTABLES = $(SOURCES:%.fir=%)

#---------------------------------------------------------------
#                DO NOT CHANGE AFTER THIS LINE
#---------------------------------------------------------------

all: $(EXECUTABLES)

%.c: %.fir
	$(COMPILER) $< -o $@

%.asm: %.fir
	$(COMPILER) $< -o $@

%.o: %.asm
	$(ASM) -felf32 $< -o $@

%:%.o
	$(LD) $< -o results/$@ $(LDFLAGS)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLES) *.c *.asm

#---------------------------------------------------------------
#                           THE END
#---------------------------------------------------------------
