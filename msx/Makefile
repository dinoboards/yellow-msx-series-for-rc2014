SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

# Example of MSX_JED_CDWIN environment variable
# MSX_JED_CDWIN="u: && cd \\home\\jane\\<path to project>\\msx &&"

all: rom-image-pal rom-image-ntsc rom-image-msxsyssrc #rom-image-nms8250

jeds: bin/mem-selector.jed bin/rom-mapper.jed

bin/mem-selector.jed: mem-selector.pld
	@cd $$(wslpath "$$(wslvar USERPROFILE)")
	if [[ -z $${MSX_JED_CDWIN+x} ]]; then
		echo "JEDS files are compiled under windows using WINCUPL using build-jed.bat."
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null ; then
			echo "To assembly thru this makefile on WSL - you need to assign MSX_JED_CDWIN env var."
		fi
		exit 1
	fi
	cmd.exe /C "$(MSX_JED_CDWIN) build-jed.bat mem-selector"

bin/rom-mapper.jed: rom-mapper.pld
	@cd $$(wslpath "$$(wslvar USERPROFILE)")
	if [[ -z $${MSX_JED_CDWIN+x} ]]; then
		echo "JEDS files are compiled under windows using WINCUPL using build-jed.bat."
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null ; then
			echo "To assembly thru this makefile on WSL - you need to assign MSX_JED_CDWIN env var."
		fi
		exit 1
	fi
	cmd.exe /C "$(MSX_JED_CDWIN) build-jed.bat rom-mapper"

bin/turbo-cpu.jed: turbo-cpu.pld
	@cd $$(wslpath "$$(wslvar USERPROFILE)")
	if [[ -z $${MSX_JED_CDWIN+x} ]]; then
		echo "JEDS files are compiled under windows using WINCUPL using build-jed.bat."
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null ; then
			echo "To assembly thru this makefile on WSL - you need to assign MSX_JED_CDWIN env var."
		fi
		exit 1
	fi
	cmd.exe /C "$(MSX_JED_CDWIN) build-jed.bat turbo-cpu"

bin/slot-backplane-driver.jed: slot-backplane-driver.pld
	@cd $$(wslpath "$$(wslvar USERPROFILE)")
	if [[ -z $${MSX_JED_CDWIN+x} ]]; then
		echo "JEDS files are compiled under windows using WINCUPL using build-jed.bat."
		if grep -qEi "(Microsoft|WSL)" /proc/version &> /dev/null ; then
			echo "To assembly thru this makefile on WSL - you need to assign MSX_JED_CDWIN env var."
		fi
		exit 1
	fi
	cmd.exe /C "$(MSX_JED_CDWIN) build-jed.bat slot-backplane-driver"


.PHONY: cbios
cbios:
	@mkdir -p ./bin
	$(MAKE) -C cbios --no-print-directory derived/bin/cbios_main_rc2014_pal.rom  derived/bin/cbios_main_rc2014_ntsc.rom derived/bin/cbios_logo_rc2014.rom derived/bin/cbios_sub.rom
	cp -u ./cbios/derived/bin/*.rom ./bin


INCLUDED_APPS 	:= testram.com testram.txt memtest.com  memtest.eng memtest.fr memtest.jpn pt3play.com iteratn.pt3 lines.com dots.com mbrot.com
INCLUDED_SYSTEM := xrecv.com fdisk.com
INCLUDED_ROOT 	:=  AUTOEXEC.BAT  SROM.COM SROM.TXT SROM.INI

.PHONY: nextor
nextor: $(addprefix nextor/extras/apps/,$(INCLUDED_APPS)) $(addprefix nextor/extras/system/,$(INCLUDED_SYSTEM))  $(addprefix nextor/extras/,$(INCLUDED_ROOT)) #nextor/extras/roms/pacman.rom
	@mkdir -p ./bin
	echo "Requires sudo permission"
	sudo echo
	cd nextor/source
	BUILD_TYPE=cbios $(MAKE) nextor-2.1.1.rc2014.rom -O -j --no-print-directory
	cp -u ../bin/nextor-2.1.1.rc2014.rom ../../bin

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
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1.rc2014.rom    				of=bin/yellow-msx-pal-rc2104.rom bs=16k count=27 seek=4
	dd conv=notrunc status=none if=./extbio-rc2014/bin/extbio-rc2014.rom		 			of=bin/yellow-msx-pal-rc2104.rom bs=16k count=1 seek=31
	printf 'Built bin/yellow-msx-pal-rc2104.rom\r\n\n'

.PHONY: rom-image-ntsc
rom-image-ntsc: nextor cbios ./extbio-rc2014/bin/extbio-rc2014.rom
	@rm -f bin/ymsx-ntsc.rom
	dd if=/dev/zero bs=16k count=8 of=bin/yellow-msx-ntsc-rc2104.rom
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_main_rc2014_ntsc.rom 	of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_sub.rom        			of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1.rc2014.rom    				of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=27 seek=4
	dd conv=notrunc status=none if=./extbio-rc2014/bin/extbio-rc2014.rom		 			of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=1 seek=31
	printf 'Built bin/yellow-msx-ntsc-rc2104.rom\r\n\n'

.PHONY: rom-image-nms8250
rom-image-nms8250: nextor cbios systemroms/nms8250_basic-bios2.rom systemroms/nms8250_msx2sub.rom ./extbio-rc2014/bin/extbio-rc2014.rom
	@rm -f bin/msxrc2014.rom
	dd if=/dev/zero bs=16k count=8 of=bin/nms8250-rc2014.rom
	dd conv=notrunc status=none if=./systemroms/nms8250_basic-bios2.rom					 	of=bin/nms8250-rc2014.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./systemroms/nms8250_msx2sub.rom				   			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1.rc2014.rom    				of=bin/nms8250-rc2014.rom bs=16k count=27 seek=4
	dd conv=notrunc status=none if=./extbio-rc2014/bin/extbio-rc2014.rom		 			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=31
	printf 'Built bin/msxrc2014.rom\r\n\n'

.PHONY: rom-image-msxsyssrc
rom-image-msxsyssrc: nextor cbios msxsys ./extbio-rc2014/bin/extbio-rc2014.rom
	@rm -f bin/msxrc2014.rom
	dd if=/dev/zero bs=16k count=8 of=bin/msxsyssrc-rc2014.rom
	dd conv=notrunc status=none if=./bin/main.rom							 										of=bin/msxsyssrc-rc2014.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./bin/subrom.rom				   											of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1.rc2014.rom    				of=bin/msxsyssrc-rc2014.rom bs=16k count=27 seek=4
	dd conv=notrunc status=none if=./extbio-rc2014/bin/extbio-rc2014.rom		 			of=bin/msxsyssrc-rc2014.rom bs=16k count=1 seek=31
	printf 'Built bin/msxrc2014.rom\r\n\n'

clean:
	@rm -rf ./bin
	$(MAKE) -C apps --no-print-directory clean
	$(MAKE) -C cbios --no-print-directory clean
	$(MAKE) -C ./extbio-rc2014 --no-print-directory clean
	cd nextor/source
	$(MAKE) --no-print-directory clean
	rm ../extras/*

## Tools

VPATH = ./tools/xmodem:./bin/
PASMO := pasmo --public -v -I ./bin/

tools: bin/xrecv.com bin/chkspd.com
bin/xrecv.com: xrecv.asm sio.asm bin/highpage.bin bdos.inc cbios.inc sio.inc ascii.inc cbios/src/hardware.asm cbios/src/hooks.asm

bin/chkspd.com: ./tools/chkspd/chkspd.asm
	@mkdir -p bin
	$(PASMO) $< $@

nextor/extras/system/xrecv.com: bin/xrecv.com
	@mkdir -p nextor/extras/system
	@cp -up bin/xrecv.com nextor/extras/system/xrecv.com

bin/%.com: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@
	#$(@:.com=.sym)

bin/%.bin: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@ $(@:.bin=.sym)

nextor/extras/apps/%: bin/%
	@mkdir -p nextor/extras/apps
	@cp -up "$<" "$@"

nextor/extras/system/%.com: bin/%.com
	@mkdir -p nextor/extras/system
	@cp -up "$<" "$@"

nextor/extras/SROM.%: 3rdparty/SROM.%
	@mkdir -p nextor/extras
	@cp -up "$<" "$@"

nextor/extras/apps/%: 3rdparty/%
	@mkdir -p nextor/extras/apps
	@cp -up "$<" "$@"

nextor/extras/apps/%.tsr: 3rdparty/%.tsr
	@mkdir -p nextor/extras/apps
	@cp -up "$<" "$@"

nextor/extras/roms/%.rom: systemroms/%.rom
	@mkdir -p nextor/extras/roms
	@cp -up "$<" "$@"

nextor/extras/AUTOEXEC.BAT: ./AUTOEXEC.BAT
	@mkdir -p nextor/extras
	@cp -up ./AUTOEXEC.BAT nextor/extras/AUTOEXEC.BAT

.PHONY: extbio-rc2014/bin/extbio-rc2014.rom
extbio-rc2014/bin/extbio-rc2014.rom:
	@$(MAKE) -C ./extbio-rc2014 --no-print-directory bin/extbio-rc2014.rom

include Makefile-apps.mk

.PHONY: msxsys
msxsys:
	@cd msxsys-build
	$(MAKE) -O -j --no-print-directory
	cp -up ./working/*.rom ../bin/

release:
	@version=$$(date +%y-%m-%d)
	mkdir -p ../release
	rm -f ../release/msx-rc2014-$${version}.zip
	zip -j ../release/msx-rc2014-$${version}.zip ./bin/*
	cd ./bin && tar -czvf ../../release/msx-rc2014-$${version}.tar.gz *
