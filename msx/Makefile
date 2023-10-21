SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

MAKEFLAGS += -s

all: apps pmsxsysromimages rom-image-pal rom-image-ntsc bin/rcmusic.rom bin/bbcbasic.com

pmsxsysromimages:
	@$(MAKE) -j -O msxsysromimages --no-print-directory

jeds: bin/mem-selector.jed bin/rom-mapper.jed bin/turbo-cpu.jed bin/turbo-cpu-mwait.jed bin/slot-backplane-driver.jed bin/v9958-cs.jed

bin/%.jed:
	./build-jed.sh $<

bin/mem-selector.jed: mem-selector.pld
bin/rom-mapper.jed: rom-mapper.pld
bin/turbo-cpu.jed: turbo-cpu.pld
bin/turbo-cpu-mwait.jed: turbo-cpu-mwait.pld
bin/slot-backplane-driver.jed: slot-backplane-driver.pld
bin/v9958-cs.jed: v9958-cs.pld

.PHONY: cbios
cbios:
	@mkdir -p ./bin
	$(MAKE) -C cbios --no-print-directory derived/bin/cbios_main_rc2014_pal.rom  derived/bin/cbios_main_rc2014_ntsc.rom derived/bin/cbios_logo_rc2014.rom derived/bin/cbios_sub.rom
	cp -u ./cbios/derived/bin/*.rom ./bin


# esp8266.com
INCLUDED_APPS 	:= testram.com testram.txt memtest.com memtest.eng memtest.fr memtest.jpn pt3play.com iteratn.pt3 mbrot.com usbctrl.com
INCLUDED_SYSTEM := xrecv.com fdisk.com rtccalb.com siocfg.com fossilon.com
INCLUDED_ROOT 	:= AUTOEXEC.BAT SROM.COM SROM.TXT SROM.INI

define BUILD_versions =

.PHONY: bin/$1.nextor.rom
bin/$1.nextor.rom: $(addprefix nextor-rc2014/extras/apps/,$(INCLUDED_APPS)) $(addprefix nextor-rc2014/extras/system/,$(INCLUDED_SYSTEM))  $(addprefix nextor-rc2014/extras/,$(INCLUDED_ROOT))
	@mkdir -p ./bin
	cd nextor-rc2014
	$(MAKE) ./bin/$1.nextor.rom -O -j --no-print-directory
	cp -u ./bin/$1.nextor.rom ../bin
endef

$(eval $(call BUILD_versions,cbios))
$(eval $(call BUILD_versions,rc2014))

.PHONY: nextor-mfr
nextor-mfr:
	mkdir -p ./bin
	cd nextor/source
	BUILD_TYPE=std $(MAKE) ../bin/mfr.nextor-2.1.1.rom -O -j --no-print-directory -f Makefile-rc2014.mk
	cp -u ../bin/mfr.nextor-2.1.1.rom ../../bin/
	BUILD_TYPE=std $(MAKE) ../bin/mfr.dsk --no-print-directory -f Makefile-rc2014.mk
	cp -u ../bin/mfr.dsk ../../bin

install-prereq:
	@(cd nextor/source && $(MAKE) install-prereq --no-print-directory) -f Makefile-rc2014.mk
	(cd rc2014-extended && $(MAKE) install-prereq --no-print-directory)

.PHONY: rom-image-pal
rom-image-pal: bin/cbios.nextor.rom cbios ./rc2014-extended/bin/rc2014-extended.rom
	@rm -f bin/yellow-msx-pal-rc2104.rom
	dd status=none if=/dev/zero bs=16k count=8 of=bin/yellow-msx-pal-rc2104.rom
	dd status=none conv=notrunc if=./cbios/derived/bin/cbios_main_rc2014_pal.rom 	of=bin/yellow-msx-pal-rc2104.rom bs=16k count=2  seek=0
	dd status=none conv=notrunc if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/yellow-msx-pal-rc2104.rom bs=16k count=1  seek=2
	dd status=none conv=notrunc if=./cbios/derived/bin/cbios_sub.rom        			of=bin/yellow-msx-pal-rc2104.rom bs=16k count=1  seek=3
	dd status=none conv=notrunc if=./bin/cbios.nextor.rom             	    			of=bin/yellow-msx-pal-rc2104.rom bs=16k count=27 seek=4
	dd status=none conv=notrunc if=./rc2014-extended/bin/rc2014-extended.rom		 	of=bin/yellow-msx-pal-rc2104.rom bs=16k count=1  seek=31
	# printf 'Built bin/yellow-msx-pal-rc2104.rom\r\n'

.PHONY: rom-image-ntsc
rom-image-ntsc: bin/cbios.nextor.rom cbios ./rc2014-extended/bin/rc2014-extended.rom
	@rm -f bin/yellow-msx-ntsc-rc2104.rom
	dd status=none if=/dev/zero bs=16k count=8 of=bin/yellow-msx-ntsc-rc2104.rom
	dd status=none conv=notrunc if=./cbios/derived/bin/cbios_main_rc2014_ntsc.rom of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=2  seek=0
	dd status=none conv=notrunc if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=1  seek=2
	dd status=none conv=notrunc if=./cbios/derived/bin/cbios_sub.rom        			of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=1  seek=3
	dd status=none conv=notrunc if=./bin/cbios.nextor.rom    			     						of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=27 seek=4
	dd status=none conv=notrunc if=./rc2014-extended/bin/rc2014-extended.rom		 	of=bin/yellow-msx-ntsc-rc2104.rom bs=16k count=1  seek=31
	# printf 'Built bin/yellow-msx-ntsc-rc2104.rom\r\n'

.PHONY: rom-image-nms8250
rom-image-nms8250: nextor-rc2014 cbios systemroms/nms8250_basic-bios2.rom systemroms/nms8250_msx2sub.rom ./rc2014-extended/bin/rc2014-extended.rom
	@rm -f bin/nms8250-rc2014.rom
	dd status=none if=/dev/zero bs=16k count=8 of=bin/nms8250-rc2014.rom
	dd status=none conv=notrunc if=./systemroms/nms8250_basic-bios2.rom					 	of=bin/nms8250-rc2014.rom bs=16k count=2 seek=0
	dd status=none conv=notrunc if=./cbios/derived/bin/cbios_logo_rc2014.rom 			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=2
	dd status=none conv=notrunc if=./systemroms/nms8250_msx2sub.rom				   			of=bin/nms8250-rc2014.rom bs=16k count=1 seek=3
	dd status=none conv=notrunc if=./nextor/bin/rc2014.nextor-2.1.1.rom    				of=bin/nms8250-rc2014.rom bs=16k count=27 seek=4
	dd status=none conv=notrunc if=./rc2014-extended/bin/rc2014-extended.rom      of=bin/nms8250-rc2014.rom bs=16k count=1 seek=31
	# printf 'Built bin/nms8250-rc2014.rom\r\n'

.PHONY: clean
clean:
	@rm -rf ./bin
	$(MAKE) -C apps --no-print-directory clean
	(cd msxsys-build && $(MAKE) --no-print-directory clean)
	$(MAKE) -C ./cbios --no-print-directory clean
	$(MAKE) -C ./rc2014-extended --no-print-directory clean
	$(MAKE) -C ./rc2014-music --no-print-directory clean
	$(MAKE) -C ./nextor-rc2014 --no-print-directory clean
	$(MAKE) -C ./bbcbasic-z80 --no-print-directory clean
	cd nextor/source && $(MAKE) --no-print-directory clean -f Makefile-rc2014.mk
	rm -rf nextor-rc2014/extras/*

## Tools

VPATH = ./tools/xmodem:./bin/
PASMO := pasmo --public -v -I ./bin/

tools: apps/bin/xrecv.com bin/chkspd.com

bin/chkspd.com: ./tools/chkspd/chkspd.asm
	@mkdir -p bin
	$(PASMO) $< $@

.PHONY: apps/bin/xrecv.com
apps/bin/xrecv.com:
	@make -C apps bin/xrecv.com --no-print-directory

nextor-rc2014/extras/system/xrecv.com: apps/bin/xrecv.com
	@mkdir -p nextor-rc2014/extras/system
	cp -up apps/bin/xrecv.com nextor-rc2014/extras/system/xrecv.com

bin/%.com: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@
	#$(@:.com=.sym)

bin/%.bin: %.asm
	@mkdir -p bin
	@$(PASMO) -I ./tools/xmodem/ $< $@ $(@:.bin=.sym)

nextor-rc2014/extras/apps/%: bin/%
	@mkdir -p nextor-rc2014/extras/apps
	@cp -up "$<" "$@"

nextor-rc2014/extras/system/%.com: bin/%.com
	@mkdir -p nextor-rc2014/extras/system
	@cp -up "$<" "$@"

nextor-rc2014/extras/SROM.%: 3rdparty/SROM.%
	@mkdir -p nextor-rc2014/extras
	@cp -up "$<" "$@"

nextor-rc2014/extras/apps/%: 3rdparty/%
	@mkdir -p nextor-rc2014/extras/apps
	@cp -up "$<" "$@"

nextor-rc2014/extras/apps/%.tsr: 3rdparty/%.tsr
	@mkdir -p nextor-rc2014/extras/apps
	@cp -up "$<" "$@"

nextor-rc2014/extras/roms/%.rom: systemroms/%.rom
	@mkdir -p nextor-rc2014/extras/roms
	@cp -up "$<" "$@"

nextor-rc2014/extras/AUTOEXEC.BAT: ./AUTOEXEC.BAT
	@mkdir -p nextor-rc2014/extras
	@cp -up ./AUTOEXEC.BAT nextor-rc2014/extras/AUTOEXEC.BAT

.PHONY: rc2014-extended/bin/rc2014-extended.rom
rc2014-extended/bin/rc2014-extended.rom:
	@$(MAKE) -C ./rc2014-extended --no-print-directory bin/rc2014-extended.rom
	cp rc2014-extended/bin/rc2014-extended.rom ./bin/

include Makefile-apps.mk

define BUILD_msxsysromimage =
.PHONY: rom-msxsyssrc-$1-$2-rtc
rom-msxsyssrc-$1-$2-rtc: bin/rc2014.nextor.rom cbios msxsys-$1-$2-rtc ./rc2014-extended/bin/rc2014-extended.rom
	@rm -f bin/msxsyssrc-rc2014-$1-$2-rtc.rom
	dd status=none if=/dev/zero bs=16k count=8 of=bin/msxsyssrc-rc2014-$1-$2-rtc.rom
	dd status=none conv=notrunc status=none if=./bin/$1-$2-rtc-main.rom													of=bin/msxsyssrc-rc2014-$1-$2-rtc.rom bs=16k count=2  seek=0
	dd status=none conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_rc2014.rom 				of=bin/msxsyssrc-rc2014-$1-$2-rtc.rom bs=16k count=1  seek=2
	dd status=none conv=notrunc status=none if=./bin/$1-$2-rtc-subrom.rom												of=bin/msxsyssrc-rc2014-$1-$2-rtc.rom bs=16k count=1  seek=3
	dd status=none conv=notrunc status=none if=./bin/rc2014.nextor.rom    											of=bin/msxsyssrc-rc2014-$1-$2-rtc.rom bs=16k count=27 seek=4
	dd status=none conv=notrunc status=none if=./rc2014-extended/bin/rc2014-extended.rom				of=bin/msxsyssrc-rc2014-$1-$2-rtc.rom bs=16k count=1  seek=31
	# printf 'Built bin/msxsyssrc-rc2014-$1-$2-rtc.rom\r\n'

.PHONY: msxsys-$1-$2-rtc
msxsys-$1-$2-rtc:
	@cd msxsys-build
	RTC=$2 COUNTRY=$1 $(MAKE) --no-print-directory ./working/$1-$2-rtc/main.rom ./working/$1-$2-rtc/optrom.rom ./working/$1-$2-rtc/subrom.rom -O -j
	cp -up ./working/$1-$2-rtc/main.rom    ../bin/$1-$2-rtc-main.rom
	cp -up ./working/$1-$2-rtc/optrom.rom  ../bin/$1-$2-rtc-optrom.rom
	cp -up ./working/$1-$2-rtc/subrom.rom  ../bin/$1-$2-rtc-subrom.rom

msxsysromimages: rom-msxsyssrc-$1-$2-rtc
endef

define BUILD_msxsysromimage-country =
$(eval $(call BUILD_msxsysromimage,$1,with))
# $(eval $(call BUILD_msxsysromimage,$1,without))
endef

$(eval $(call BUILD_msxsysromimage-country,aust))
$(eval $(call BUILD_msxsysromimage-country,japan))
$(eval $(call BUILD_msxsysromimage-country,usa))
$(eval $(call BUILD_msxsysromimage-country,international))
$(eval $(call BUILD_msxsysromimage-country,uk))
$(eval $(call BUILD_msxsysromimage-country,france))
$(eval $(call BUILD_msxsysromimage-country,germany))
$(eval $(call BUILD_msxsysromimage-country,italy))
$(eval $(call BUILD_msxsysromimage-country,spain))
$(eval $(call BUILD_msxsysromimage-country,arabic))
$(eval $(call BUILD_msxsysromimage-country,korea))
$(eval $(call BUILD_msxsysromimage-country,russia))

.PHONY: bin/rcmusic.rom
bin/rcmusic.rom:
	$(MAKE) --no-print-directory -C rc2014-music
	cp rc2014-music/bin/rcmusic.rom ./bin/rcmusic.rom

release:
	@version=$$(date +%y-%m-%d)
	mkdir -p ../release
	rm -f ../release/msx-rc2014-$${version}.zip
	zip -j ../release/msx-rc2014-$${version}.zip ./bin/*.jed
	zip -j ../release/msx-rc2014-$${version}.zip ./bin/*.doc
	zip -j ../release/msx-rc2014-$${version}.zip ./bin/*.com
	zip -j ../release/msx-rc2014-$${version}.zip ./bin/msxsyssrc-*.rom
	zip -j ../release/msx-rc2014-$${version}.zip ./bin/yellow-*.rom
	zip -j ../release/msx-rc2014-$${version}.zip ./bin/rcmusic.rom
	(cd ./bin && tar -cvf ../../release/msx-rc2014-$${version}.tar *.jed)
	(cd ./bin && tar -uvf ../../release/msx-rc2014-$${version}.tar *.doc)
	(cd ./bin && tar -uvf ../../release/msx-rc2014-$${version}.tar *.com)
	(cd ./bin && tar -uvf ../../release/msx-rc2014-$${version}.tar msxsyssrc-*.rom)
	(cd ./bin && tar -uvf ../../release/msx-rc2014-$${version}.tar yellow-*.rom)
	(cd ./bin && tar -uvf ../../release/msx-rc2014-$${version}.tar rcmusic.rom)
	(cd ./bin && gzip -qf ../../release/msx-rc2014-$${version}.tar)

bin/bbcbasic.com: $(wildcard bbcbasic-z80/*)
	@$(MAKE) -C bbcbasic-z80 --no-print-directory
	cp -u bbcbasic-z80/bin/bbcbasic.com $@
