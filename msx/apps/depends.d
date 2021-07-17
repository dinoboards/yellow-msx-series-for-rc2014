
./bin/cpusptst/main.c.asm: cpusptst/main.c
./bin/dots.c.asm: dots.c msxdos.h v9958.h
./bin/extbio/main.c.asm: extbio/main.c extbio.h fossil.h msxdos.h xstdio.h
./bin/fusion/width.c.asm: fusion/width.c fusion/msx_fusion.h
./bin/fusion/msx.c.asm: fusion/msx.c fusion/msx.h
./bin/lines.c.asm: lines.c v9958.h
./bin/xrecv2/crc16.c.asm: xrecv2/crc16.c xrecv2/crc16.h
./bin/xrecv2/xmodem.c.asm: xrecv2/xmodem.c xrecv2/xmodem.h xrecv2/crc16.h \
 xrecv2/serial.h fossil.h
./bin/xrecv2/serial.c.asm: xrecv2/serial.c xrecv2/serial.h fossil.h fusion/msx.h
./bin/xrecv2/arguments.c.asm: xrecv2/arguments.c xrecv2/arguments.h xrecv2/print.h
./bin/xrecv2/print.c.asm: xrecv2/print.c xrecv2/print.h
./bin/xrecv2/xrecv.c.asm: xrecv2/xrecv.c xrecv2/arguments.h crt_override.h fossil.h \
 fusion/msx.h xrecv2/print.h xrecv2/xmodem.h
./bin/vramtest.c.asm: vramtest.c v9958.h
./bin/helloworld.c.asm: helloworld.c
./bin/fossil.c.asm: fossil.c extbio.h msxdos.h xstdio.h
./bin/xymodem.c.asm: xymodem.c xymodem.h aofossilhelper.h fusion/msx_fusion.h \
 print.h fusion/io.h
./bin/telnet/telnet.c.asm: telnet/telnet.c telnet/telnet.h telnet/asm.h \
 fusion/msx_fusion.h telnet/ansiprint.h print.h aofossilhelper.h \
 crt_override.h msxdos.h xymodem.h
./bin/telnet/fossil_interface.c.asm: telnet/fossil_interface.c fossil.h \
 fusion/msx_fusion.h
./bin/telnet/ansiprint.c.asm: telnet/ansiprint.c telnet/ansiprint.h print.h telnet/asm.h \
 telnet/msx2ansi.h
./bin/fdisk/fdisk.c.asm: fdisk/fdisk.c fdisk/fdisk.h msxdos.h fdisk/fdisk2.h \
 datatypes.h fdisk/partition.h
./bin/fdisk/fdisk2.c.asm: fdisk/fdisk2.c fdisk/fdisk.h msxdos.h fdisk/partition.h \
 datatypes.h
./bin/v9958.c.asm: v9958.c v9958.h
./bin/rs232tst.c.asm: rs232tst.c crt_override.h extbio.h fusion/msx.h msxdos.h
./bin/fosiltst.c.asm: fosiltst.c extbio.h fossil.h msxdos.h xstdio.h
./bin/aofossilhelper.c.asm: aofossilhelper.c aofossilhelper.h fossil.h \
 fusion/msx_fusion.h print.h
./bin/mbrot.c.asm: mbrot.c v9958.h
./bin/crt_override.c.asm: crt_override.c crt_override.h msxdos.h
./bin/msx.o: ./msx.asm ./msx.inc
./bin/cpusptst/cpusptst.o: ./cpusptst/cpusptst.asm
./bin/fusion/inputchar.o: ./fusion/inputchar.asm
./bin/fusion/io.o: ./fusion/io.asm
./bin/fusion/inputstring.o: ./fusion/inputstring.asm
./bin/fusion/inkey.o: ./fusion/inkey.asm
./bin/xstdio.o: ./xstdio.asm
./bin/msxdos.o: ./msxdos.asm
./bin/telnet/print.o: ./telnet/print.asm
./bin/telnet/msx2ansi.o: ./telnet/msx2ansi.asm
./bin/telnet/screen.o: ./telnet/screen.asm
./bin/fdisk/fdisk.o: ./fdisk/fdisk.asm
./bin/fdisk/memmap-override.o: ./fdisk/memmap-override.asm
./bin/fossil.o: ./fossil.asm ./msx.inc
./bin/getslt.o: ./getslt.asm ./msx.inc
./bin/extbio.o: ./extbio.asm ./msx.inc
./bin/v9958.o: ./v9958.asm ./msx.inc
./bin/utils.o: ./utils.asm 
./bin/rs232.o: ./rs232.asm ./msx.inc
