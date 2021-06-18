SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

BIN := ./bin/

all: bin/rc2014-extended.rom

# $(BIN)rs232_100.bin $(BIN)rs232-map.bin
bin/rc2014-extended.rom: rc2014-extended.asm msx.inc utils.asm rs232.asm sio.asm alloc.asm rominit.asm
	@mkdir -p bin
	./tools/zasm -v2 --labels -i rc2014-extended.asm -o ./bin/rc2014-extended.rom

# bin/rs232_100.bin: rs232.asm
# 	@mkdir -p bin
# 	echo "  ORG 0x100" > ./rs232_100.asm.tmp
# 	cat rs232.asm >> ./rs232_100.asm.tmp
# 	./tools/zasm  -v2 ./rs232_100.asm.tmp -o ./bin/rs232_100.bin -l ./bin/rs232_100.lst
# 	rm ./rs232_100.asm.tmp

# bin/rs232_200.bin: rs232.asm
# 	@mkdir -p bin
# 	echo "  ORG 0x100" > ./rs232_200.asm.tmp
# 	cat rs232.asm >> ./rs232_200.asm.tmp
# 	./tools/zasm  -v2 ./rs232_200.asm.tmp -o ./bin/rs232_200.bin -l ./bin/rs232_200.lst
# 	rm ./rs232_200.asm.tmp

# $(BIN)rs232-map.bin: $(addprefix $(BIN),rs232_100.bin rs232_200.bin)
# 	@node ./extract-relocation-map.js $^ $(BIN)rs232-map.bin

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
