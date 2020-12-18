
SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: memory.jed cbios

.PHONY: memory.jed
memory.jed: bin/memory.jed

bin/memory.jed:
	@cmd.exe /C build-jed.bat

.PHONY: cbios
cbios:
	@mkdir -p ./bin
	ln -sf ../../cbios/derived ./bin/cbios
	$(MAKE) -C cbios derived/bin/cbios_main_msx2+.rom derived/bin/cbios_logo_msx2+.rom derived/bin/cbios_sub.rom

clean:
	@rm -rf ./bin
	$(MAKE) -C cbios clean
