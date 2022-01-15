SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

.DELETE_ON_ERROR:

ASM = sjasmplus --msg=war --nologo -Wno-rdlow --fullpath --raw=$@ --sym=$(basename $@).sym.asm --lst=$@.lst --exp=$(basename $@).exp.asm

BIN := ./bin/

all: $(BIN)rc2014-extended.rom

$(BIN)rc2014-extended.rom: ../rc2014config.inc sio.inc rc2014-extended.asm extended-bios.asm msx.inc utils.asm rs232.asm sio.asm alloc.asm rominit.asm probing.asm fossil.asm segment1.asm segment1.inc
	@mkdir -p bin
	$(ASM) rc2014-extended.asm
	echo "Assembled $@"

clean:
	@rm -rf bin
