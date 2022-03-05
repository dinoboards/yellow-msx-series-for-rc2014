# MSXUSB makefile
#
# Creates 3 variants of the Nextor driver for different hardware
# MSXUSBCARTv1 	- SCC mapper cartridge with ports 0x10 and 0x11
# ROOKIEDRIVE	- ASCII16 mapper cartridge with ports 0x20 and 0x21
# MISTERSPI		- CH376s module connected via SPI to MiSTerFPGA

ASM = sjasmplus
ASMFLAGS = --fullpath --raw=$@ --sym=$(basename $@).sym --lst=$(basename $@).lst -D__MSXUSBCARTv1
RDASMFLAGS = --fullpath --raw=$@ --sym=$(basename $@).sym --lst=$(basename $@).lst -D__ROOKIEDRIVE
MSTRASMFLAGS = --fullpath --raw=$@ --sym=$(basename $@).sym --lst=$(basename $@).lst -D__MISTERSPI #-D__NOWAIT
OMSTRASMFLAGS = --fullpath --raw=$@ --sym=$(basename $@).sym --lst=$(basename $@).lst -D__MISTERSPI -D__NOWAIT
OUTPUT_DIR = ../dist
KERNEL_DIR = ../kernel
RDDISK_DIR = ../ramdsk
OCM_ROM_DIR = ../../../../../fpga/OCM/sdbios/make/roms

all: $(OUTPUT_DIR)/chgbnk.bin $(OUTPUT_DIR)/chgbnkrd.bin $(OUTPUT_DIR)/nextor.rom $(OUTPUT_DIR)/nextorrd.rom $(OUTPUT_DIR)/nextormstr.rom $(OUTPUT_DIR)/nextoromstr.rom $(OUTPUT_DIR)/extra1k.dat $(OUTPUT_DIR)/OCM-BIOS.DAT $(OUTPUT_DIR)/msxusb.vhd

.PHONY: all

$(OUTPUT_DIR)/extra1k.dat: extra1k.asm
	$(ASM) $(ASMFLAGS) $<

# bank switching variants
$(OUTPUT_DIR)/chgbnk.bin: chgbnk.asm
	$(ASM) $(ASMFLAGS) $< 

$(OUTPUT_DIR)/chgbnkrd.bin: chgbnk.asm
	$(ASM) $(RDASMFLAGS) $< 

$(OUTPUT_DIR)/chgbnkmstr.bin: chgbnk.asm
	$(ASM) $(MSTRASMFLAGS) $< 

# nextor driver variants
$(OUTPUT_DIR)/drivermstr.rom: driver_low.asm driver_helpers_low.asm flashdisk.asm print_bios.asm ch376s.asm ch376s_helpers.asm usbhost.asm nextordirect.asm scsi.asm scsi_helpers.asm
	$(ASM) $(MSTRASMFLAGS) $<

$(OUTPUT_DIR)/driveromstr.rom: driver_low.asm driver_helpers_low.asm flashdisk.asm print_bios.asm ch376s.asm ch376s_helpers.asm usbhost.asm nextordirect.asm scsi.asm scsi_helpers.asm
	$(ASM) $(OMSTRASMFLAGS) $<

$(OUTPUT_DIR)/driverrd.rom: driver_low.asm driver_helpers_low.asm flashdisk.asm print_bios.asm ch376s.asm ch376s_helpers.asm usbhost.asm nextordirect.asm scsi.asm scsi_helpers.asm
	$(ASM) $(RDASMFLAGS) $<

$(OUTPUT_DIR)/driver.rom: driver_low.asm driver_helpers_low.asm flashdisk.asm print_bios.asm ch376s.asm ch376s_helpers.asm usbhost.asm nextordirect.asm scsi.asm scsi_helpers.asm
	$(ASM) $(ASMFLAGS) $<

	
# ROOKIEDRV HW on port 0x20 and 0x21 and ASCII16 bank switching
$(OUTPUT_DIR)/nextorrd.rom: $(OUTPUT_DIR)/driverrd.rom $(OUTPUT_DIR)/chgbnkrd.bin $(OUTPUT_DIR)/extra1k.dat
	$(KERNEL_DIR)/mknexrom $(KERNEL_DIR)/Nextor-2.1.0.base.dat $(OUTPUT_DIR)/nextorrd.rom \
							-d:$(OUTPUT_DIR)/driverrd.rom -m:$(OUTPUT_DIR)/chgbnkrd.bin \
							-e:$(OUTPUT_DIR)/extra1k.dat
	cat $(OUTPUT_DIR)/nextorrd.rom $(RDDISK_DIR)/RDDISK.DSK > $(OUTPUT_DIR)/nextorrd_complete.rom

	
clean:
	-rm $(OUTPUT_DIR)/*.rom
	-rm $(OUTPUT_DIR)/*.bin
	-rm $(OUTPUT_DIR)/*.sym
	-rm $(OUTPUT_DIR)/*.lst
	-rm $(OUTPUT_DIR)/*.dat
	-rm $(OUTPUT_DIR)/*.DAT
	-rm $(OUTPUT_DIR)/*.vhd

