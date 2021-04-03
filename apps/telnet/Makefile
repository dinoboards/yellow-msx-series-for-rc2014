SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

BIN := ./bin/
SRC := ./
APPS := telnet
TARGETS := $(addsuffix .com,$(addprefix $(BIN),$(APPS)))

ZCC := zcc +msx -create-app -subtype=msxdos2 -compiler=sdcc -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1
#--c-code-in-asm
#-SO1 --max-allocs-per-node200000

# $(BIN)telnet.com: $(addprefix $(SRC),fake_connection.c print.asm inputchar.asm inputstring.asm io.asm xymodem.c inkey.asm screen.asm width.c msx2ansibuffer.asm print.c fossil_interface.asm fossil_interface.c telnet.c)
$(BIN)telnet.com: $(addprefix $(SRC),telnet.c print.asm print.c msx2ansibuffer.asm inkey.asm)

#aofossilhelper.c

$(TARGETS):
	@mkdir -p $(BIN)
	# $(ZCC) -a --c-code-in-asm $(filter-out %.inc,$(filter-out %.h,$^)) -o $(notdir $@)
	$(ZCC) $(filter-out %.inc,$(filter-out %.h,$^)) -o $(notdir $@)
	mv $(notdir $@) "$@"
	rm -f $(basename $(notdir $@)).img
	rm -f $(basename $(notdir $@))_BSS.bin
	echo "Compiled $(notdir $@) from $(filter-out %.h,$^)"

clean:
	@rm -rf ./bin
