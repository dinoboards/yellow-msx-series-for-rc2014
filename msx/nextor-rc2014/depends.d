
./bin/usb-dev-read.c.asm: cexten/usb-dev-read.c cexten/ch376.h cexten/scsi.h \
 cexten/work-area.h cexten/print.h
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
./bin/version.o: ./version.asm
./bin/scsi_helpers.o: ./scsi_helpers.asm 
./bin/musicdriver.o: ./musicdriver.asm
./bin/rc2014dr.o: ./rc2014dr.asm    
./bin/embinc.o: ./embinc.asm
./bin/print.o: ./cexten/print.asm ./cexten/asm.inc
./bin/crt.o: ./cexten/crt.asm
./bin/work-area.o: ./cexten/work-area.asm ./cexten/asm.inc
