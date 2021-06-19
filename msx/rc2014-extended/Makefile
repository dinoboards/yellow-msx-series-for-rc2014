SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

BIN := ./bin/

all: $(BIN)rc2014-extended.rom

$(BIN)rc2014-extended.rom: rc2014-extended.asm extended-bios.asm msx.inc utils.asm rs232.asm sio.asm alloc.asm rominit.asm $(BIN)fossil_000.bin $(BIN)fossil-map.bin
	@mkdir -p bin
	./tools/zasm -v2 --labels -i rc2014-extended.asm -o $(BIN)rc2014-extended.rom


$(BIN)fossil_000.bin: fossil.asm
	@mkdir -p bin
	echo "  ORG 0x000" > ./fossil_000.asm.tmp
	cat fossil.asm >> ./fossil_000.asm.tmp
	./tools/zasm  -v2 ./fossil_000.asm.tmp -o $(BIN)fossil_000.bin -l $(BIN)fossil_000.lst
	rm ./fossil_000.asm.tmp

$(BIN)fossil_100.bin: fossil.asm
	@mkdir -p bin
	echo "  ORG 0x200" > ./fossil_100.asm.tmp
	cat fossil.asm >> ./fossil_100.asm.tmp
	./tools/zasm  -v2 ./fossil_100.asm.tmp -o $(BIN)fossil_100.bin -l $(BIN)fossil_100.lst
	rm ./fossil_100.asm.tmp

$(BIN)fossil-map.bin: $(addprefix $(BIN),fossil_000.bin fossil_100.bin)
	@node ./extract-relocation-map.js $^ $(BIN)fossil-map.bin

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
