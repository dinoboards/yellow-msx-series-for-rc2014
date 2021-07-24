
./bin/cpusptst/main.c.asm: cpusptst/main.c
./bin/dots.c.asm: dots.c v9958.h libraries/msxdos/msxdos.h
./bin/extbio/main.c.asm: extbio/main.c fossil.h libraries/msxdos/msxdos.h xstdio.h \
 libraries/msxbios/extbio.h
./bin/lines.c.asm: lines.c v9958.h
./bin/xrecv2/crc16.c.asm: xrecv2/crc16.c xrecv2/crc16.h
./bin/xrecv2/xmodem.c.asm: xrecv2/xmodem.c xrecv2/xmodem.h xrecv2/memory.h \
 xrecv2/crc16.h xrecv2/serial.h fossil.h libraries/msxbios/system_vars.h
./bin/xrecv2/serial.c.asm: xrecv2/serial.c xrecv2/serial.h fossil.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
./bin/xrecv2/arguments.c.asm: xrecv2/arguments.c xrecv2/arguments.h xrecv2/print.h
./bin/xrecv2/print.c.asm: xrecv2/print.c xrecv2/print.h
./bin/xrecv2/utils.c.asm: xrecv2/utils.c xrecv2/utils.h xrecv2/memory.h
./bin/xrecv2/xrecv.c.asm: xrecv2/xrecv.c xrecv2/arguments.h fossil.h \
 libraries/msxdos/msxdos.h xrecv2/print.h xrecv2/utils.h xrecv2/memory.h \
 xrecv2/xmodem.h libraries/msxbios/extbio.h
./bin/vramtest.c.asm: vramtest.c v9958.h
./bin/helloworld.c.asm: helloworld.c libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
./bin/fossil.c.asm: fossil.c libraries/msxdos/msxdos.h xstdio.h \
 libraries/msxbios/extbio.h
./bin/xymodem.c.asm: xymodem.c xymodem.h aofossilhelper.h \
 libraries/fusion/msx_fusion.h print.h libraries/fusion/io.h
./bin/telnet/telnet.c.asm: telnet/telnet.c telnet/telnet.h libraries/fusion/msx_fusion.h \
 telnet/ansiprint.h print.h aofossilhelper.h libraries/msxdos/msxdos.h \
 xymodem.h
./bin/telnet/fossil_interface.c.asm: telnet/fossil_interface.c fossil.h \
 libraries/fusion/msx_fusion.h
./bin/telnet/ansiprint.c.asm: telnet/ansiprint.c telnet/ansiprint.h print.h \
 telnet/msx2ansi.h
./bin/fdisk/fdisk.c.asm: fdisk/fdisk.c fdisk/fdisk.h libraries/msxdos/msxdos.h \
 fdisk/fdisk2.h datatypes.h fdisk/partition.h \
 libraries/msxbios/system_vars.h
./bin/fdisk/fdisk2.c.asm: fdisk/fdisk2.c fdisk/fdisk.h libraries/msxdos/msxdos.h \
 fdisk/partition.h datatypes.h
./bin/v9958.c.asm: v9958.c v9958.h
./bin/rs232tst.c.asm: rs232tst.c libraries/msxbios/extbio.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
./bin/libraries/fusion/width.c.asm: libraries/fusion/width.c libraries/fusion/msx_fusion.h
./bin/libraries/msxbios/system_vars.c.asm: libraries/msxbios/system_vars.c \
 libraries/msxbios/system_vars.h
./bin/fosiltst.c.asm: fosiltst.c fossil.h xstdio.h libraries/msxbios/extbio.h \
 libraries/msxdos/msxdos.h
./bin/aofossilhelper.c.asm: aofossilhelper.c aofossilhelper.h fossil.h \
 libraries/fusion/msx_fusion.h print.h
./bin/mbrot.c.asm: mbrot.c v9958.h
./bin/msx.o: ./msx.asm ./msx.inc
./bin/cpusptst/cpusptst.o: ./cpusptst/cpusptst.asm
./bin/xstdio.o: ./xstdio.asm
./bin/telnet/print.o: ./telnet/print.asm
./bin/telnet/msx2ansi.o: ./telnet/msx2ansi.asm
./bin/telnet/screen.o: ./telnet/screen.asm
./bin/fdisk/fdisk.o: ./fdisk/fdisk.asm
./bin/fossil.o: ./fossil.asm ./msx.inc
./bin/memmap-override.o: ./memmap-override.asm
./bin/getslt.o: ./getslt.asm ./msx.inc
./bin/libraries/fusion/inputchar.o: ./libraries/fusion/inputchar.asm
./bin/libraries/fusion/io.o: ./libraries/fusion/io.asm
./bin/libraries/fusion/inputstring.o: ./libraries/fusion/inputstring.asm
./bin/libraries/fusion/inkey.o: ./libraries/fusion/inkey.asm
./bin/libraries/msxdos/msxdos.o: ./libraries/msxdos/msxdos.asm
./bin/extbio.o: ./extbio.asm ./msx.inc
./bin/v9958.o: ./v9958.asm ./msx.inc
./bin/utils.o: ./utils.asm 
./bin/rs232.o: ./rs232.asm ./msx.inc
