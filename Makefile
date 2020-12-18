
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: cbios nextor

.PHONY: memory.jed
memory.jed: bin/memory.jed

bin/memory.jed:
	@cmd.exe /C build-jed.bat

.PHONY: cbios
cbios:
	@mkdir -p ./bin
	ln -sf ../../cbios/derived ./bin/cbios
	$(MAKE) -C cbios --no-print-directory derived/bin/cbios_main_msx2+.rom derived/bin/cbios_logo_msx2+.rom derived/bin/cbios_sub.rom

.PHONY: nextor
nextor:
	@mkdir -p ./bin
	# ln -sf ../../cbios/derived ./bin/cbios
	cd nextor/source
	$(MAKE) -O -j --no-print-directory

install-prereq:
	cd nextor/source
	$(MAKE) install-prereq -O -j --no-print-directory

clean:
	@rm -rf ./bin
	$(MAKE) -C cbios --no-print-directory clean
	cd nextor/source
	$(MAKE) --no-print-directory clean
