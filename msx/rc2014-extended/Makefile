SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ASM = sjasmplus -Wno-rdlow --fullpath --raw=$@ --sym=$@.sym --lst=$@.lst --exp=$@.exp

BIN := ./bin/

all: $(BIN)rc2014-extended.rom

$(BIN)rc2014-extended.rom: ../rc2014config.inc sio.inc rc2014-extended.asm extended-bios.asm msx.inc utils.asm rs232.asm sio.asm alloc.asm rominit.asm probing.asm fossil.asm
	@mkdir -p bin
	$(ASM) rc2014-extended.asm

clean:
	@rm -rf bin
