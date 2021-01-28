
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
CDWINDOWS := u: && cd \\home\\dean\\retro\\msxrc2014 &&

all: rom-image-pal bin/lines.com bin/dots.com bin/mbrot.com

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
	$(MAKE) -C cbios --no-print-directory derived/bin/cbios_main_rc2014_pal.rom  derived/bin/cbios_main_rc2014_ntsc.rom derived/bin/cbios_logo_msx2+.rom derived/bin/cbios_sub.rom

.PHONY: nextor
nextor: nextor/extras/xrecv.com nextor/extras/lines.com nextor/extras/dots.com nextor/extras/mbrot.com nextor/extras/SROM.COM nextor/extras/SROM.TXT nextor/extras/SROM.INI
	@mkdir -p ./bin
	echo "Requires sudo permission"
	sudo echo
	cd nextor/source
	BUILD_TYPE=cbios $(MAKE) nextor-2.1.1-alpha2.embedded.rom -O -j --no-print-directory

install-prereq:
	@cd nextor/source
	$(MAKE) install-prereq --no-print-directory

.PHONY: rom-image-pal
rom-image-pal: nextor cbios
	@rm -f bin/ymsx-pal.rom
	dd if=/dev/zero bs=16k count=8 of=bin/ymsx-pal.rom
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_main_rc2014_pal.rom 	of=bin/ymsx-pal.rom bs=16k count=2 seek=0
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_logo_msx2+.rom 			of=bin/ymsx-pal.rom bs=16k count=1 seek=2
	dd conv=notrunc status=none if=./cbios/derived/bin/cbios_sub.rom        			of=bin/ymsx-pal.rom bs=16k count=1 seek=3
	dd conv=notrunc status=none if=./nextor/bin/nextor-2.1.1-alpha2.embedded.rom  of=bin/ymsx-pal.rom bs=16k count=24 seek=4

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

Z88DK := zcc +msx -create-app -subtype=msxdos2 -compiler=sdcc -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1 --c-code-in-asm -SO3 --max-allocs-per-node200000

bin/lines.com: lines.c v9958.c msx.asm v9958.asm
	@mkdir -p bin
	cd apps
	$(Z88DK) lines.c v9958.c v9958.asm msx.asm -o lines.com
	mv lines.com ../bin/
	rm lines.img

nextor/extras/lines.com: bin/lines.com
	cp bin/lines.com nextor/extras/lines.com

bin/dots.com: dots.c v9958.c msx.asm v9958.asm
	@mkdir -p bin
	cd apps
	$(Z88DK) dots.c v9958.c v9958.asm msx.asm -o dots.com
	mv dots.com ../bin/
	rm dots.img

nextor/extras/dots.com: bin/dots.com
	cp bin/dots.com nextor/extras/dots.com

bin/mbrot.com: mbrot.c v9958.c msx.asm v9958.asm
	@mkdir -p bin
	cd apps
	$(Z88DK) mbrot.c v9958.c v9958.asm msx.asm -o mbrot.com
	mv mbrot.com ../bin/
	rm mbrot.img


bin/spike-fdd.com: spike-fdd.c fdd.asm
	@mkdir -p bin
	cd apps
	$(Z88DK) spike-fdd.c fdd.asm -o spike-fdd.com
	mv spike-fdd.com ../bin/
	rm spike-fdd.img

nextor/extras/mbrot.com: bin/mbrot.com
	cp bin/mbrot.com nextor/extras/mbrot.com

nextor/extras/SROM.%: 3rdparty/SROM.%
	@cp "$<" "$@"

bin/fdu.com: fdu.asm
