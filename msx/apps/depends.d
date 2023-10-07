
main.o: cpusptst/main.c
dots.o: dots.c config_request.h v9958.h libraries/msxdos/msxdos.h
main.o: extbio/main.c libraries/fossil/fossil.h libraries/msxdos/msxdos.h \
 xstdio.h libraries/msxbios/extbio.h
lines.o: lines.c config_request.h v9958.h libraries/msxdos/msxdos.h
wget.o: esp8266/wget.c esp8266/wget.h esp8266/arguments.h \
 esp8266/esp8266.h print.h xrecv2/utils.h libraries/fossil/fossil.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h \
 xrecv2/xmodem.h
xmodem_diagnostics.o: esp8266/xmodem_diagnostics.c esp8266/features.h \
 esp8266/../xrecv2/xmodem.c esp8266/../xrecv2/xmodem.h \
 esp8266/../xrecv2/crc16.h esp8266/../xrecv2/serial.h \
 libraries/fossil/fossil.h libraries/msxbios/system_vars.h
arguments.o: esp8266/arguments.c esp8266/arguments.h print.h
msxhub.o: esp8266/msxhub.c esp8266/msxhub.h esp8266/arguments.h \
 esp8266/esp8266.h print.h esp8266/wget.h xrecv2/xmodem.h
esp8266.o: esp8266/esp8266.c esp8266/features.h esp8266/arguments.h \
 esp8266/esp8266.h esp8266/msxhub.h print.h esp8266/wget.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/msxbios/extbio.h libraries/fossil/fossil.h \
 libraries/msxdos/msxdos.h
crc16.o: xrecv2/crc16.c xrecv2/crc16.h
xmodem.o: xrecv2/xmodem.c xrecv2/xmodem.h xrecv2/crc16.h xrecv2/serial.h \
 libraries/fossil/fossil.h libraries/msxbios/system_vars.h
serial.o: xrecv2/serial.c xrecv2/serial.h libraries/fossil/fossil.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
arguments.o: xrecv2/arguments.c xrecv2/arguments.h print.h
utils.o: xrecv2/utils.c xrecv2/utils.h
xrecv.o: xrecv2/xrecv.c xrecv2/arguments.h libraries/msxdos/msxdos.h \
 print.h xrecv2/utils.h xrecv2/xmodem.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/msxbios/extbio.h \
 libraries/fossil/fossil.h
arguments.o: clrdir/arguments.c clrdir/arguments.h
fdisk2.o: clrdir/fdisk2.c clrdir/fdisk2.h datatypes.h \
 libraries/msxdos/partition.h libraries/msxdos/fat.h \
 libraries/msxdos/msxdos.h
clrdir.o: clrdir/clrdir.c clrdir/arguments.h clrdir/fdisk2.h datatypes.h \
 libraries/msxdos/partition.h libraries/msxdos/fat.h \
 libraries/fusion/io.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
vramtest.o: vramtest.c v9958.h libraries/msxdos/msxdos.h
reporters.o: usbctrl/reporters.c usbctrl/reporters.h \
 libraries/usb/protocol.h libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/fusion/io.h
usbctrl.o: usbctrl/usbctrl.c usbctrl/arguments.h usbctrl/reporters.h \
 libraries/usb/protocol.h libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/msxbios/extbio.h libraries/msxbios/extbio_rc2014.h \
 libraries/fusion/io.h
arguments.o: usbctrl/arguments.c usbctrl/arguments.h
helloworld.o: helloworld.c libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
printer_drv.o: usbtst/printer_drv.c libraries/usb/class_printer.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h usbtst/print.h \
 libraries/usb/usb_state.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
usb-dev-info-ufi.o: usbtst/usb-dev-info-ufi.c usbtst/usb-dev-info-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/class_ufi.h \
 libraries/usb/protocol.h libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h \
 usbtst/usb-dev-info.h libraries/usb/usb_state.h
enumerate_trace.o: usbtst/enumerate_trace.c libraries/usb/class_ufi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h usbtst/nextor.h \
 usbtst/print.h libraries/usb/usb_state.h libraries/usb/enumerate_trace.h
usb-dev-write-ufi.o: usbtst/usb-dev-write-ufi.c \
 usbtst/usb-dev-write-ufi.h usbtst/nextor.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/class_ufi.h libraries/usb/protocol.h \
 libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/usb/enumerate_trace.h
work-area.o: usbtst/work-area.c libraries/usb/usb_state.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h
usb-lun-info-ufi.o: usbtst/usb-lun-info-ufi.c usbtst/usb-lun-info-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/class_ufi.h \
 libraries/usb/protocol.h libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h
usb-dev-read-ufi.o: usbtst/usb-dev-read-ufi.c usbtst/usb-dev-read-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/class_ufi.h \
 libraries/usb/protocol.h libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h
main.o: usbtst/main.c usbtst/main.h libraries/usb/enumerate_trace.h \
 usbtst/print.h usbtst/printer_drv.h libraries/usb/usb_state.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 usbtst/usb-dev-info-ufi.h usbtst/nextor.h libraries/usb/protocol.h \
 usbtst/usb-lun-info-ufi.h libraries/usb/ch376.h \
 libraries/usb/class_printer.h libraries/usb/class_scsi.h \
 libraries/usb/enumerate.h libraries/usb/usb_state.h \
 libraries/usb/class_ufi.h libraries/usb/usb_cbi.h \
 libraries/usb/dev_transfers.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h usbtst/usb-dev-read-ufi.h \
 usbtst/usb-dev-write-ufi.h
fdisk.o: fdisk/fdisk.c fdisk/fdisk.h libraries/msxdos/msxdos.h \
 fdisk/fdisk2.h datatypes.h libraries/msxdos/partition.h \
 libraries/msxbios/system_vars.h
fdisk2.o: fdisk/fdisk2.c fdisk/fdisk2.h datatypes.h \
 libraries/msxdos/partition.h fdisk/fdisk.h libraries/msxdos/msxdos.h \
 libraries/msxdos/fat.h
v9958.o: v9958.c v9958.h
xymodem.o: term/xymodem.c term/xymodem.h term/aofossilhelper.h \
 libraries/fusion/msx_fusion.h term/print.h libraries/fusion/io.h
term.o: term/term.c term/term.h libraries/fusion/msx_fusion.h \
 term/system-state.h term/ansi_codes.h term/ansiprint.h term/print.h \
 term/aofossilhelper.h term/arguments.h libraries/msxdos/msxdos.h \
 term/xymodem.h libraries/msxbios/extbio.h \
 libraries/msxbios/system_vars.h
arguments.o: term/arguments.c term/arguments.h term/system-state.h
aofossilhelper.o: term/aofossilhelper.c term/aofossilhelper.h \
 term/arguments.h libraries/fossil/fossil.h term/msx2ansi.h \
 libraries/fusion/msx_fusion.h term/print.h term/system-state.h
fossil_interface.o: term/fossil_interface.c libraries/fossil/fossil.h \
 libraries/fusion/msx_fusion.h
ansiprint.o: term/ansiprint.c term/ansiprint.h term/print.h \
 term/msx2ansi.h
system-state.o: term/system-state.c term/system-state.h
print.o: print.c print.h
rs232tst.o: rs232tst.c libraries/msxbios/extbio.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
arguments.o: muflash/arguments.c muflash/arguments.h print.h
main.o: muflash/main.c muflash/arguments.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
width.o: libraries/fusion/width.c libraries/fusion/msx_fusion.h
delay.o: libraries/delay/delay.c libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
dev_transfers.o: libraries/usb/dev_transfers.c \
 libraries/usb/dev_transfers.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/transfers.h \
 libraries/usb/protocol.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
class_printer.o: libraries/usb/class_printer.c \
 libraries/usb/class_printer.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/enumerate_trace.h
usb_cbi.o: libraries/usb/usb_cbi.c libraries/usb/usb_cbi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/ch376.h \
 libraries/usb/transfers.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h
enumerate.o: libraries/usb/enumerate.c libraries/usb/enumerate.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h libraries/usb/enumerate_hub.h \
 libraries/usb/enumerate_storage.h libraries/usb/enumerate_trace.h
class_hub.o: libraries/usb/class_hub.c libraries/usb/class_hub.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h
enumerate_storage.o: libraries/usb/enumerate_storage.c \
 libraries/usb/enumerate_storage.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h
enumerate_hub.o: libraries/usb/enumerate_hub.c \
 libraries/usb/enumerate_hub.h libraries/usb/enumerate.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h libraries/usb/class_hub.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/usb/enumerate_trace.h
transfers.o: libraries/usb/transfers.c libraries/usb/transfers.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
class_ufi.o: libraries/usb/class_ufi.c libraries/usb/class_ufi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/usb/enumerate_trace.h
ch376.o: libraries/usb/ch376.c libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
protocol.o: libraries/usb/protocol.c libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
class_scsi.o: libraries/usb/class_scsi.c libraries/usb/class_scsi.h \
 libraries/usb/protocol.h libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h libraries/msxbios/system_vars.h
system_vars.o: libraries/msxbios/system_vars.c \
 libraries/msxbios/system_vars.h
fossil.o: libraries/fossil/fossil.c libraries/fossil/fossil.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
fossilon.o: fossilon.c libraries/msxdos/msxdos.h xstdio.h \
 libraries/msxbios/extbio.h
fosiltst.o: fosiltst.c libraries/fossil/fossil.h xstdio.h \
 libraries/msxbios/extbio.h libraries/msxdos/msxdos.h
arguments.o: siocfg/arguments.c siocfg/arguments.h
siocfg.o: siocfg/siocfg.c siocfg/arguments.h libraries/msxbios/extbio.h
mbrot.o: mbrot.c config_request.h libraries/msxdos/msxdos.h v9958.h
rtccalb.o: rtccalb/rtccalb.c print.h libraries/msxdos/msxdos.h
config_request.o: config_request.c v9958.h
romflash.o: romflash/romflash.c romflash/flashloader.h \
 romflash/memmapper.h libraries/msxbios/extbio.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
./bin/cpusptst/cpusptst.o: ./cpusptst/cpusptst.asm
./bin/xstdio.o: ./xstdio.asm
./bin/clrdir/fdisk.o: ./clrdir/fdisk.asm
./bin/usbtst/print.o: ./usbtst/print.asm
./bin/romflashwriter/romflashwriter.o: ./romflashwriter/romflashwriter.asm
./bin/fdisk/fdisk.o: ./fdisk/fdisk.asm
./bin/term/print.o: ./term/print.asm
./bin/term/msx2ansi.o: ./term/msx2ansi.asm
./bin/term/screen.o: ./term/screen.asm
./bin/memmap-override.o: ./memmap-override.asm
./bin/muflash/slot_helpers.o: ./muflash/slot_helpers.asm
./bin/getslt.o: ./getslt.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_get_present.o: ./libraries/extbio/extbio_usb_get_present.asm ./msx.inc
./bin/libraries/extbio/extbio.o: ./libraries/extbio/extbio.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_control_transfer.o: ./libraries/extbio/extbio_usb_control_transfer.asm ./msx.inc
./bin/libraries/fusion/inputchar.o: ./libraries/fusion/inputchar.asm
./bin/libraries/fusion/io.o: ./libraries/fusion/io.asm
./bin/libraries/fusion/inputstring.o: ./libraries/fusion/inputstring.asm
./bin/libraries/fusion/inkey.o: ./libraries/fusion/inkey.asm
./bin/libraries/msxdos/msxdos_get_drive_letter_info.o: ./libraries/msxdos/msxdos_get_drive_letter_info.asm ./libraries/msxdos/msxdos.inc
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
