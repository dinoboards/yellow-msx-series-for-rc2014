
./bin/cpusptst/main.c.asm: cpusptst/main.c
./bin/dots.c.asm: dots.c config_request.h v9958.h libraries/msxdos/msxdos.h
./bin/extbio/main.c.asm: extbio/main.c libraries/fossil/fossil.h \
 libraries/msxdos/msxdos.h xstdio.h libraries/msxbios/extbio.h
./bin/lines.c.asm: lines.c config_request.h v9958.h libraries/msxdos/msxdos.h
./bin/esp8266/wget.c.asm: esp8266/wget.c esp8266/wget.h esp8266/arguments.h \
 esp8266/esp8266.h print.h xrecv2/utils.h libraries/fossil/fossil.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h \
 xrecv2/xmodem.h
./bin/esp8266/xmodem_diagnostics.c.asm: esp8266/xmodem_diagnostics.c esp8266/features.h \
 esp8266/../xrecv2/xmodem.c esp8266/../xrecv2/xmodem.h \
 esp8266/../xrecv2/crc16.h esp8266/../xrecv2/serial.h \
 libraries/fossil/fossil.h libraries/msxbios/system_vars.h
./bin/esp8266/arguments.c.asm: esp8266/arguments.c esp8266/arguments.h print.h
./bin/esp8266/msxhub.c.asm: esp8266/msxhub.c esp8266/msxhub.h esp8266/arguments.h \
 esp8266/esp8266.h print.h esp8266/wget.h xrecv2/xmodem.h
./bin/esp8266/esp8266.c.asm: esp8266/esp8266.c esp8266/features.h esp8266/arguments.h \
 esp8266/esp8266.h esp8266/msxhub.h print.h esp8266/wget.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/msxbios/extbio.h libraries/fossil/fossil.h \
 libraries/msxdos/msxdos.h
./bin/xrecv2/crc16.c.asm: xrecv2/crc16.c xrecv2/crc16.h
./bin/xrecv2/xmodem.c.asm: xrecv2/xmodem.c xrecv2/xmodem.h xrecv2/crc16.h \
 xrecv2/serial.h libraries/fossil/fossil.h \
 libraries/msxbios/system_vars.h
./bin/xrecv2/serial.c.asm: xrecv2/serial.c xrecv2/serial.h libraries/fossil/fossil.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
./bin/xrecv2/arguments.c.asm: xrecv2/arguments.c xrecv2/arguments.h print.h
./bin/xrecv2/utils.c.asm: xrecv2/utils.c xrecv2/utils.h
./bin/xrecv2/xrecv.c.asm: xrecv2/xrecv.c xrecv2/arguments.h libraries/msxdos/msxdos.h \
 print.h xrecv2/utils.h xrecv2/xmodem.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/msxbios/extbio.h \
 libraries/fossil/fossil.h
./bin/vramtest.c.asm: vramtest.c v9958.h libraries/msxdos/msxdos.h
./bin/helloworld.c.asm: helloworld.c libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
./bin/usbtst/printer_drv.c.asm: usbtst/printer_drv.c libraries/usb/class_printer.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h usbtst/print.h libraries/usb/usb_state.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
./bin/usbtst/usb-dev-info-ufi.c.asm: usbtst/usb-dev-info-ufi.c usbtst/usb-dev-info-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/transfers.h \
 libraries/usb/class_ufi.h libraries/usb/protocol.h \
 libraries/usb/usb_state.h usbtst/usb-dev-info.h \
 libraries/usb/usb_state.h
./bin/usbtst/enumerate_trace.c.asm: usbtst/enumerate_trace.c usbtst/nextor.h \
 usbtst/print.h libraries/usb/usb_state.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h libraries/usb/enumerate_trace.h
./bin/usbtst/work-area.c.asm: usbtst/work-area.c libraries/usb/usb_state.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h
./bin/usbtst/usb-lun-info-ufi.c.asm: usbtst/usb-lun-info-ufi.c usbtst/usb-lun-info-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/transfers.h \
 libraries/usb/class_ufi.h libraries/usb/protocol.h \
 libraries/usb/usb_state.h
./bin/usbtst/main.c.asm: usbtst/main.c usbtst/main.h libraries/usb/enumerate_trace.h \
 usbtst/print.h usbtst/printer_drv.h libraries/usb/usb_state.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h usbtst/usb-dev-info-ufi.h usbtst/nextor.h \
 libraries/usb/protocol.h usbtst/usb-lun-info-ufi.h libraries/usb/ch376.h \
 libraries/usb/class_printer.h libraries/usb/class_scsi.h \
 libraries/usb/enumerate.h libraries/usb/usb_state.h
./bin/fdisk/fdisk.c.asm: fdisk/fdisk.c fdisk/fdisk.h libraries/msxdos/msxdos.h \
 fdisk/fdisk2.h datatypes.h fdisk/partition.h \
 libraries/msxbios/system_vars.h
./bin/fdisk/fdisk2.c.asm: fdisk/fdisk2.c fdisk/fdisk.h libraries/msxdos/msxdos.h \
 fdisk/partition.h datatypes.h
./bin/v9958.c.asm: v9958.c v9958.h
./bin/term/xymodem.c.asm: term/xymodem.c term/xymodem.h term/aofossilhelper.h \
 libraries/fusion/msx_fusion.h term/print.h libraries/fusion/io.h
./bin/term/term.c.asm: term/term.c term/term.h libraries/fusion/msx_fusion.h \
 term/system-state.h term/ansi_codes.h term/ansiprint.h term/print.h \
 term/aofossilhelper.h term/arguments.h libraries/msxdos/msxdos.h \
 term/xymodem.h libraries/msxbios/extbio.h \
 libraries/msxbios/system_vars.h
./bin/term/arguments.c.asm: term/arguments.c term/arguments.h term/system-state.h
./bin/term/aofossilhelper.c.asm: term/aofossilhelper.c term/aofossilhelper.h \
 term/arguments.h libraries/fossil/fossil.h term/msx2ansi.h \
 libraries/fusion/msx_fusion.h term/print.h term/system-state.h
./bin/term/fossil_interface.c.asm: term/fossil_interface.c libraries/fossil/fossil.h \
 libraries/fusion/msx_fusion.h
./bin/term/ansiprint.c.asm: term/ansiprint.c term/ansiprint.h term/print.h \
 term/msx2ansi.h
./bin/term/system-state.c.asm: term/system-state.c term/system-state.h
./bin/print.c.asm: print.c print.h
./bin/rs232tst.c.asm: rs232tst.c libraries/msxbios/extbio.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
./bin/muflash/arguments.c.asm: muflash/arguments.c muflash/arguments.h print.h
./bin/muflash/main.c.asm: muflash/main.c muflash/arguments.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
./bin/libraries/fusion/width.c.asm: libraries/fusion/width.c libraries/fusion/msx_fusion.h
./bin/libraries/delay/delay.c.asm: libraries/delay/delay.c libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
./bin/libraries/usb/class_printer.c.asm: libraries/usb/class_printer.c \
 libraries/usb/class_printer.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h libraries/usb/enumerate_trace.h
./bin/libraries/usb/enumerate.c.asm: libraries/usb/enumerate.c libraries/usb/enumerate.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h libraries/usb/usb_state.h \
 libraries/usb/enumerate_hub.h libraries/usb/enumerate_storage.h \
 libraries/usb/enumerate_trace.h
./bin/libraries/usb/class_hub.c.asm: libraries/usb/class_hub.c libraries/usb/class_hub.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h libraries/usb/usb_state.h
./bin/libraries/usb/enumerate_storage.c.asm: libraries/usb/enumerate_storage.c \
 libraries/usb/enumerate_storage.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/transfers.h
./bin/libraries/usb/enumerate_hub.c.asm: libraries/usb/enumerate_hub.c \
 libraries/usb/enumerate_hub.h libraries/usb/enumerate.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h libraries/usb/usb_state.h \
 libraries/usb/class_hub.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/usb/enumerate_trace.h
./bin/libraries/usb/transfers.c.asm: libraries/usb/transfers.c libraries/usb/transfers.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
./bin/libraries/usb/class_ufi.c.asm: libraries/usb/class_ufi.c libraries/usb/class_ufi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/transfers.h libraries/usb/usb_state.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/usb/enumerate_trace.h
./bin/libraries/usb/ch376.c.asm: libraries/usb/ch376.c libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
./bin/libraries/usb/protocol.c.asm: libraries/usb/protocol.c libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/transfers.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
./bin/libraries/usb/class_scsi.c.asm: libraries/usb/class_scsi.c libraries/usb/class_scsi.h \
 libraries/usb/protocol.h libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/transfers.h libraries/usb/usb_state.h
./bin/libraries/msxbios/system_vars.c.asm: libraries/msxbios/system_vars.c \
 libraries/msxbios/system_vars.h
./bin/libraries/fossil/fossil.c.asm: libraries/fossil/fossil.c libraries/fossil/fossil.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
./bin/fossilon.c.asm: fossilon.c libraries/msxdos/msxdos.h xstdio.h \
 libraries/msxbios/extbio.h
./bin/fosiltst.c.asm: fosiltst.c libraries/fossil/fossil.h xstdio.h \
 libraries/msxbios/extbio.h libraries/msxdos/msxdos.h
./bin/siocfg/arguments.c.asm: siocfg/arguments.c siocfg/arguments.h
./bin/siocfg/siocfg.c.asm: siocfg/siocfg.c siocfg/arguments.h libraries/msxbios/extbio.h
./bin/mbrot.c.asm: mbrot.c config_request.h libraries/msxdos/msxdos.h v9958.h
./bin/rtccalb/rtccalb.c.asm: rtccalb/rtccalb.c print.h libraries/msxdos/msxdos.h
./bin/config_request.c.asm: config_request.c v9958.h
./bin/romflash/romflash.c.asm: romflash/romflash.c romflash/flashloader.h \
 romflash/memmapper.h libraries/msxbios/extbio.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
./bin/cpusptst/cpusptst.o: ./cpusptst/cpusptst.asm
./bin/xstdio.o: ./xstdio.asm
./bin/usbtst/print.o: ./usbtst/print.asm
./bin/romflashwriter/romflashwriter.o: ./romflashwriter/romflashwriter.asm
./bin/fdisk/fdisk.o: ./fdisk/fdisk.asm
./bin/term/print.o: ./term/print.asm
./bin/term/msx2ansi.o: ./term/msx2ansi.asm
./bin/term/screen.o: ./term/screen.asm
./bin/memmap-override.o: ./memmap-override.asm
./bin/muflash/slot_helpers.o: ./muflash/slot_helpers.asm
./bin/getslt.o: ./getslt.asm ./msx.inc
./bin/libraries/extbio/extbio.o: ./libraries/extbio/extbio.asm ./msx.inc
./bin/libraries/fusion/inputchar.o: ./libraries/fusion/inputchar.asm
./bin/libraries/fusion/io.o: ./libraries/fusion/io.asm
./bin/libraries/fusion/inputstring.o: ./libraries/fusion/inputstring.asm
./bin/libraries/fusion/inkey.o: ./libraries/fusion/inkey.asm
./bin/libraries/msxdos/msxdos.o: ./libraries/msxdos/msxdos.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_get_time.o: ./libraries/msxdos/msxdos_get_time.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_set_time.o: ./libraries/msxdos/msxdos_set_time.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_set_date.o: ./libraries/msxdos/msxdos_set_date.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/fossil/fossil.o: ./libraries/fossil/fossil.asm ./msx.inc
./bin/rtccalb/mesaure.o: ./rtccalb/mesaure.asm
./bin/v9958.o: ./v9958.asm ./msx.inc
./bin/romflash/flashloader.o: ./romflash/flashloader.asm 
./bin/romflash/memmapper.o: ./romflash/memmapper.asm
./bin/utils.o: ./utils.asm 
./bin/rs232.o: ./rs232.asm ./msx.inc
