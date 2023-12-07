SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

ZCCRELFLAGS=
ifdef RELEASE
ZCCRELFLAGS=-SO3 --max-allocs-per-node500000 --allow-unsafe-read
endif

LIBS := -I./fdisk/ -I./libraries/msx/ -I./telnet/ -I./libraries/fusion/ -I./libraries/msxbios -I./libraries/msxdos -I./libraries/fossil  -I./libraries/delay -I./xrecv2 -I./libraries/usb -I./libraries

# -DSTACK_TRACE_ENABLED
ZCC := zcc +msx -subtype=msxdos2 -compiler=sdcc --vc -Cs --Werror -lmath32 -Cc-D__MATH_MATH32 -D__MATH_MATH32 -g -pragma-define:CLIB_32BIT_FLOAT=1 -DAMALLOC1 -pragma-define:CRT_MODEL=0 -pragma-define:CRT_COMMANDLINE_REDIRECTION=0 $(ZCCRELFLAGS) $(LIBS)
ZCC_FOR_sys := zcc  +msx --vc -subtype=rom -startup=1  -compiler=sdcc -Cs --Werror -D__MATH_MATH32 -g $(ZCCRELFLAGS) $(LIBS)

BIN := ./bin/
SRC := ./

#
.PHONY: all
all:  $(addsuffix .sys,$(addprefix $(BIN),hello prnt ftdi sio2)) $(addsuffix .com,$(addprefix $(BIN),ctty mode fdisk fossil sraltest service memory helloworld clrdir dots lines mbrot vramtest cpusptst extbio rs232tst fosiltst fossilon rtccalb term esp8266 romflash chip8 chip8asm muflash siocfg usbtst usbctrl))

include	Makefile.rules.mk

zcchelp:
	@$(ZCC) -Cl

LIBGEN_LIB := $(addprefix $(BIN)libraries/msx/libgen/,basename.c.lib dirname.c.lib)
GETSLT_LIB := $(addprefix $(BIN),getslt.lib)
CONFIG_REQUEST_LIB := $(addprefix $(BIN),config_request.c.lib)
SYSTEM_VARS_LIB := $(addprefix $(BIN)libraries/msxbios/,system_vars.c.lib)
MSXBIOS_LIB := $(addprefix $(BIN),$(patsubst %.asm,%.lib,$(wildcard libraries/msxbios/*.asm)))
MSXDOS_LIB := $(addprefix $(BIN),$(patsubst %.asm,%.lib,$(wildcard libraries/msxdos/*.asm))) $(MSXBIOS_LIB)
EXTBIO_LIB := $(addprefix $(BIN),$(patsubst %.asm,%.lib,$(wildcard libraries/extbio/*.asm))) $(addprefix $(BIN),$(patsubst %.c,%.c.lib,$(wildcard libraries/extbio/*.c)))
EXTBIO_USB_LIB :=  $(addprefix $(BIN)libraries/extbio/,extbio_usb_control_transfer.lib extbio_usb_data_out_transfer.lib extbio_usb_data_in_transfer_n.lib extbio_usb_data_in_transfer.lib)
FOSSIL_LIB := $(addprefix $(BIN)libraries/fossil/,fossil.lib fossil.c.lib)
DELAY_LIB := $(BIN)libraries/delay/delay.c.lib
XMODEM_LIB :=  $(addprefix $(BIN)libraries/xmodem/,xmodem.c.lib crc16.c.lib utils.c.lib)

USB_KERNEL_LIB := $(addprefix $(BIN)libraries/usb/,ch376.lib usb_state.c.lib ch376.c.lib transfers.c.lib dev_transfers.c.lib enumerate_storage.c.lib enumerate.c.lib enumerate_hub.c.lib)
USB_USER_LIB := $(addprefix $(BIN)libraries/usb/,format_mapping.c.lib trace_ufi_format_capacities_response.c.lib class_printer.c.lib class_hid.c.lib class_hid_keyboard.c.lib class_ufi.c.lib protocol.c.lib usb_cbi.c.lib dev_transfers.c.lib class_hub.c.lib class_printer.c.lib usb_cbi.c.lib class_scsi.c.lib find_device.c.lib)

FUSION_LIB := $(addprefix $(BIN)libraries/fusion/,io.lib fprintf.c.lib interrupts.lib inputstring.lib inkey.lib inputchar.lib io.lib)
V9958_LIB := $(addprefix $(BIN),v9958.c.lib v9958.lib)
SYS_LIB := $(addprefix $(BIN)libraries/sysload/, sysload.c.lib sysload.lib)

.PHONY: list
list:
	@LC_ALL=C $(MAKE) -pRrq -f $(firstword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/(^|\n)# Files(\n|$$)/,/(^|\n)# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | grep -E -v -e '^[^[:alnum:]]' -e '^$@$$'


ftdi.body := $(addprefix $(BIN),$(call app_file,sys/ftdi/body)) $(BIN)sys/ftdi/vendor_ftdi.c.lib $(GETSLT_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB) $(USB_USER_LIB) $(USB_KERNEL_LIB) $(DELAY_LIB)
ftdi.init := $(addprefix $(BIN),$(call app_file,sys/ftdi/init)) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYS_LIB) $(SYSTEM_VARS_LIB) $(USB_USER_LIB)
$(eval $(call relocatable,ftdi))

sio2.body := $(addprefix $(BIN),$(call app_file,sys/sio2/body)) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
sio2.init := $(addprefix $(BIN),$(call app_file,sys/sio2/init)) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYS_LIB) $(SYSTEM_VARS_LIB)
$(eval $(call relocatable,sio2))

hello.body := $(addprefix $(BIN),$(call app_file,sys/hello/body)) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
hello.init := $(addprefix $(BIN),$(call app_file,sys/hello/init)) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYS_LIB) $(SYSTEM_VARS_LIB)
$(eval $(call relocatable,hello))

prnt.body := $(addprefix $(BIN),$(call app_file,sys/prnt/body)) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
prnt.init := $(addprefix $(BIN),$(call app_file,sys/prnt/init)) $(DELAY_LIB) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYS_LIB) $(SYSTEM_VARS_LIB) $(USB_USER_LIB)
$(eval $(call relocatable,prnt))

$(BIN)fossil.com: $(BIN)fossil.sys
	@cp $< $@

fossil.body := $(addprefix $(BIN),$(call app_file,sys/fossil/body)) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
fossil.init := $(addprefix $(BIN),$(call app_file,sys/fossil/init)) $(DELAY_LIB) $(EXTBIO_LIB) $(GETSLT_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB) $(USB_USER_LIB)
$(eval $(call relocatable,fossil))

service_files = $(call app_file,service)
$(BIN)service.com: $(addprefix $(BIN),$(service_files)) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
service.com.crt_enable_commandline:=1

memory_files = $(call app_file,memory)
$(BIN)memory.com: $(addprefix $(BIN),$(memory_files)) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB) $(EXTBIO_LIB) $(GETSLT_LIB)
memory.com.crt_enable_commandline:=1

helloworld_files = ./helloworld.c.o
$(BIN)helloworld.com: $(addprefix $(BIN),$(helloworld_files)) $(LIBGEN_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
helloworld.com.crt_enable_commandline:=1

ctty_files = $(call app_file,ctty,memmap-override.asm)
$(BIN)ctty.com: $(addprefix $(BIN),$(ctty_files)) $(MSXDOS_LIB) $(EXTBIO_LIB) $(DELAY_LIB) $(FUSION_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
ctty.com.crt_enable_commandline:=1

mode_files = $(call app_file,mode,memmap-override.asm)
$(BIN)mode.com: $(addprefix $(BIN),$(mode_files)) $(MSXDOS_LIB) $(EXTBIO_LIB) $(DELAY_LIB) $(FUSION_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
mode.com.crt_enable_commandline:=1

sraltest_files = $(call app_file,sraltest,memmap-override.asm)
$(BIN)sraltest.com: $(addprefix $(BIN),$(sraltest_files)) $(MSXDOS_LIB) $(EXTBIO_LIB) $(DELAY_LIB) $(FUSION_LIB) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)
sraltest.com.crt_enable_commandline:=1

fdisk_files = $(call app_files,fdisk,memmap-override.asm)
$(BIN)fdisk.com: $(addprefix $(BIN),$(fdisk_files)) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB)

clrdir_files = $(call app_file,clrdir)
$(BIN)clrdir.com: $(addprefix $(BIN),$(clrdir_files)) $(MSXDOS_LIB) $(FUSION_LIB) $(SYSTEM_VARS_LIB)
clrdir.com.crt_enable_commandline:=1

usbctrl_files = $(call app_file,usbctrl)
$(BIN)usbctrl.com: $(addprefix $(BIN),$(usbctrl_files)) $(MSXDOS_LIB) $(EXTBIO_LIB) $(DELAY_LIB) $(FUSION_LIB) $(USB_USER_LIB) $(SYSTEM_VARS_LIB)
usbctrl.com.crt_enable_commandline:=1

usbtst_files = $(call app_file,usbtst)
$(BIN)usbtst.com: $(addprefix $(BIN),$(usbtst_files)) $(BIN)sys/ftdi/vendor_ftdi.c.lib $(MSXDOS_LIB) $(USB_USER_LIB) $(USB_KERNEL_LIB)  $(SYSTEM_VARS_LIB) $(DELAY_LIB) $(EXTBIO_LIB)
usbtst.com.crt_enable_commandline:=1

dots_files = $(call app_file,dots)
$(BIN)dots.com: $(addprefix $(BIN),$(dots_files)) $(MSXDOS_LIB) $(V9958_LIB) $(CONFIG_REQUEST_LIB)

lines_files = $(call app_file,lines)
$(BIN)lines.com: $(addprefix $(BIN),$(lines_files)) $(MSXDOS_LIB) $(V9958_LIB) $(CONFIG_REQUEST_LIB)

mbrot_files = $(call app_file,mbrot)
$(BIN)mbrot.com: $(addprefix $(BIN),$(mbrot_files)) $(MSXDOS_LIB) $(V9958_LIB) $(CONFIG_REQUEST_LIB)

vramtest_files = $(call app_file,vramtest)
$(BIN)vramtest.com: $(addprefix $(BIN),$(vramtest_files)) $(MSXDOS_LIB) $(V9958_LIB)

cpusptst_files = $(call app_file,cpusptst)
$(BIN)cpusptst.com: $(addprefix $(BIN),$(cpusptst_files))

extbio_files = $(call app_file,extbio)
$(BIN)extbio.com: $(addprefix $(BIN),$(extbio_files)) $(MSXDOS_LIB) $(EXTBIO_LIB) $(EXTBIO_LIB) $(GETSLT_LIB) $(SYSTEM_VARS_LIB)

rs232tst_files = $(call app_files,rs232tst,rs232.asm)
$(BIN)rs232tst.com: $(addprefix $(BIN),$(rs232tst_files)) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB) $(EXTBIO_LIB) $(GETSLT_LIB)

fosiltst_files = $(call app_files,fosiltst,xstdio.asm)
$(BIN)fosiltst.com: $(addprefix $(BIN),$(fosiltst_files)) $(MSXDOS_LIB) $(FOSSIL_LIB) $(SYSTEM_VARS_LIB) $(EXTBIO_LIB) $(GETSLT_LIB)

fossilon_files = $(call app_file,fossilon)
$(BIN)fossilon.com: $(addprefix $(BIN),$(fossilon_files)) $(EXTBIO_LIB) $(GETSLT_LIB)

term_files = $(call app_files,term,memmap-override.asm)
$(BIN)term.com: $(addprefix $(BIN),$(term_files)) $(MSXDOS_LIB) $(SYSTEM_VARS_LIB) $(EXTBIO_LIB) $(FUSION_LIB) $(GETSLT_LIB)
term.com.crt_enable_commandline:=1

xrecv2_files = $(call app_files,xrecv2,memmap-override.asm print.c)
$(BIN)xrecv2.com: $(addprefix $(BIN),$(xrecv2_files)) $(MSXDOS_LIB) $(FOSSIL_LIB) $(DELAY_LIB) $(SYSTEM_VARS_LIB) $(EXTBIO_LIB) $(FUSION_LIB) $(GETSLT_LIB) $(XMODEM_LIB)
xrecv2.com.crt_enable_commandline:=1

rtccalb_files = $(call app_files,rtccalb,memmap-override.asm print.c)
$(BIN)rtccalb.com: $(addprefix $(BIN),$(rtccalb_files)) $(MSXDOS_LIB)
rtccalb.com.crt_enable_commandline:=1

esp8266_files = $(call app_files,esp8266,memmap-override.asm)
$(BIN)esp8266.com: $(addprefix $(BIN),$(esp8266_files)) $(LIBGEN_LIB) $(MSXDOS_LIB) $(EXTBIO_LIB) $(SYSTEM_VARS_LIB) $(DELAY_LIB) $(GETSLT_LIB) $(XMODEM_LIB)
esp8266.com.crt_enable_commandline:=1

siocfg_files = $(call app_files,siocfg,memmap-override.asm)
$(BIN)siocfg.com: $(addprefix $(BIN),$(siocfg_files)) $(EXTBIO_LIB) $(GETSLT_LIB)
siocfg.com.crt_enable_commandline:=1

romflashwriter_files = $(call app_file,romflashwriter)
$(BIN)romflashwriter/romflashwriter.bin: ./romflashwriter/romflashwriter.asm
	@mkdir -p $(dir $@)
	zcc +z80 -startup=0 -SO3 -clib=sdcc_iy --no-crt -nostdlib -create-app -pragma-define:CRT_ORG_CODE=0x0C000 ./romflashwriter/romflashwriter.asm -o ./bin/romflashwriter/romflashwriter.bin
	echo "Assembled $< to romflashwriter.bin"

#-pragma-define:CRT_ORG_CODE=0x0C000

$(BIN)romflashwriter/romflashwriter.inc: $(BIN)romflashwriter/romflashwriter.bin
	@./romflashwriter/export.sh $(BIN)romflashwriter/romflashwriter.bin $(BIN)romflashwriter/romflashwriter.inc
	echo "exported romflashwriter.bin to romflashwriter.inc"

./bin/romflash/flashloader.o: romflash/flashloader.asm $(BIN)romflashwriter/romflashwriter.inc

romflash_files = $(call app_files,romflash,getslt.asm)
$(BIN)romflash.com: $(BIN)romflashwriter/romflashwriter.inc $(addprefix $(BIN),$(romflash_files)) $(EXTBIO_LIB)
romflash.com.crt_enable_commandline:=1

muflash_files = $(call app_file,muflash)
$(BIN)muflash.com: $(addprefix $(BIN),$(muflash_files)) $(SYSTEM_VARS_LIB)
muflash.com.crt_enable_commandline:=1

.PHONY: bin/chip8.com
bin/chip8.com:
	@$(MAKE) --no-print-directory -C chip8 chip8 -j
	cp ./chip8/bin/chip8.com ./bin/chip8.com

.PHONY: bin/chip8asm.com
bin/chip8asm.com:
	@$(MAKE) --no-print-directory -C chip8 chip8asm -j
	cp ./chip8/bin/chip8asm.com ./bin/chip8asm.com

.PHONY: test
test: chip8test

.PHONY: chip8test
chip8test:
	$(MAKE) --no-print-directory -C chip8 test -j

.PHONY: clean
clean:
	@rm -rf ./bin
	$(MAKE) --no-print-directory -C chip8 clean

.PHONY: format
format: SHELL:=/bin/bash
format:
	find \( -name "*.c" -o -name "*.h" \) -not -path "./chip8/*" -exec echo "formating {}" \; -exec clang-format -i {} \;
