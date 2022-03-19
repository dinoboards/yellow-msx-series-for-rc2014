SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ZCCRELFLAGS=
ifdef RELEASE
ZCCRELFLAGS=-SO3 --max-allocs-per-node200000 --allow-unsafe-read
endif

LIBS := -I./../apps/libraries/msxbios/ -I./../apps/libraries/delay/
# -I./fdisk/ -I./telnet/ -I./libraries/fusion/ -I./libraries/msxbios -I./libraries/msxdos -I./libraries/fossil  -I./libraries/delay -I./xrecv2

SRC := ./cexten/

ZCC := zcc +msx  -subtype=msxdos2 -startup=1 -crt0 $(SRC)crt.asm -compiler=sdcc  -Cs --Werror -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1-DAMALLOC1 -pragma-define:CRT_MODEL=0 -pragma-define:CRT_COMMANDLINE_REDIRECTION=0 $(ZCCRELFLAGS) $(LIBS)

$(BINDIR)spike.bin: $(BINDIR)debuggin.c.o $(BINDIR)print.o $(BINDIR)print.c.o $(BINDIR)usb-host-init.c.o $(BINDIR)work-area.o ../apps/libraries/delay/delay.c ../apps/libraries/msxbios/system_vars.c

$(BINDIR)usb-host-init.c.asm: $(SRC)usb-host-init.c $(SRC)work-area.h
$(BINDIR)debuggin.c.asm: $(SRC)debuggin.c $(SRC)debuggin.h

$(BINDIR)%.bin:
	@mkdir -p $(dir $@)
	$(ZCC) $(filter-out %.inc,$^) -o $@
	echo "Linked $(notdir $@) from $(notdir $^)"

define assemble
	@mkdir -p $(dir $@)
	$(ZCC) --compile-only $< -o $@
	echo "Assembled $(notdir $@) from $(notdir $<)"
endef

define compile
	@mkdir -p $(dir $@)
	$(ZCC) --c-code-in-asm --assemble-only $< -o $@
	echo "Compiled $(notdir $@) from $(notdir $<)"
endef

define objects
 $(patsubst %.asm,%.o,$(patsubst %.c,%.c.o,$(1)))
endef



.PRECIOUS: $(BINDIR)%.c.asm
$(BINDIR)%.c.asm: $(SRC)%.c; $(compile)
$(BINDIR)%.o: $(BINDIR)%.asm; $(assemble)
$(BINDIR)%.o: $(SRC)%.asm; $(assemble)
$(BINDIR)%.m4.o: %.asm.m4; $(assemble)

