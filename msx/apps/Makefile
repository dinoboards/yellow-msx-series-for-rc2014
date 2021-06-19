SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

BIN := ./bin/
SRC := ./
APPS := dots lines mbrot spike-fdd ide cpusptst fdisk vramtest extbio rs232tst fosiltst
TARGETS := $(addsuffix .com,$(addprefix $(BIN),$(APPS)))

ZCC := zcc +msx -create-app -subtype=msxdos2 -compiler=sdcc -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1

.PHONY: all
all: $(TARGETS) bin/telnet.com

V9958_DEPS := v9958.c v9958.h msx.asm v9958.asm msx.inc
$(BIN)dots.com: $(addprefix $(SRC),dots.c $(V9958_DEPS) msxdos.asm msxdos.h)

V9958_DEPS := v9958.c v9958.h msx.asm v9958.asm msx.inc
$(BIN)dots.com: $(addprefix $(SRC),dots.c $(V9958_DEPS) msxdos.asm msxdos.h)
$(BIN)vramtest.com: $(addprefix $(SRC),vramtest.c $(V9958_DEPS))
$(BIN)lines.com: $(addprefix $(SRC),lines.c $(V9958_DEPS))
$(BIN)mbrot.com: $(addprefix $(SRC),mbrot.c $(V9958_DEPS))
$(BIN)spike-fdd.com: $(addprefix $(SRC),spike-fdd.c fdd.asm)
$(BIN)ide.com: $(addprefix $(SRC),spike-ide.c spike-ide.asm spike-ide.h utils.asm cfg.inc msxdos.asm msxdos.h partition.h)
$(BIN)cpusptst.com: $(addprefix $(SRC),cpusptst.c cpusptst.asm)
$(BIN)fdisk.com: $(addprefix $(SRC),memmap-override.asm fdisk.c fdisk.h fdisk2.c fdisk2.h datatypes.h msxdos.asm msxdos.h partition.h)
$(BIN)extbio.com: $(addprefix $(SRC),extbio.c msxdos.h msxdos.asm extbio.asm telnet/fossil_interface.asm)
$(BIN)rs232tst.com: $(addprefix $(SRC),rs232tst.c msxdos.h msxdos.asm extbio.asm)
$(BIN)fosiltst.com: $(addprefix $(SRC),fosiltst.c msxdos.h msxdos.asm extbio.asm telnet/fossil_interface.asm)

$(TARGETS):
	@mkdir -p $(BIN)
	$(ZCC) $(filter-out %.inc,$(filter-out %.h,$^)) -o $(notdir $@)
	mv $(notdir $@) "$@"
	rm -f $(basename $(notdir $@)).img
	rm -f $(basename $(notdir $@))_BSS.bin
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"

.PHONY: bin/telnet.com
bin/telnet.com:
	@$(MAKE) --no-print-directory -C ./telnet
	cp -up ./telnet/bin/telnet.com ./bin/

clean:
	@rm -rf ./bin
	$(MAKE) -C telnet --no-print-directory clean
