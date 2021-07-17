SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ZCCRELFLAGS=
#-SO0 --max-allocs-per-node0
ifdef RELEASE
ZCCRELFLAGS=-SO3 --max-allocs-per-node200000 --allow-unsafe-read
# -Cs"--opt-code-speed"  --opt-code-speed
endif

ZCC := zcc +msx -subtype=msxdos2 -compiler=sdcc -Cs --Werror -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -pragma-define:CLIB_32BIT_FLOAT=1 -pragma-define:CRT_ENABLE_COMMANDLINE=0 -pragma-define:CRT_COMMANDLINE_REDIRECTION=0 -DAMALLOC1 $(ZCCRELFLAGS) -I./fusion/

telnet_sources = $(wildcard telnet/*.c) $(wildcard telnet/*.asm)

BIN := ./bin/
SRC := ./

.PHONY: all
all: $(addsuffix .com,$(addprefix $(BIN),fdisk dots lines mbrot vramtest cpusptst extbio rs232tst fosiltst fossil xrecv2))

include	Makefile.rules.mk

fdisk_files = $(call app_files,fdisk,fdisk/memmap-override.asm msxdos.asm)
$(BIN)fdisk.com: $(addprefix $(BIN),$(fdisk_files))

V9958_DEPS := v9958.c msx.asm v9958.asm

helloworld_files = $(call app_files,helloworld,)
$(BIN)helloworld.com: $(addprefix $(BIN),$(helloworld_files))

dots_files = $(call app_files,dots,$(V9958_DEPS) msxdos.asm)
$(BIN)dots.com: $(addprefix $(BIN),$(dots_files))

lines_files = $(call app_files,lines,$(V9958_DEPS) msxdos.asm)
$(BIN)lines.com: $(addprefix $(BIN),$(lines_files))

mbrot_files = $(call app_files,mbrot,$(V9958_DEPS) msxdos.asm)
$(BIN)mbrot.com: $(addprefix $(BIN),$(mbrot_files))

vramtest_files = $(call app_files,vramtest,$(V9958_DEPS))
$(BIN)vramtest.com: $(addprefix $(BIN),$(vramtest_files))

cpusptst_files = $(call app_files,cpusptst,)
$(BIN)cpusptst.com: $(addprefix $(BIN),$(cpusptst_files))

extbio_files = $(call app_files,extbio,extbio.asm msxdos.asm xstdio.asm getslt.asm)
$(BIN)extbio.com: $(addprefix $(BIN),$(extbio_files))

rs232tst_files = $(call app_files,rs232tst,extbio.asm crt_override.c msxdos.asm getslt.asm rs232.asm fusion/msx.c)
$(BIN)rs232tst.com: $(addprefix $(BIN),$(rs232tst_files))

fosiltst_files = $(call app_files,fosiltst,extbio.asm msxdos.asm xstdio.asm getslt.asm fossil.asm)
$(BIN)fosiltst.com: $(addprefix $(BIN),$(fosiltst_files))

fossil_files = $(call app_files,fossil,extbio.asm getslt.asm)
$(BIN)fossil.com: $(addprefix $(BIN),$(fossil_files))

telnet_files = $(call app_files,telnet,extbio.asm crt_override.c fossil.asm getslt.asm msxdos.asm xymodem.c aofossilhelper.c fusion/io.asm fusion/inkey.asm fusion/inputstring.asm fusion/inputchar.asm fusion/width.c)
$(BIN)telnet.com: $(addprefix $(BIN),$(telnet_files))

xrecv2_files = $(call app_files,xrecv2,extbio.asm crt_override.c fossil.asm getslt.asm msxdos.asm fusion/io.asm fusion/inkey.asm fusion/inputstring.asm fusion/inputchar.asm fusion/msx.c)
$(BIN)xrecv2.com: $(addprefix $(BIN),$(xrecv2_files))

clean:
	@rm -rf ./bin
