PASMO := pasmo --public -v -I ./bin/

SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: bin/extbio-rc2014.rom

bin/%.rom: %.asm
	@mkdir -p bin
	$(PASMO)  $< $@
	cp $@ ../bin/

bin/extbio-rc2014.rom: extbio-rc2014.asm sio.asm

clean:
	@rm -rf bin
