SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules
PATH := $(PWD)/tools:$(PATH)
MSXSYS ?= base300
COUNTRY ?= aust
RTC ?= with
WRK_DIR := ./working/$(COUNTRY)-$(RTC)-rtc/

.PRECIOUS: $(WRK_DIR)%.hex $(WRK_DIR)%.mac $(WRK_DIR)%.inc

all: $(WRK_DIR)mainrom.rom $(WRK_DIR)subrom.rom $(WRK_DIR)optrom.rom

PREP := $(shell WRK_DIR="$(WRK_DIR)" $(PWD)/tools/prep.sh > /dev/null; echo "$$?")
ifneq ($(PREP),0)
  $(error msxsys prep.sh failed.)
endif

WORKING_PREP := $(shell WRK_DIR="working/" $(PWD)/tools/prep.sh > /dev/null; echo "$$?")
ifneq ($(WORKING_PREP),0)
  $(error msxsys/working prep.sh failed.)
endif

$(WRK_DIR)msx.inc: base300.inc
	@unix2dos -f -n "$<" "$@"
	./tools/country-to-code.sh $(COUNTRY)
	CNTRY=$$(./tools/country-to-code.sh $(COUNTRY))
	echo "$$(echo -n CNTRY  EQU  $${CNTRY}; cat "$@")" > "$@"
	echo "$$(echo; cat "$@")" > "$@"
	if [ "$(RTC)" = "with" ]; then
		echo "$$(echo -n RTC  EQU  1; cat "$@")" > "$@"
	else
		echo "$$(echo -n RTC  EQU  0; cat "$@")" > "$@"
	fi
	echo "$$(echo; cat "$@")" > "$@"
	unix2dos -f -n "$@" "$@"

$(WRK_DIR)%.mac: msxsyssrc/$(MSXSYS)/%.mac
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.l80: msxsyssrc/$(MSXSYS)/%.l80
	@unix2dos -f -n "$<" "$@"

working/music100.mac: msxsyssrc/music100/music.mac
	@unix2dos -f -n "$<" "$@"

working/fmpac1.mac: msxsyssrc/music100/fmpac-1.mac
	@unix2dos -f -n "$<" "$@"

working/fmpac2.mac: msxsyssrc/music100/fmpac-2.mac
	@unix2dos -f -n "$<" "$@"

working/fmpac3.mac: msxsyssrc/music100/fmpac-3.mac
	@unix2dos -f -n "$<" "$@"

working/music410.mac: msxsyssrc/music410/music.mac
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.mac: msxsyssrc/basekey/%.mac
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.mac: msxsyssrc/basechr/%.mac
	@unix2dos -f -n "$<" "$@"

$(WRK_DIR)%.rel: $(WRK_DIR)%.mac
	@echo -e "\nAssembling \e[32m$<\e[0m"
	cd $(WRK_DIR)
	m80.sh $(notdir $<) $(notdir $@) 2>&1 | grep -v "Sorry, terminal not found, using cooked mode."

working/%.rel: working/%.mac
	@echo -e "\nAssembling \e[32m$<\e[0m"
	cd working
	m80.sh $(notdir $<) $(notdir $@) 2>&1 | grep -v "Sorry, terminal not found, using cooked mode."

$(WRK_DIR)%.hex:

$(WRK_DIR)bio.rel: $(addprefix $(WRK_DIR), msx.inc keyint.mac keyfr.mac keyuk.mac keyger.mac keyrus.mac keyspa.mac)
$(WRK_DIR)msxio.rel: $(addprefix $(WRK_DIR), msx.inc keyint.mac keyfr.mac keyuk.mac keyger.mac keyrus.mac keyspa.mac)
$(WRK_DIR)biohdr.rel: $(addprefix $(WRK_DIR), msx.inc)
$(WRK_DIR)msxchr.rel: $(addprefix $(WRK_DIR), msx.inc chrint.mac chrkor.mac chrger.mac chrrus.mac chrjapv2.mac)
$(WRK_DIR)basic1.rel: $(addprefix $(WRK_DIR), msx.inc)
$(WRK_DIR)basic2.rel: $(addprefix $(WRK_DIR), msx.inc)
$(WRK_DIR)optrom.rel: $(addprefix $(WRK_DIR), msx.inc)
$(WRK_DIR)subrom.rel: $(addprefix $(WRK_DIR), msx.inc)

$(WRK_DIR)%.rom: $(addprefix $(WRK_DIR), %.hex)
	@hex2bin $<
	mv $(basename $@).bin $@

working/%.rom: $(addprefix working/, %.hex)
	@hex2bin $<
	mv $(basename $@).bin $@

$(WRK_DIR)mainrom.hex: $(addprefix $(WRK_DIR), mainrom.l80 biohdr.rel slot.rel msxio.rel queuetl.rel msxgrp.rel caset.rel bio.rel msxchr.rel msxinl.rel bashdr.rel f4.rel bintrp.rel msxexp.rel maclng.rel gengrp.rel advgrp.rel biptrg.rel biprtu.rel bimisc.rel bistrs.rel spcdsk.rel dskcom.rel spcdev.rel basbio.rel msxsts.rel init.rel)
	@cd $(WRK_DIR)
	l80.sh mainrom.hex mainrom.l80

$(WRK_DIR)main.rom: $(WRK_DIR)mainrom.rom
	@cp $< $@

$(WRK_DIR)subrom.hex: $(addprefix $(WRK_DIR), subrom.l80 subrom.rel)
	@cd $(WRK_DIR)
	l80.sh subrom.hex subrom.l80

$(WRK_DIR)optrom.hex: $(addprefix $(WRK_DIR), optrom.l80 optrom.rel)
	@cd $(WRK_DIR)
	l80.sh optrom.hex optrom.l80

working/music100.hex: working/music100.rel
	@cd working
	l80.sh music100.hex MUSIC100/X/E/N/P:4000,MUSIC100

working/fmpac1.hex: working/fmpac1.rel
	@cd working
	l80.sh fmpac1.hex FMPAC1/X/E/N/P:4000,FMPAC1

working/fmpac2.hex: working/fmpac2.rel
	@cd working
	l80.sh fmpac2.hex FMPAC2/X/E/N/P:4000,FMPAC2

working/fmpac3.hex: working/fmpac3.rel
	@cd working
	l80.sh fmpac3.hex FMPAC3/X/E/N/P:4000,FMPAC3

working/music410.hex: working/music410.rel
	@cd working
	l80.sh music410.hex MUSIC410/X/E/N/P:4000,MUSIC410

clean:
	@rm -rf ./working
