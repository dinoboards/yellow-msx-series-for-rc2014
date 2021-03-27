
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
CDWINDOWS := u: && cd \\home\\dean\\retro\\msxrc2014 &&

all: rom-image-pal apps

jeds: mem-selector.jed rom-mapper.jed

.PHONY: mem-selector.jed
mem-selector.jed: bin/mem-selector.jed

bin/mem-selector.jed:
	@cmd.exe /C "$(CDWINDOWS) build-jed.bat mem-selector"

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
	$(MAKE) -C cbios --no-print-directory derived/bin/cbios_main_rc2014_pal.rom  derived/bin/cbios_main_rc2014_ntsc.rom derived/bin/cbios_logo_rc2014.rom derived/bin/cbios_sub.rom

.PHONY: nextor
nextor: nextor/extras/xrecv.com nextor/extras/fdisk.com nextor/extras/lines.com nextor/extras/dots.com nextor/extras/mbrot.com nextor/extras/SROM.COM nextor/extras/SROM.TXT nextor/extras/SROM.INI nextor/extras/tv.com nextor/extras/tl.com nextor/extras/memman.com nextor/extras/cfgmman.com  nextor/extras/caps.tsr
	@mkdir -p ./bin
	echo "Requires sudo permission"
	sudo echo
	cd nextor/source
	BUILD_TYPE=cbios $(MAKE) nextor-2.1.1-alpha2.rc2014.rom -O -j --no-print-directory

install-prereq:
	@cd nextor/source
	$(MAKE) install-prereq --no-print-directory

.PHONY: rom-image-pal
rom-image-pal: nextor cbios
	@rm -f bin/ymsx-pal.rom
	dd if=/dev/zero bs=16k count=8 of=bin/yellow-msx-pal-rc2104.rom
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_main_rc2014_pal.rom 	of=bin/ymsx-pal.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/ymsx-pal.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_sub.rom        			of=bin/ymsx-pal.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom    of=bin/ymsx-pal.rom bs=16k count=28 seek=4

.PHONY: rom-image-nms8250
rom-image-nms8250: nextor cbios systemroms/nms8250_basic-bios2.rom systemroms/nms8250_msx2sub.rom
	@rm -f bin/msxrc2014.rom
	dd if=/dev/zero bs=16k count=8 of=bin/nms8250-rc2014.rom
	dd conv=notrunc status=none if=./systemroms/nms8250_basic-bios2.rom					 	of=bin/nms8250-rc2014.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./systemroms/nms8250_msx2sub.rom				   			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom    of=bin/nms8250-rc2014.rom bs=16k count=28 seek=4

.PHONY: rom-image-msxsyssrc
rom-image-msxsyssrc: nextor cbios msxsys
	@rm -f bin/msxrc2014.rom
	dd if=/dev/zero bs=16k count=8 of=bin/msxsyssrc-rc2014.rom
	dd conv=notrunc status=none if=./bin/main.rom							 										of=bin/msxsyssrc-rc2014.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./bin/subrom.rom				   											of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom    of=bin/msxsyssrc-rc2014.rom bs=16k count=28 seek=4

clean:
	@rm -rf ./bin
	$(MAKE) -C cbios --no-print-directory clean
	cd nextor/source
	$(MAKE) --no-print-directory clean


## Tools

VPATH = ./tools/xmodem:./bin/:./apps
PASMO := ~/pasmo-0.5.3/pasmo --public -v -I ./bin/

bin/xrecv.com: xrecv.asm sio.asm bin/highpage.bin bdos.inc cbios.inc sio.inc ascii.inc cbios/src/hardware.asm cbios/src/hooks.asm

bin/chkspd.com: ./tools/chkspd/chkspd.asm
	mkdir -p bin
	$(PASMO) $< $@
	#$(@:.com=.sym)

nextor/extras/xrecv.com: bin/xrecv.com
	cp bin/xrecv.com nextor/extras/xrecv.com

# bin/highpage.bin: highpage.asm

bin/%.com: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@ $(@:.com=.sym)

bin/%.bin: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@ $(@:.bin=.sym)


nextor/extras/lines.com: bin/lines.com
	cp bin/lines.com nextor/extras/lines.com

nextor/extras/dots.com: bin/dots.com
	cp bin/dots.com nextor/extras/dots.com

nextor/extras/mbrot.com: bin/mbrot.com
	cp bin/mbrot.com nextor/extras/mbrot.com

nextor/extras/fdisk.com: bin/fdisk.com
	cp bin/fdisk.com nextor/extras/fdisk.com

nextor/extras/SROM.%: 3rdparty/SROM.%
	@cp "$<" "$@"

nextor/extras/%.com: 3rdparty/%.com
	@cp "$<" "$@"


nextor/extras/%.tsr: 3rdparty/%.tsr
	@cp "$<" "$@"

include Makefile-apps.mk

.PHONY: msxsys
msxsys:
	@cd msxsys-build
	$(MAKE) -O -j --no-print-directory
	cp -up ./working/*.rom ../bin/
