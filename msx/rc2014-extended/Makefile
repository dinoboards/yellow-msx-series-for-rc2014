SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

BIN := ./bin/

all: bin/rc2014-extended.rom

bin/rc2014-extended.rom: rc2014-extended.asm extended-bios.asm msx.inc utils.asm rs232.asm sio.asm alloc.asm rominit.asm $(BIN)fossil_100.bin $(BIN)fossil-map.bin
	@mkdir -p bin
	./tools/zasm -v2 --labels -i rc2014-extended.asm -o ./bin/rc2014-extended.rom


bin/fossil_100.bin: fossil.asm
	@mkdir -p bin
	echo "  ORG 0x100" > ./fossil_100.asm.tmp
	cat fossil.asm >> ./fossil_100.asm.tmp
	./tools/zasm  -v2 ./fossil_100.asm.tmp -o ./bin/fossil_100.bin -l ./bin/fossil_100.lst
	rm ./fossil_100.asm.tmp

bin/fossil_200.bin: fossil.asm
	@mkdir -p bin
	echo "  ORG 0x100" > ./fossil_200.asm.tmp
	cat fossil.asm >> ./fossil_200.asm.tmp
	./tools/zasm  -v2 ./fossil_200.asm.tmp -o ./bin/fossil_200.bin -l ./bin/fossil_200.lst
	rm ./fossil_200.asm.tmp

$(BIN)fossil-map.bin: $(addprefix $(BIN),fossil_100.bin fossil_200.bin)
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
