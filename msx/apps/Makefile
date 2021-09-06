SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ZCCRELFLAGS=
ifdef RELEASE
ZCCRELFLAGS=-SO3 --max-allocs-per-node200000 --allow-unsafe-read
endif

LIBS := -I./fdisk/ -I./telnet/ -I./libraries/fusion/ -I./libraries/msxbios -I./libraries/msxdos -I./libraries/fossil  -I./libraries/delay

ZCC := zcc +msx -subtype=msxdos2 -compiler=sdcc  -Cs --Werror -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1-DAMALLOC1 -pragma-define:CRT_MODEL=0 -pragma-define:CRT_COMMANDLINE_REDIRECTION=0 $(ZCCRELFLAGS) $(LIBS)

BIN := ./bin/
SRC := ./

.PHONY: all
all: $(addsuffix .com,$(addprefix $(BIN),fdisk dots lines mbrot vramtest cpusptst extbio rs232tst fosiltst fossilon xrecv helloworld rtccalb term esp8266))

include	Makefile.rules.mk

SYSTEM_VARS := libraries/msxbios/system_vars.c
MSXDOS := libraries/msxdos/msxdos.asm
FOSSIL := libraries/fossil/fossil.asm libraries/fossil/fossil.c
DELAY := libraries/delay/delay.c

fdisk_files = $(call app_files,fdisk,memmap-override.asm $(MSXDOS) $(SYSTEM_VARS))
$(BIN)fdisk.com: $(addprefix $(BIN),$(fdisk_files))

V9958_DEPS := v9958.c msx.asm v9958.asm

helloworld_files = $(call app_files,helloworld,$(MSXDOS) $(SYSTEM_VARS))
$(BIN)helloworld.com: $(addprefix $(BIN),$(helloworld_files))
helloworld.com.crt_enable_commandline:=1

dots_files = $(call app_files,dots,$(V9958_DEPS) $(MSXDOS))
$(BIN)dots.com: $(addprefix $(BIN),$(dots_files))

lines_files = $(call app_files,lines,$(V9958_DEPS) $(MSXDOS))
$(BIN)lines.com: $(addprefix $(BIN),$(lines_files))

mbrot_files = $(call app_files,mbrot,$(V9958_DEPS) $(MSXDOS))
$(BIN)mbrot.com: $(addprefix $(BIN),$(mbrot_files))

vramtest_files = $(call app_files,vramtest,$(V9958_DEPS))
$(BIN)vramtest.com: $(addprefix $(BIN),$(vramtest_files))

cpusptst_files = $(call app_files,cpusptst,)
$(BIN)cpusptst.com: $(addprefix $(BIN),$(cpusptst_files))

extbio_files = $(call app_files,extbio,libraries/extbio/extbio.asm xstdio.asm getslt.asm $(MSXDOS) )
$(BIN)extbio.com: $(addprefix $(BIN),$(extbio_files))

rs232tst_files = $(call app_files,rs232tst,libraries/extbio/extbio.asm getslt.asm rs232.asm $(MSXDOS) $(SYSTEM_VARS))
$(BIN)rs232tst.com: $(addprefix $(BIN),$(rs232tst_files))

fosiltst_files = $(call app_files,fosiltst,libraries/extbio/extbio.asm $(MSXDOS) xstdio.asm getslt.asm $(FOSSIL))
$(BIN)fosiltst.com: $(addprefix $(BIN),$(fosiltst_files))

fossilon_files = $(call app_files,fossilon,libraries/extbio/extbio.asm getslt.asm)
$(BIN)fossilon.com: $(addprefix $(BIN),$(fossilon_files))

term_files = $(call app_files,term,memmap-override.asm libraries/extbio/extbio.asm $(FOSSIL) getslt.asm $(addprefix libraries/fusion/,io.asm inkey.asm inputstring.asm inputchar.asm) $(MSXDOS) $(SYSTEM_VARS))
$(BIN)term.com: $(addprefix $(BIN),$(term_files))
term.com.crt_enable_commandline:=1

xrecv2_files = $(call app_files,xrecv2,memmap-override.asm libraries/extbio/extbio.asm $(FOSSIL) $(DELAY) getslt.asm print.c $(addprefix libraries/fusion/,io.asm inkey.asm inputstring.asm inputchar.asm) $(MSXDOS) $(SYSTEM_VARS))
$(BIN)xrecv2.com: $(addprefix $(BIN),$(xrecv2_files))
xrecv2.com.crt_enable_commandline:=1

rtccalb_files = $(call app_files,rtccalb,memmap-override.asm print.c $(MSXDOS))
$(BIN)rtccalb.com: $(addprefix $(BIN),$(rtccalb_files))
rtccalb.com.crt_enable_commandline:=1

esp8266_files = $(call app_files,esp8266,memmap-override.asm libraries/extbio/extbio.asm $(FOSSIL) getslt.asm print.c $(SYSTEM_VARS))
$(BIN)esp8266.com: $(addprefix $(BIN),$(esp8266_files))
esp8266.com.crt_enable_commandline:=1

bin/xrecv.com: bin/xrecv2.com
	@cp bin/xrecv2.com bin/xrecv.com

.PHONY: clean
clean:
	@rm -rf ./bin

.PHONY: format
format: SHELL:=/bin/bash
format:
	find \( -name "*.c" -o -name "*.h" \) -exec echo "formating {}" \; -exec clang-format -i {} \;
