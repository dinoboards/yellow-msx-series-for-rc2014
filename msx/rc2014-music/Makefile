SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

.DELETE_ON_ERROR:

ASM = sjasmplus --msg=war --nologo -Wno-rdlow --fullpath --raw=$@ --sym=$(basename $@).sym.asm --lst=$@.lst --exp=$(basename $@).exp.asm

BIN := ./bin/

all: $(BIN)rcmusic.rom

$(BIN)rcmusic.rom: rcmusic.asm
	@mkdir -p bin
	$(ASM) rcmusic.asm
	echo "Assembled $@"

clean:
	@rm -rf bin
