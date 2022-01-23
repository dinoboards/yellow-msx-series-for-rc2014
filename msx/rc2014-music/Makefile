SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

.DELETE_ON_ERROR:

ASM = sjasmplus --msg=war --nologo -Wno-rdlow --fullpath --raw=$@ --sym=$(basename $@).sym.asm --lst=$@.lst --exp=$(basename $@).exp.asm

BIN := ./bin/

all: $(BIN)rcmusic.rom

$(BIN)rcmusic.rom: $(BIN)rcmusic.bin $(BIN)music.dsk
	@cat $(BIN)rcmusic.bin $(BIN)music.dsk > $(BIN)rcmusic.rom
	echo "Built $(BIN)rcmusic.bin"

$(BIN)rcmusic.bin: rcmusic.asm
	@mkdir -p $(BIN)
	$(ASM) rcmusic.asm
	echo "Assembled $@"

DISK_SIZE := 507904		# (512-16)*1024

DISK_FILES := $(wildcard ../3rdparty/vgmplay/*)
$(BIN)music.dsk: $(DISK_FILES)
	@mkdir -p $(BIN)
	rm -f $(BIN)music.dsk
	dd status=none if=/dev/zero of=$(BIN)music.dsk bs=$(DISK_SIZE) count=1
	mkfs.vfat -F 12 -f 1 $(BIN)music.dsk > /dev/null
	mmd -i $(BIN)music.dsk vgmplay
	mcopy -i $(BIN)music.dsk ../3rdparty/vgmplay/* ::/vgmplay/
	echo "Built disk image $(BIN)music.dsk"

clean:
	@rm -rf bin
