SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
CDWINDOWS := u: && cd \\home\\dean\\retro\\msxrc2014 &&

all: rom-image-pal rom-image-nms8250 rom-image-msxsyssrc

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
nextor: nextor/extras/extbio.com nextor/extras/xrecv.com nextor/extras/fdisk.com nextor/extras/lines.com nextor/extras/dots.com nextor/extras/mbrot.com nextor/extras/SROM.COM nextor/extras/SROM.TXT nextor/extras/SROM.INI nextor/extras/tv.com nextor/extras/tl.com nextor/extras/memman.com nextor/extras/cfgmman.com  nextor/extras/caps.tsr
	@mkdir -p ./bin
	echo "Requires sudo permission"
	sudo echo
	cd nextor/source
	BUILD_TYPE=cbios $(MAKE) nextor-2.1.1-alpha2.rc2014.rom -O -j --no-print-directory

install-prereq:
	@cd nextor/source
	$(MAKE) install-prereq --no-print-directory

.PHONY: rom-image-pal
rom-image-pal: nextor cbios ./extbio-rc2014/bin/extbio-rc2014.rom
	@rm -f bin/ymsx-pal.rom
	dd if=/dev/zero bs=16k count=8 of=bin/yellow-msx-pal-rc2104.rom
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_main_rc2014_pal.rom 	of=bin/yellow-msx-pal-rc2104.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/yellow-msx-pal-rc2104.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_sub.rom        			of=bin/yellow-msx-pal-rc2104.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom    of=bin/yellow-msx-pal-rc2104.rom bs=16k count=27 seek=4
	dd conv=notrunc status=none if=./extbio-rc2014/bin/extbio-rc2014.rom		 			of=bin/yellow-msx-pal-rc2104.rom bs=16k count=1 seek=31
	printf 'Built bin/yellow-msx-pal-rc2104.rom\r\n\n'

.PHONY: rom-image-nms8250
rom-image-nms8250: nextor cbios systemroms/nms8250_basic-bios2.rom systemroms/nms8250_msx2sub.rom ./extbio-rc2014/bin/extbio-rc2014.rom
	@rm -f bin/msxrc2014.rom
	dd if=/dev/zero bs=16k count=8 of=bin/nms8250-rc2014.rom
	dd conv=notrunc status=none if=./systemroms/nms8250_basic-bios2.rom					 	of=bin/nms8250-rc2014.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./systemroms/nms8250_msx2sub.rom				   			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom    of=bin/nms8250-rc2014.rom bs=16k count=27 seek=4
	dd conv=notrunc status=none if=./extbio-rc2014/bin/extbio-rc2014.rom		 			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=31
	printf 'Built bin/msxrc2014.rom\r\n\n'

.PHONY: rom-image-msxsyssrc
rom-image-msxsyssrc: nextor cbios msxsys ./extbio-rc2014/bin/extbio-rc2014.rom
	@rm -f bin/msxrc2014.rom
	dd if=/dev/zero bs=16k count=8 of=bin/msxsyssrc-rc2014.rom
	dd conv=notrunc status=none if=./bin/main.rom							 										of=bin/msxsyssrc-rc2014.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./bin/subrom.rom				   											of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.rc2014.rom    of=bin/msxsyssrc-rc2014.rom bs=16k count=27 seek=4
	dd conv=notrunc status=none if=./extbio-rc2014/bin/extbio-rc2014.rom		 			of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=31
	printf 'Built bin/msxrc2014.rom\r\n\n'

clean:
	@rm -rf ./bin
	$(MAKE) -C cbios --no-print-directory clean
	$(MAKE) -C ./extbio-rc2014 --no-print-directory clean
	cd nextor/source
	$(MAKE) --no-print-directory clean


## Tools

VPATH = ./tools/xmodem:./bin/:./apps
PASMO := pasmo --public -v -I ./bin/

bin/xrecv.com: xrecv.asm sio.asm bin/highpage.bin bdos.inc cbios.inc sio.inc ascii.inc cbios/src/hardware.asm cbios/src/hooks.asm

bin/chkspd.com: ./tools/chkspd/chkspd.asm
	mkdir -p bin
	$(PASMO) $< $@

nextor/extras/xrecv.com: bin/xrecv.com
	cp bin/xrecv.com nextor/extras/xrecv.com

bin/%.com: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@ $(@:.com=.sym)

bin/%.bin: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@ $(@:.bin=.sym)

nextor/extras/%.com: bin/%.com
	@cp "$<" "$@"

nextor/extras/SROM.%: 3rdparty/SROM.%
	@cp "$<" "$@"

nextor/extras/%.com: 3rdparty/%.com
	@cp "$<" "$@"

nextor/extras/%.tsr: 3rdparty/%.tsr
	@cp "$<" "$@"

.PHONY: extbio-rc2014/bin/extbio-rc2014.rom
extbio-rc2014/bin/extbio-rc2014.rom:
	@$(MAKE) -C ./extbio-rc2014 --no-print-directory bin/extbio-rc2014.rom

include Makefile-apps.mk

.PHONY: msxsys
msxsys:
	@cd msxsys-build
	$(MAKE) -O -j --no-print-directory
	cp -up ./working/*.rom ../bin/

.PHONY: bin/telnet.com
bin/telnet.com:
	@$(MAKE) --no-print-directory -C apps/telnet
	cp -up ./apps/telnet/bin/telnet.com ./bin/
