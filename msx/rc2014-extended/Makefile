SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

PASMO := pasmo --public -v -I ./bin/

BIN := ./bin/

.NOTPARALLEL:
all: $(BIN)rc2014-extended.rom

$(BIN)rc2014-extended.rom: ../rc2014config.inc sio.inc rc2014-extended.asm extended-bios.asm msx.inc utils.asm rs232.asm sio.asm alloc.asm rominit.asm probing.asm $(BIN)fossil_000.bin $(BIN)fossil-map.bin $(BIN)fossil.sym
	@mkdir -p bin
	$(PASMO) --equ SYMBOL_ONLY=0 rc2014-extended.asm $@

$(BIN)rc2014-extended.rom.sym: ../rc2014config.inc sio.inc rc2014-extended.asm extended-bios.asm msx.inc utils.asm rs232.asm sio.asm alloc.asm rominit.asm probing.asm $(BIN)fossil_xxx.bin $(BIN)fossil-map-xxx.bin $(BIN)fossil.sym
	@mkdir -p bin
	$(PASMO) --equ SYMBOL_ONLY=1 rc2014-extended.asm $(BIN)ignored $@

$(BIN)fossil_xxx.bin $(BIN)fossil.sym: fossil.asm
	@mkdir -p bin
	echo "  ORG 0x000" > $(BIN)fossil_xxx.asm
	echo "  include \"fossil-import.asm\"" >> $(BIN)fossil_xxx.asm
	cat fossil.asm >> $(BIN)fossil_xxx.asm
	$(PASMO) $(BIN)fossil_xxx.asm $@ $(BIN)fossil.sym

$(BIN)fossil_000.bin: fossil.asm $(BIN)rc2014-extended.rom.sym
	@mkdir -p bin
	echo "  ORG 0x000" > $(BIN)fossil_000.asm
	echo "  include \"$(BIN)rc2014-extended.rom.sym\"" >> $(BIN)fossil_000.asm
	cat fossil.asm >> $(BIN)fossil_000.asm
	$(PASMO) $(BIN)fossil_000.asm $@ $@.sym

$(BIN)fossil_100.bin: fossil.asm $(BIN)rc2014-extended.rom.sym
	@mkdir -p bin
	echo "  ORG 0x100" > $(BIN)fossil_100.asm
	echo "  include \"$(BIN)rc2014-extended.rom.sym\"" >> $(BIN)fossil_100.asm
	cat fossil.asm >> $(BIN)fossil_100.asm
	$(PASMO) $(BIN)fossil_100.asm $@ $@.sym

$(BIN)fossil-map.bin: $(addprefix $(BIN),fossil_000.bin fossil_100.bin)
	@node ./extract-relocation-map.js $^ $(BIN)fossil-map.bin

$(BIN)fossil-map-xxx.bin: $(addprefix $(BIN),fossil_xxx.bin)
	@node ./extract-relocation-map.js $^ $^ $(BIN)fossil-map-xxx.bin

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
