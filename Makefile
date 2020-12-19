
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: rom-image

.PHONY: memory.jed
memory.jed: bin/memory.jed

bin/memory.jed:
	@cmd.exe /C build-jed.bat

.PHONY: cbios
cbios:
	@mkdir -p ./bin
	$(MAKE) -C cbios --no-print-directory derived/bin/cbios_main_rc2014.rom derived/bin/cbios_logo_msx2+.rom derived/bin/cbios_sub.rom

.PHONY: nextor
nextor:
	@mkdir -p ./bin
	sudo echo "Requires sudo permission"
	cd nextor/source
	BUILD_TYPE=cbios $(MAKE) nextor-2.1.1-alpha2.embedded.rom -O -j --no-print-directory

install-prereq:
	@cd nextor/source
	$(MAKE) install-prereq --no-print-directory

.PHONY: rom-image
rom-image: nextor cbios
	@rm -f bin/ymsx.rom
	dd if=/dev/zero bs=16k count=8 of=bin/ymsx.rom
	dd conv=notrunc if=./cbios/derived/bin/cbios_main_rc2014.rom 			of=bin/ymsx.rom bs=16k count=2 seek=0
	dd conv=notrunc if=./cbios/derived/bin/cbios_logo_msx2+.rom 			of=bin/ymsx.rom bs=16k count=1 seek=2
	dd conv=notrunc if=./cbios/derived/bin/cbios_sub.rom        			of=bin/ymsx.rom bs=16k count=1 seek=4
	dd conv=notrunc if=./nextor/bin/nextor-2.1.1-alpha2.embedded.rom  of=bin/ymsx.rom bs=16k count=16 seek=8

clean:
	@rm -rf ./bin
	$(MAKE) -C cbios --no-print-directory clean
	cd nextor/source
	$(MAKE) --no-print-directory clean
