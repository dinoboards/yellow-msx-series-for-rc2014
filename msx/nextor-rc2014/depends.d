
./bin/usb-dev-write-scsi.c.asm: usbdrv/usb-dev-write-scsi.c \
 usbdrv/usb-dev-write-scsi.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h usbdrv/work-area.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h \
 usbdrv/nextor.h ../apps/libraries/usb/class_scsi.h
./bin/extended-bios.c.asm: usbdrv/extended-bios.c usbdrv/print.h
./bin/exports.c.asm: usbdrv/exports.c ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/transfers.h
./bin/usb-dev-read.c.asm: usbdrv/usb-dev-read.c usbdrv/nextor.h \
 usbdrv/usb-dev-read-scsi.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h usbdrv/work-area.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h \
 usbdrv/usb-dev-read-ufi.h usbdrv/usb-dev.h
./bin/usb-lun-info.c.asm: usbdrv/usb-lun-info.c usbdrv/nextor.h usbdrv/usb-dev.h \
 usbdrv/usb-lun-info-scsi.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/transfers.h \
 usbdrv/usb-lun-info-ufi.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/dev_transfers.h usbdrv/work-area.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h
./bin/usb-dev-write.c.asm: usbdrv/usb-dev-write.c usbdrv/nextor.h \
 usbdrv/usb-dev-write-scsi.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h usbdrv/work-area.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h \
 usbdrv/usb-dev-write-ufi.h ../apps/libraries/usb/class_ufi.h \
 ../apps/libraries/usb/usb_cbi.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/usb_state.h usbdrv/usb-dev.h
./bin/usb-dev-info.c.asm: usbdrv/usb-dev-info.c usbdrv/nextor.h \
 usbdrv/usb-dev-info-scsi.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/ch376.h usbdrv/usb-dev-info-ufi.h usbdrv/usb-dev.h \
 usbdrv/work-area.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/class_ufi.h \
 ../apps/libraries/usb/usb_cbi.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/usb_state.h
./bin/keyboard_drv.c.asm: usbdrv/keyboard_drv.c usbdrv/keyboard_drv.h \
 ../apps/libraries/usb/class_hid.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/class_hid_keyboard.h \
 ../apps/libraries/msxbios/system_vars.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/z80.h
./bin/usb-dev-read-scsi.c.asm: usbdrv/usb-dev-read-scsi.c \
 usbdrv/usb-dev-read-scsi.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h usbdrv/work-area.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h \
 usbdrv/nextor.h ../apps/libraries/usb/class_scsi.h
./bin/usb-dev-status-scsi.c.asm: usbdrv/usb-dev-status-scsi.c usbdrv/nextor.h \
 usbdrv/work-area.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/transfers.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/class_scsi.h
./bin/usb-dev-status-ufi.c.asm: usbdrv/usb-dev-status-ufi.c usbdrv/nextor.h \
 usbdrv/work-area.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/transfers.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/class_ufi.h \
 ../apps/libraries/usb/usb_cbi.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/usb_state.h
./bin/usb-dev.c.asm: usbdrv/usb-dev.c usbdrv/usb-dev.h \
 usbdrv/../../apps/libraries/msxdos/msxdos_rc2014_ex.h \
 usbdrv/keyboard_drv.h usbdrv/work-area.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/transfers.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/protocol.h
./bin/usb-dev-info-scsi.c.asm: usbdrv/usb-dev-info-scsi.c \
 usbdrv/usb-dev-info-scsi.h usbdrv/nextor.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/transfers.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/class_scsi.h ../apps/libraries/usb/protocol.h \
 usbdrv/usb-dev-info.h usbdrv/work-area.h \
 ../apps/libraries/usb/usb_state.h
./bin/usb-lun-info-scsi.c.asm: usbdrv/usb-lun-info-scsi.c usbdrv/usb-lun-info-ufi.h \
 usbdrv/nextor.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/class_scsi.h \
 ../apps/libraries/usb/protocol.h
./bin/usb-dev-info-ufi.c.asm: usbdrv/usb-dev-info-ufi.c usbdrv/usb-dev-info-ufi.h \
 usbdrv/nextor.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/ch376.h usbdrv/usb-dev-info.h usbdrv/work-area.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/class_ufi.h ../apps/libraries/usb/usb_cbi.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/usb_state.h
./bin/usb-dev-status.c.asm: usbdrv/usb-dev-status.c usbdrv/nextor.h \
 usbdrv/usb-dev-status-scsi.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/transfers.h usbdrv/usb-dev-status-ufi.h \
 usbdrv/usb-dev.h usbdrv/work-area.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/class_ufi.h \
 ../apps/libraries/usb/usb_cbi.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/usb_state.h
./bin/usb-dev-write-ufi.c.asm: usbdrv/usb-dev-write-ufi.c \
 usbdrv/usb-dev-write-ufi.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/ch376inc.h ../apps/libraries/usb/class_ufi.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/usb_cbi.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/usb_state.h usbdrv/nextor.h usbdrv/work-area.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/usb_state.h
./bin/work-area.c.asm: usbdrv/work-area.c usbdrv/work-area.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h
./bin/usb-lun-info-ufi.c.asm: usbdrv/usb-lun-info-ufi.c usbdrv/usb-lun-info-ufi.h \
 usbdrv/nextor.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/class_ufi.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/usb_cbi.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/format_mapping.h
./bin/usb-dev-read-ufi.c.asm: usbdrv/usb-dev-read-ufi.c usbdrv/usb-dev-read-ufi.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 usbdrv/work-area.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/dev_transfers.h \
 ../apps/libraries/usb/transfers.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/protocol.h usbdrv/nextor.h \
 ../apps/libraries/usb/class_ufi.h ../apps/libraries/usb/usb_cbi.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/usb_state.h
./bin/usb-init.c.asm: usbdrv/usb-init.c usbdrv/nextor.h usbdrv/print.h \
 usbdrv/usb-dev.h usbdrv/usb-lun-info.h usbdrv/work-area.h \
 ../apps/libraries/usb/ch376.h ../apps/libraries/usb/ch376inc.h \
 ../apps/libraries/usb/protocol.h ../apps/libraries/usb/ch376.h \
 ../apps/libraries/usb/dev_transfers.h ../apps/libraries/usb/transfers.h \
 ../apps/libraries/usb/usb_state.h ../apps/libraries/usb/protocol.h \
 ../apps/libraries/usb/class_scsi.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h \
 ../apps/libraries/usb/enumerate.h ../apps/libraries/usb/usb_state.h \
 ../apps/libraries/usb/enumerate_hub.h ../apps/libraries/usb/enumerate.h
./bin/rc2014dr.inc.o: ./rc2014dr.inc.asm
./bin/rc2014dr.o: ./rc2014dr.asm
./bin/embinc.o: ./embinc.asm
./bin/rcembdrv.o: ./rcembdrv.asm
./bin/musicdriver.o: ./musicdriver.asm
./bin/crt.o: ./usbdrv/crt.asm
./bin/work-area.o: ./usbdrv/work-area.asm ./usbdrv/msx.inc
./bin/print.o: ./usbdrv/print.asm ./usbdrv/msx.inc
./bin/usb-dev.o: ./usbdrv/usb-dev.asm
./bin/drvend.o: ./drvend.asm
./bin/unapi-nextor-id.o: ./unapi-nextor-id.asm
./bin/ch376drv.o: ./ch376drv.asm
./bin/chgbnk.o: ./chgbnk.asm
./bin/version.o: ./version.asm
./bin/cfdrv.o: ./cfdrv.asm
