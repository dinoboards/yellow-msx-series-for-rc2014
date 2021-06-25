
./bin/cpusptst/main.c.asm: cpusptst/main.c
./bin/dots.c.asm: dots.c msxdos.h v9958.h
./bin/extbio/main.c.asm: extbio/main.c extbio.h msxdos.h telnet/fossil_interface.h \
 xstdio.h
./bin/lines.c.asm: lines.c v9958.h
./bin/vramtest.c.asm: vramtest.c v9958.h
./bin/telnet/fake_connection.c.asm: telnet/fake_connection.c telnet/aofossilhelper.h \
 telnet/fossil_interface.h telnet/msx_fusion.h telnet/print.h
./bin/telnet/telnet.c.asm: telnet/telnet.c telnet/ansi_codes.h telnet/msx2ansi.h \
 telnet/msx_fusion.h telnet/print.h
./bin/telnet/width.c.asm: telnet/width.c telnet/msx_fusion.h
./bin/telnet/xymodem.c.asm: telnet/xymodem.c telnet/xymodem.h telnet/aofossilhelper.h \
 telnet/io.h telnet/msx_fusion.h telnet/print.h
./bin/telnet/print.c.asm: telnet/print.c telnet/print.h telnet/asm.h telnet/msx2ansi.h
./bin/telnet/aofossilhelper.c.asm: telnet/aofossilhelper.c telnet/aofossilhelper.h \
 telnet/fossil_interface.h telnet/msx_fusion.h telnet/print.h
./bin/telnet/fossil_interface.c.asm: telnet/fossil_interface.c telnet/fossil_interface.h \
 telnet/msx_fusion.h
./bin/telnet/telnet.old.c.asm: telnet/telnet.old.c telnet/telnet.old.h telnet/asm.h \
 telnet/msx_fusion.h telnet/aofossilhelper.h telnet/print.h \
 telnet/xymodem.h
./bin/fdisk/fdisk.c.asm: fdisk/fdisk.c fdisk/fdisk.h msxdos.h fdisk/fdisk2.h \
 datatypes.h fdisk/partition.h
./bin/fdisk/fdisk2.c.asm: fdisk/fdisk2.c fdisk/fdisk.h msxdos.h fdisk/partition.h \
 datatypes.h
./bin/v9958.c.asm: v9958.c v9958.h
./bin/rs232tst.c.asm: rs232tst.c extbio.h msxdos.h
./bin/fosiltst.c.asm: fosiltst.c extbio.h msxdos.h xstdio.h
./bin/mbrot.c.asm: mbrot.c v9958.h
./bin/msx.o: ./msx.asm ./msx.inc
./bin/cpusptst/cpusptst.o: ./cpusptst/cpusptst.asm
./bin/xstdio.o: ./xstdio.asm
./bin/msxdos.o: ./msxdos.asm
./bin/telnet/msx2ansibuffer.o: ./telnet/msx2ansibuffer.asm
./bin/telnet/inputchar.o: ./telnet/inputchar.asm
./bin/telnet/io.o: ./telnet/io.asm
./bin/telnet/inputstring.o: ./telnet/inputstring.asm
./bin/telnet/inkey.o: ./telnet/inkey.asm
./bin/telnet/print.o: ./telnet/print.asm
./bin/telnet/fossil_interface.o: ./telnet/fossil_interface.asm
./bin/telnet/screen.o: ./telnet/screen.asm
./bin/fdisk/fdisk.o: ./fdisk/fdisk.asm
./bin/fdisk/memmap-override.o: ./fdisk/memmap-override.asm
./bin/fossil.o: ./fossil.asm ./msx.inc
./bin/extbio.o: ./extbio.asm ./msx.inc
./bin/v9958.o: ./v9958.asm ./msx.inc
./bin/utils.o: ./utils.asm 
./bin/rs232.o: ./rs232.asm ./msx.inc
