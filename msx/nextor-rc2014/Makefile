SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
MAKEFLAGS += --warn-undefined-variables
MAKEFLAGS += --no-builtin-rules

.DELETE_ON_ERROR:

BINDIR := ./bin/

ASM = sjasmplus --msg=war --nologo --fullpath --inc=../nextor/source/kernel/ --raw=$@ --sym=$(basename $@).sym.asm --lst=$@.lst --exp=$(basename $@).exp.asm

all: $(BINDIR)cbios.nextor.rom $(BINDIR)rc2014.nextor.rom

.PHONY: clean
clean:
	@rm -rf bin
	cd ../nextor/source && make clean --no-print-directory -f Makefile-rc2014.mk

$(BINDIR)rcembdrv.bin: rcembdrv.asm embinc.asm rcembdrv.inc
	@mkdir -p $(BINDIR)
	$(ASM) rcembdrv.asm rcembdrv.inc
	filesize=$$(stat -c%s "$(BINDIR)rcembdrv.bin")
	if ((filesize > 16484 )); then
		echo -e "\r\nError: $(BINDIR)rcembdrv.bin exceeded size of 16k"
		exit 1
	fi
	echo "Assembled $@"

$(BINDIR)rc2014dr.bin: rc2014dr.asm cfdrv.asm musicdriver.asm embinc.asm rc2014dr.inc.asm version.asm ch376drv.asm $(BINDIR)spike.bin drvend.asm
	@mkdir -p $(BINDIR)
	$(ASM) rc2014dr.asm ch376drv.asm rc2014dr.inc.asm drvend.asm
	cat $(BINDIR)rc2014dr.bin $(BINDIR)spike.bin > $(BINDIR)tmp.bin
	rm $(BINDIR)rc2014dr.bin
	mv $(BINDIR)tmp.bin $(BINDIR)rc2014dr.bin
	filesize=$$(stat -c%s "$(BINDIR)rc2014dr.bin")
	if ((filesize > 16484 )); then
		echo -e "\r\nError: $(BINDIR)rc2014dr.bin exceeded size of 16k"
		exit 1
	fi
	echo "Assembled $@"

$(BINDIR)chgbnk.bin: chgbnk.asm
	@mkdir -p $(BINDIR)
	$(ASM) $^
	sjasmplus --fullpath --inc=../nextor/source/kernel/ --lst=$(BINDIR)chgbnk.lst --exp=$(BINDIR)chgbnk.exp.asm --raw=$(BINDIR)chgbnk.bin --sym=$(BINDIR)chgbnk.sym.asm chgbnk.asm
	echo "Assembled $@"

cbios-nextor-kernel:
	@cd ../nextor/source && BUILD_TYPE=cbios $(MAKE) --no-print-directory -f Makefile-rc2014.mk

rc2014-nextor-kernel:
	@cd ../nextor/source && BUILD_TYPE=rc2014 $(MAKE) --no-print-directory -f Makefile-rc2014.mk


define BUILD_versions =
# Build a FAT12 floppy disk image containing nextor.sys, command2.com and other files
.PHONY: $(BINDIR)$1.fdd.dsk
$(BINDIR)$1.fdd.dsk: $1-nextor-kernel $(BINDIR)rcembdrv.bin
	@mkdir -p $(BINDIR)
	DATSIZ=$$$$(./scripts/getsymb.sh $(BINDIR)rcembdrv.exp.asm DATSIZ)
	rm -f $(BINDIR)$1.fdd.dsk
	dd status=none if=/dev/zero of=$(BINDIR)$1.fdd.dsk bs=$$$$(($$$$DATSIZ*18)) count=1
	mkfs.vfat -F 12 -f 1 $(BINDIR)$1.fdd.dsk > /dev/null
	mmd -i $(BINDIR)$1.fdd.dsk system
	mcopy -i $(BINDIR)$1.fdd.dsk ../nextor/bin/working/$1/*.com ::/system/
	mmove -i $(BINDIR)$1.fdd.dsk ::/system/command2.com ::/
	mcopy -i $(BINDIR)$1.fdd.dsk ../nextor/bin/working/$1/nextor.sys ::/
	[[ $$$$(ls -A ./extras) ]] && mcopy -i $(BINDIR)$1.fdd.dsk ./extras/* ::/
	# echo "Built disk image $(BINDIR)$1.fdd.dsk"

#Package driver and disk image
$(BINDIR)$1.driver-with-sectors.bin: $(BINDIR)rc2014dr.bin $(BINDIR)rcembdrv.bin $(BINDIR)$1.fdd.dsk
	@cd $(BINDIR)
	SECSTRT=$$$$(../scripts/getsymb.sh rcembdrv.exp.asm SECSTR)
	DATSIZ=$$$$(../scripts/getsymb.sh rcembdrv.exp.asm DATSIZ)
	dd status=none if=/dev/zero of=$1.driver-with-sectors.bin bs=16k count=19 seek=0
	dd status=none conv=notrunc if=rc2014dr.bin of=$1.driver-with-sectors.bin bs=8k count=2 seek=0
	BNK_START_ADDR=$$$$((SECSTRT-16384))
	for i in {1..18}
	do
		BNK_ADDR=$$$$(($$$$BNK_START_ADDR + (16384*($$$$i))))
		SKIP=$$$$(($$$$DATSIZ*($$$$i-1)))
		dd status=none conv=notrunc if=rcembdrv.bin of=$1.driver-with-sectors.bin bs=8k count=1 seek=$$$$((2*$$$$i))
		dd status=none conv=notrunc if=$1.fdd.dsk of=$1.driver-with-sectors.bin bs=1 count=$$$${DATSIZ} seek=$$$$BNK_ADDR skip=$$$$SKIP
	done
	# echo "Built image $(BINDIR)$1.driver-with-sectors.bin"

#Package rom with driver and disk image
$(BINDIR)$1.nextor.rom: $(BINDIR)$1.driver-with-sectors.bin $(BINDIR)chgbnk.bin
	@cd $(BINDIR)
	../../nextor/linuxtools/mknexrom ../../nextor/bin/working/$1/dos250ba.dat $1.nextor.rom -q -d:$1.driver-with-sectors.bin -m:chgbnk.bin

endef

$(eval $(call BUILD_versions,cbios))
$(eval $(call BUILD_versions,rc2014))

# include ./cexten/Makefile
include ./usbfdd/Makefile
include ./depends.d
