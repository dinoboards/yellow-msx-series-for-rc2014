
./bin/usb-dev-read.c.asm: usbfdd/usb-dev-read.c usbfdd/nextor.h
./bin/ch376.c.asm: usbfdd/ch376.c usbfdd/ch376.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h usbfdd/print.h
./bin/usb-lun-info.c.asm: usbfdd/usb-lun-info.c usbfdd/nextor.h
./bin/ufi.c.asm: usbfdd/ufi.c usbfdd/ufi.h usbfdd/ch376.h usbfdd/usb.h \
 usbfdd/work-area.h usbfdd/hw.h usbfdd/print.h
./bin/usb-init.c.asm: usbfdd/usb-init.c usbfdd/ch376.h usbfdd/hw.h usbfdd/usb.h \
 usbfdd/work-area.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h usbfdd/debuggin.h usbfdd/ufi.h \
 usbfdd/print.h
./bin/print.c.asm: usbfdd/print.c usbfdd/print.h
./bin/usb-dev-write.c.asm: usbfdd/usb-dev-write.c usbfdd/nextor.h
./bin/hw.c.asm: usbfdd/hw.c usbfdd/hw.h usbfdd/ch376.h usbfdd/usb.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h \
 usbfdd/print.h
./bin/work-area.c.asm: usbfdd/work-area.c usbfdd/work-area.h usbfdd/ch376.h
./bin/usb-dev-info.c.asm: usbfdd/usb-dev-info.c usbfdd/nextor.h
./bin/usb-dev-status.c.asm: usbfdd/usb-dev-status.c
./bin/debuggin.c.asm: usbfdd/debuggin.c usbfdd/debuggin.h usbfdd/ufi.h \
 usbfdd/ch376.h usbfdd/usb.h usbfdd/work-area.h usbfdd/print.h
./bin/usb-dev-read.c.asm: cexten/usb-dev-read.c cexten/ch376.h cexten/scsi.h \
 cexten/work-area.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h cexten/print.h
./bin/usb-lun-info.c.asm: cexten/usb-lun-info.c cexten/ch376.h cexten/print.h \
 cexten/scsi.h cexten/work-area.h
./bin/print.c.asm: cexten/print.c cexten/print.h
./bin/scsi.c.asm: cexten/scsi.c cexten/scsi.h cexten/work-area.h
./bin/usb-dev-write.c.asm: cexten/usb-dev-write.c cexten/ch376.h cexten/scsi.h \
 cexten/work-area.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h cexten/print.h
./bin/spike.c.asm: cexten/spike.c
./bin/usb-dev-info.c.asm: cexten/usb-dev-info.c cexten/work-area.h
./bin/usb-dev-status.c.asm: cexten/usb-dev-status.c cexten/work-area.h
./bin/debuggin.c.asm: cexten/debuggin.c cexten/debuggin.h cexten/print.h \
 cexten/work-area.h
./bin/usb-host-init.c.asm: cexten/usb-host-init.c cexten/ch376.h \
 cexten/debuggin.h cexten/print.h cexten/work-area.h cexten/scsi.h \
 ../apps/libraries/delay/delay.h ../apps/libraries/msxbios/system_vars.h
./bin/chgbnk.o: ./chgbnk.asm
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
./bin/print.o: ./cexten/print.asm ./cexten/asm.inc
./bin/crt.o: ./cexten/crt.asm
./bin/work-area.o: ./cexten/work-area.asm ./cexten/asm.inc
