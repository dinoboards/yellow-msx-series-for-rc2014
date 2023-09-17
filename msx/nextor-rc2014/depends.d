
./bin/usb-dev-write-scsi.c.asm: usbfdd/usb-dev-write-scsi.c \
 usbfdd/usb-dev-write-scsi.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/work-area.h usbfdd/class-scsi.h usbfdd/hw.h usbfdd/nextor.h
./bin/extended-bios.c.asm: usbfdd/extended-bios.c usbfdd/print.h
./bin/usb-enumerate-storage.c.asm: usbfdd/usb-enumerate-storage.c \
 usbfdd/usb-enumerate-storage.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/hw.h usbfdd/work-area.h usbfdd/usb-enumerate.h
./bin/usb-dev-read.c.asm: usbfdd/usb-dev-read.c usbfdd/nextor.h \
 usbfdd/usb-dev-read-scsi.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/work-area.h usbfdd/usb-dev-read-ufi.h usbfdd/usb-dev.h
./bin/usb-lun-info.c.asm: usbfdd/usb-lun-info.c usbfdd/nextor.h usbfdd/usb-dev.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/work-area.h \
 usbfdd/usb-lun-info-scsi.h usbfdd/usb-lun-info-ufi.h
./bin/usb-dev-write.c.asm: usbfdd/usb-dev-write.c usbfdd/nextor.h \
 usbfdd/usb-dev-write-scsi.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/work-area.h usbfdd/usb-dev-write-ufi.h usbfdd/class-ufi.h \
 usbfdd/hw.h usbfdd/usb-dev.h
./bin/usb-dev-info.c.asm: usbfdd/usb-dev-info.c usbfdd/class-ufi.h usbfdd/ch376.h \
 usbfdd/ch376inc.h usbfdd/hw.h usbfdd/work-area.h usbfdd/nextor.h \
 usbfdd/usb-dev-info-scsi.h usbfdd/usb-dev-info-ufi.h usbfdd/usb-dev.h
./bin/usb-dev-read-scsi.c.asm: usbfdd/usb-dev-read-scsi.c \
 usbfdd/usb-dev-read-scsi.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/work-area.h usbfdd/class-scsi.h usbfdd/hw.h usbfdd/nextor.h
./bin/usb-enumerate.c.asm: usbfdd/usb-enumerate.c usbfdd/usb-enumerate.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/hw.h usbfdd/work-area.h \
 usbfdd/debuggin.h usbfdd/class-hub.h usbfdd/class-ufi.h usbfdd/nextor.h \
 usbfdd/print.h usbfdd/usb-enumerate-storage.h usbfdd/usb-enumerate-hub.h
./bin/usb-dev-status-scsi.c.asm: usbfdd/usb-dev-status-scsi.c usbfdd/class-scsi.h \
 usbfdd/hw.h usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/nextor.h \
 usbfdd/work-area.h
./bin/class-ufi.c.asm: usbfdd/class-ufi.c usbfdd/class-ufi.h usbfdd/ch376.h \
 usbfdd/ch376inc.h usbfdd/hw.h usbfdd/work-area.h usbfdd/print.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h
./bin/usb-dev-status-ufi.c.asm: usbfdd/usb-dev-status-ufi.c usbfdd/class-ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/hw.h usbfdd/work-area.h \
 usbfdd/nextor.h
./bin/usb-enumerate-hub.c.asm: usbfdd/usb-enumerate-hub.c \
 usbfdd/usb-enumerate-hub.h usbfdd/usb-enumerate.h usbfdd/ch376.h \
 usbfdd/ch376inc.h usbfdd/hw.h usbfdd/work-area.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h \
 usbfdd/debuggin.h usbfdd/class-hub.h usbfdd/class-ufi.h usbfdd/nextor.h \
 usbfdd/print.h
./bin/usb-dev.c.asm: usbfdd/usb-dev.c usbfdd/usb-dev.h usbfdd/ch376.h \
 usbfdd/ch376inc.h usbfdd/work-area.h
./bin/class-hub.c.asm: usbfdd/class-hub.c usbfdd/class-hub.h usbfdd/ch376.h \
 usbfdd/ch376inc.h usbfdd/work-area.h
./bin/usb-dev-info-scsi.c.asm: usbfdd/usb-dev-info-scsi.c \
 usbfdd/usb-dev-info-scsi.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/nextor.h usbfdd/class-scsi.h usbfdd/hw.h usbfdd/usb-dev-info.h \
 usbfdd/work-area.h
./bin/ch376.c.asm: usbfdd/ch376.c usbfdd/ch376.h usbfdd/ch376inc.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h \
 usbfdd/print.h
./bin/class-scsi.c.asm: usbfdd/class-scsi.c usbfdd/class-scsi.h usbfdd/hw.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/print.h usbfdd/work-area.h
./bin/usb-lun-info-scsi.c.asm: usbfdd/usb-lun-info-scsi.c usbfdd/class-scsi.h \
 usbfdd/hw.h usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/usb-lun-info-ufi.h \
 usbfdd/nextor.h
./bin/printer.c.asm: usbfdd/printer.c usbfdd/printer.h usbfdd/ch376.h \
 usbfdd/ch376inc.h usbfdd/hw.h
./bin/usb-dev-info-ufi.c.asm: usbfdd/usb-dev-info-ufi.c usbfdd/usb-dev-info-ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/nextor.h usbfdd/class-ufi.h \
 usbfdd/hw.h usbfdd/work-area.h usbfdd/usb-dev-info.h
./bin/usb-dev-status.c.asm: usbfdd/usb-dev-status.c usbfdd/class-ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/hw.h usbfdd/work-area.h \
 usbfdd/nextor.h usbfdd/usb-dev-status-scsi.h usbfdd/usb-dev-status-ufi.h \
 usbfdd/usb-dev.h
./bin/usb-dev-write-ufi.c.asm: usbfdd/usb-dev-write-ufi.c \
 usbfdd/usb-dev-write-ufi.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/class-ufi.h usbfdd/hw.h usbfdd/work-area.h usbfdd/nextor.h
./bin/work-area.c.asm: usbfdd/work-area.c usbfdd/work-area.h usbfdd/ch376.h \
 usbfdd/ch376inc.h
./bin/debuggin.c.asm: usbfdd/debuggin.c usbfdd/debuggin.h usbfdd/class-hub.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/class-ufi.h usbfdd/hw.h \
 usbfdd/work-area.h usbfdd/nextor.h usbfdd/print.h
./bin/usb-lun-info-ufi.c.asm: usbfdd/usb-lun-info-ufi.c usbfdd/usb-lun-info-ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/nextor.h usbfdd/class-ufi.h \
 usbfdd/hw.h usbfdd/work-area.h
./bin/usb-dev-read-ufi.c.asm: usbfdd/usb-dev-read-ufi.c usbfdd/usb-dev-read-ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/work-area.h usbfdd/class-ufi.h \
 usbfdd/hw.h usbfdd/nextor.h
./bin/hw.c.asm: usbfdd/hw.c usbfdd/hw.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/work-area.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h usbfdd/debuggin.h \
 usbfdd/class-hub.h usbfdd/class-ufi.h usbfdd/nextor.h usbfdd/print.h
./bin/usb-init.c.asm: usbfdd/usb-init.c usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/class-scsi.h usbfdd/hw.h usbfdd/nextor.h \
 usbfdd/usb-enumerate-hub.h usbfdd/usb-enumerate.h usbfdd/work-area.h \
 usbfdd/usb-lun-info.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h usbfdd/debuggin.h \
 usbfdd/class-hub.h usbfdd/class-ufi.h usbfdd/print.h
./bin/crt.o: ./usbfdd/crt.asm
./bin/work-area.o: ./usbfdd/work-area.asm ./usbfdd/msx.inc
./bin/print.o: ./usbfdd/print.asm ./usbfdd/msx.inc
./bin/rc2014dr.inc.o: ./rc2014dr.inc.asm
./bin/rc2014dr.o: ./rc2014dr.asm
./bin/embinc.o: ./embinc.asm
./bin/rcembdrv.o: ./rcembdrv.asm
./bin/musicdriver.o: ./musicdriver.asm
./bin/scsi_helpers.o: ./scsi_helpers.asm 
./bin/drvend.o: ./drvend.asm
./bin/unapi-nextor-id.o: ./unapi-nextor-id.asm
./bin/ch376drv.o: ./ch376drv.asm
./bin/chgbnk.o: ./chgbnk.asm
./bin/version.o: ./version.asm
./bin/cfdrv.o: ./cfdrv.asm
