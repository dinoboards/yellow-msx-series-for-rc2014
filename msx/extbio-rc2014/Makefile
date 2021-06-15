SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

all: bin/extbio-rc2014.rom

bin/extbio-rc2014.rom: extbio-rc2014.asm sio.asm alloc.asm msx.inc utils.asm rs232.asm
	@mkdir -p bin
	./tools/zasm -v2 -i extbio-rc2014.asm -o ./bin/extbio-rc2014.rom
	cp $@ ../bin/

clean:
	@rm -rf bin

.PHONY: install-prereq
install-prereq:
	@rm -rf ./tools
	mkdir -p ./tools/
	cd ./tools/
	wget https://k1.spdns.de/Develop/Projects/zasm/Distributions/zasm-4.4.8-Linux64.zip -O "zasm-4.4.8-Linux64.zip"
	unzip zasm-4.4.8-Linux64.zip
	mv zasm-4.4.8-Linux64/* .
	rm zasm-4.4.8-Linux64.zip
	rm -r zasm-4.4.8-Linux64
