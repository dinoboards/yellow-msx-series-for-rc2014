
./bin/print.c.asm: cexten/print.c cexten/print.h
./bin/spike.c.asm: cexten/spike.c
./bin/debuggin.c.asm: cexten/debuggin.c cexten/debuggin.h cexten/print.h \
 cexten/work-area.h
./bin/usb-host-init.c.asm: cexten/usb-host-init.c cexten/debuggin.h \
 cexten/print.h cexten/work-area.h ../apps/libraries/delay/delay.h \
 ../apps/libraries/msxbios/system_vars.h
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
