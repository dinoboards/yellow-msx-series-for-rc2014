
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
CDWINDOWS := u: && cd \\home\\dean\\retro\\msxrc2014 &&

all: rom-image bin/lines.com

.PHONY: memory.jed
memory.jed: bin/memory.jed

bin/memory.jed:
	@cmd.exe /C "$(CDWINDOWS) build-jed.bat memory"

.PHONY: rom-mapper.jed
rom-mapper.jed: bin/rom-mapper.jed

bin/rom-mapper.jed:
	@cmd.exe /C "$(CDWINDOWS) build-jed.bat rom-mapper"


.PHONY: cpu-clk-selector.jed
cpu-clk-selector.jed: bin/cpu-clk-selector.jed

bin/cpu-clk-selector.jed:
	@cmd.exe /C "$(CDWINDOWS) build-jed.bat cpu-clk-selector"


.PHONY: cbios
cbios:
	@mkdir -p ./bin
	$(MAKE) -C cbios --no-print-directory derived/bin/cbios_main_rc2014.rom derived/bin/cbios_logo_msx2+.rom derived/bin/cbios_sub.rom

.PHONY: nextor
nextor: nextor/extras/xrecv.com
	@mkdir -p ./bin
	echo "Requires sudo permission"
	sudo echo
	cd nextor/source
	BUILD_TYPE=cbios $(MAKE) nextor-2.1.1-alpha2.embedded.rom -O -j --no-print-directory

install-prereq:
	@cd nextor/source
	$(MAKE) install-prereq --no-print-directory

.PHONY: rom-image
rom-image: nextor cbios
	@rm -f bin/ymsx.rom
	dd if=/dev/zero bs=16k count=8 of=bin/ymsx.rom
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_main_rc2014.rom 			of=bin/ymsx.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_msx2+.rom 			of=bin/ymsx.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_sub.rom        			of=bin/ymsx.rom bs=16k count=1 seek=4
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.embedded.rom  of=bin/ymsx.rom bs=16k count=16 seek=8

clean:
	@rm -rf ./bin
	$(MAKE) -C cbios --no-print-directory clean
	cd nextor/source
	$(MAKE) --no-print-directory clean


## Tools

VPATH = ./tools/xmodem:./bin/:./apps
PASMO := ~/pasmo-0.5.3/pasmo --public -v -I ./bin/ -I ./tools/xmodem/

bin/xrecv.com: xrecv.asm sio.asm bin/highpage.bin bdos.inc cbios.inc sio.inc ascii.inc cbios/src/hardware.asm cbios/src/hooks.asm

nextor/extras/xrecv.com: bin/xrecv.com
	cp bin/xrecv.com nextor/extras/xrecv.com

# bin/highpage.bin: highpage.asm

bin/%.com: %.asm
	@mkdir -p bin
	@$(PASMO) $< $@ $(@:.com=.sym)

bin/%.bin: %.asm
	@mkdir -p bin
	@$(PASMO) $< $@ $(@:.bin=.sym)


## Apps

bin/lines.com: lines.c v9958.c msx.asm v9958.asm
	@mkdir -p bin
	@cd apps
	zcc +msx -create-app -subtype=msxdos2 lines.c v9958.c v9958.asm msx.asm -compiler=sdcc -o lines.com
	mv lines.com ../bin/
	rm lines.img

bin/dots.com: dots.c v9958.c msx.asm v9958.asm
	@mkdir -p bin
	@cd apps
	zcc +msx -create-app -subtype=msxdos2 dots.c v9958.c v9958.asm msx.asm -compiler=sdcc -o dots.com
	mv dots.com ../bin/
	rm dots.img
