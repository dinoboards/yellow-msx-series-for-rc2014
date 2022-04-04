
./bin/usb-enumerate-hub.c.asm: usbfdd/usb-enumerate-hub.c \
 usbfdd/usb-enumerate-hub.h usbfdd/usb.h usbfdd/work-area.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/hw.h usbfdd/usb-enumerate.h \
 usbfdd/print.h
./bin/usb-dev-read.c.asm: usbfdd/usb-dev-read.c usbfdd/nextor.h usbfdd/ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/usb.h usbfdd/work-area.h
./bin/ch376.c.asm: usbfdd/ch376.c usbfdd/ch376.h usbfdd/ch376inc.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h \
 usbfdd/print.h
./bin/usb-lun-info.c.asm: usbfdd/usb-lun-info.c usbfdd/nextor.h usbfdd/ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/usb.h usbfdd/work-area.h
./bin/ufi.c.asm: usbfdd/ufi.c usbfdd/ufi.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/usb.h usbfdd/work-area.h usbfdd/hw.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h \
 usbfdd/print.h
./bin/usb-init.c.asm: usbfdd/usb-init.c usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/hw.h usbfdd/usb.h usbfdd/nextor.h usbfdd/usb-enumerate-hub.h \
 usbfdd/work-area.h usbfdd/usb-enumerate.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h \
 usbfdd/debuggin.h usbfdd/ufi.h usbfdd/print.h
./bin/print.c.asm: usbfdd/print.c usbfdd/print.h
./bin/usb-enumerate.c.asm: usbfdd/usb-enumerate.c usbfdd/usb-enumerate.h \
 usbfdd/usb.h usbfdd/work-area.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/hw.h usbfdd/usb-enumerate-floppy.h usbfdd/usb-enumerate-hub.h \
 usbfdd/debuggin.h usbfdd/nextor.h usbfdd/ufi.h usbfdd/print.h
./bin/usb-dev-write.c.asm: usbfdd/usb-dev-write.c usbfdd/nextor.h usbfdd/ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/usb.h usbfdd/work-area.h
./bin/hw.c.asm: usbfdd/hw.c usbfdd/hw.h usbfdd/ch376.h usbfdd/ch376inc.h \
 usbfdd/usb.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h usbfdd/print.h
./bin/work-area.c.asm: usbfdd/work-area.c usbfdd/work-area.h usbfdd/ch376.h \
 usbfdd/ch376inc.h
./bin/usb-dev-info.c.asm: usbfdd/usb-dev-info.c usbfdd/nextor.h usbfdd/ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/usb.h usbfdd/work-area.h
./bin/usb-dev-status.c.asm: usbfdd/usb-dev-status.c usbfdd/nextor.h usbfdd/ufi.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/usb.h usbfdd/work-area.h
./bin/debuggin.c.asm: usbfdd/debuggin.c usbfdd/debuggin.h usbfdd/nextor.h \
 usbfdd/ufi.h usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/usb.h \
 usbfdd/work-area.h usbfdd/print.h
./bin/usb-enumerate-floppy.c.asm: usbfdd/usb-enumerate-floppy.c \
 usbfdd/usb-enumerate-floppy.h usbfdd/usb.h usbfdd/work-area.h \
 usbfdd/ch376.h usbfdd/ch376inc.h usbfdd/hw.h usbfdd/usb-enumerate.h
./bin/chgbnk.o: ./chgbnk.asm
./bin/rc2014dr.inc.o: ./rc2014dr.inc.asm
./bin/rcembdrv.o: ./rcembdrv.asm
./bin/cfdrv.o: ./cfdrv.asm
./bin/drvend.o: ./drvend.asm
./bin/ch376drv.o: ./ch376drv.asm
./bin/print.o: ./usbfdd/print.asm ./usbfdd/msx.inc
./bin/crt.o: ./usbfdd/crt.asm
./bin/work-area.o: ./usbfdd/work-area.asm ./usbfdd/msx.inc
./bin/version.o: ./version.asm
./bin/scsi_helpers.o: ./scsi_helpers.asm 
./bin/musicdriver.o: ./musicdriver.asm
./bin/rc2014dr.o: ./rc2014dr.asm
./bin/embinc.o: ./embinc.asm
