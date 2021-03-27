SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
WRK_DIR := ./working/
PATH := $(PWD)/tools:$(PATH)
MSXSYS ?= base300

.PRECIOUS: $(WRK_DIR)%.hex $(WRK_DIR)%.mac $(WRK_DIR)%.inc

all: $(WRK_DIR)main.rom $(WRK_DIR)subrom.rom $(WRK_DIR)optrom.rom

PREP := $(shell $(PWD)/tools/prep.sh > /dev/null; echo "$$?")
ifneq ($(PREP),0)
  $(error prep.sh failed.)
endif

$(WRK_DIR)msx.inc: $(MSXSYS).inc
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.mac: msxsyssrc/$(MSXSYS)/%.mac
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.mac: msxsyssrc/basekey/%.mac
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.mac: msxsyssrc/basechr/%.mac
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.rel: $(WRK_DIR)%.mac
	@echo -e "\nAssembling \e[32m$<\e[0m"
	cd $(WRK_DIR)
	m80.sh $(notdir $<) $(notdir $@) 2>&1 | grep -v "Sorry, terminal not found, using cooked mode."

$(WRK_DIR)%.hex:

$(WRK_DIR)bios.mac: $(addprefix $(WRK_DIR), msx.inc keyint.mac chrint.mac)
$(WRK_DIR)basic1.mac: $(addprefix $(WRK_DIR), msx.inc)
$(WRK_DIR)basic2.mac: $(addprefix $(WRK_DIR), msx.inc)

$(WRK_DIR)%.rom: $(addprefix $(WRK_DIR), %.hex)
	@hex2bin $<
	mv $(basename $@).bin $@

$(WRK_DIR)main.hex: $(addprefix $(WRK_DIR), bios.rel basic1.rel basic2.rel)
	@cd $(WRK_DIR)
	l80.sh main.hex MAIN/X/E/N/P:0,BIOS,BASIC1,BASIC2

$(WRK_DIR)subrom.hex: $(addprefix $(WRK_DIR), subrom.rel)
	@cd $(WRK_DIR)
	l80.sh subrom.hex SUBROM/X/E/N/P:0,SUBROM

$(WRK_DIR)optrom.hex: $(addprefix $(WRK_DIR), optrom.rel)
	@cd $(WRK_DIR)
	l80.sh optrom.hex OPTROM/X/E/N/P:4000,OPTROM

clean:
	rm -rf ./working
