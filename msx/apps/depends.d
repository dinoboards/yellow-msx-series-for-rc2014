
./bin/cpusptst/main.c.asm: cpusptst/main.c
./bin/dots.c.asm: dots.c config_request.h v9958.h libraries/msxbios/msxbios.h \
 libraries/msxdos/msxdos.h
./bin/extbio/main.c.asm: extbio/main.c libraries/fossil/fossil.h libraries/msxdos/msxdos.h \
 xstdio.h libraries/msxbios/extbio.h libraries/msxbios/system_vars.h
./bin/lines.c.asm: lines.c config_request.h v9958.h libraries/msxbios/msxbios.h \
 libraries/msxdos/msxdos.h
./bin/esp8266/wget.c.asm: esp8266/wget.c esp8266/wget.h esp8266/arguments.h \
 esp8266/esp8266.h print.h libraries/extbio/serial-helpers.h \
 libraries/extbio/serial.h libraries/msx/libgen.h \
 libraries/msxbios/msxbios.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h libraries/xmodem/utils.h \
 libraries/xmodem/xmodem.h
./bin/esp8266/arguments.c.asm: esp8266/arguments.c esp8266/arguments.h print.h \
 libraries/extbio/serial-helpers.h libraries/extbio/serial.h
./bin/esp8266/msxhub.c.asm: esp8266/msxhub.c esp8266/msxhub.h esp8266/arguments.h \
 esp8266/esp8266.h print.h esp8266/wget.h libraries/xmodem/xmodem.h
./bin/esp8266/esp8266.c.asm: esp8266/esp8266.c esp8266/esp8266.h esp8266/arguments.h \
 esp8266/msxhub.h print.h esp8266/wget.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/msxbios/extbio.h \
 libraries/extbio/serial-helpers.h libraries/extbio/serial.h \
 libraries/msxdos/msxdos.h
./bin/memory/memory.c.asm: memory/memory.c memory/memory.h libraries/msxbios/extbio.h \
 libraries/msxbios/system_vars.h libraries/msxdos/msxdos.h
./bin/xrecv2/arguments.c.asm: xrecv2/arguments.c xrecv2/arguments.h print.h
./bin/xrecv2/xrecv.c.asm: xrecv2/xrecv.c xrecv2/arguments.h libraries/msxdos/msxdos.h \
 print.h libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/msxbios/extbio.h libraries/fossil/fossil.h \
 libraries/xmodem/utils.h libraries/xmodem/xmodem.h
./bin/clrdir/arguments.c.asm: clrdir/arguments.c clrdir/arguments.h
./bin/clrdir/fdisk2.c.asm: clrdir/fdisk2.c clrdir/fdisk2.h datatypes.h \
 libraries/msxdos/partition.h libraries/msxdos/fat.h \
 libraries/msxdos/msxdos.h
./bin/clrdir/clrdir.c.asm: clrdir/clrdir.c clrdir/arguments.h clrdir/fdisk2.h datatypes.h \
 libraries/msxdos/partition.h libraries/msxdos/fat.h \
 libraries/fusion/io.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
./bin/vramtest.c.asm: vramtest.c v9958.h libraries/msxbios/msxbios.h \
 libraries/msxdos/msxdos.h
./bin/usbctrl/reporters.c.asm: usbctrl/reporters.c usbctrl/reporters.h \
 libraries/usb/protocol.h libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 usbctrl/rotator.h libraries/fusion/fusion_ext.h
./bin/usbctrl/command_floppy_dump.c.asm: usbctrl/command_floppy_dump.c \
 usbctrl/command_floppy_check.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/device_search.h usbctrl/arguments.h \
 libraries/usb/class_ufi.h libraries/usb/ch376.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h usbctrl/read_and_write_sectors.h \
 libraries/msxdos/msxdos.h
./bin/usbctrl/command_floppy_report_drive.c.asm: usbctrl/command_floppy_report_drive.c \
 usbctrl/command_floppy_report_drive.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/arguments.h usbctrl/device_search.h \
 libraries/usb/class_ufi.h libraries/usb/ch376.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h usbctrl/ufi_reporters.h \
 libraries/usb/format_mapping.h libraries/msxdos/msxdos.h \
 libraries/msxdos/msxdos_rc2014_ex.h
./bin/usbctrl/usbctrl.c.asm: usbctrl/usbctrl.c usbctrl/arguments.h \
 usbctrl/command_floppy_check.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/command_floppy_dump.h \
 usbctrl/command_floppy_format.h usbctrl/command_floppy_report.h \
 usbctrl/command_floppy_report_drive.h usbctrl/command_ftdi_check.h \
 usbctrl/command_printer_check.h usbctrl/command_report_all_devices.h \
 libraries/msxbios/extbio_rc2014.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h
./bin/usbctrl/format_fats.c.asm: usbctrl/format_fats.c usbctrl/format_fats.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/ch376.h \
 libraries/usb/transfers.h libraries/usb/format_mapping.h \
 usbctrl/read_and_write_sectors.h usbctrl/rotator.h \
 libraries/msxdos/fat.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
./bin/usbctrl/command_floppy_report.c.asm: usbctrl/command_floppy_report.c \
 usbctrl/command_floppy_report.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/device_search.h usbctrl/arguments.h \
 libraries/usb/class_ufi.h libraries/usb/ch376.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h
./bin/usbctrl/arguments.c.asm: usbctrl/arguments.c usbctrl/arguments.h
./bin/usbctrl/command_report_all_devices.c.asm: usbctrl/command_report_all_devices.c \
 usbctrl/command_report_all_devices.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/arguments.h usbctrl/reporters.h \
 libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 usbctrl/rotator.h libraries/fusion/fusion_ext.h libraries/fusion/io.h
./bin/usbctrl/device_search.c.asm: usbctrl/device_search.c usbctrl/device_search.h \
 usbctrl/arguments.h libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/class_ufi.h libraries/usb/ch376.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h libraries/msxdos/msxdos.h \
 libraries/msxdos/msxdos_rc2014_ex.h libraries/usb/protocol.h \
 libraries/usb/find_device.h
./bin/usbctrl/command_floppy_format.c.asm: usbctrl/command_floppy_format.c \
 usbctrl/command_floppy_format.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/device_search.h usbctrl/arguments.h \
 libraries/usb/class_ufi.h libraries/usb/ch376.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h usbctrl/format_fats.h \
 libraries/usb/format_mapping.h libraries/usb/usb_trace.h \
 libraries/usb/class_ufi.h
./bin/usbctrl/command_floppy_check.c.asm: usbctrl/command_floppy_check.c \
 usbctrl/command_floppy_check.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/device_search.h usbctrl/arguments.h \
 libraries/usb/class_ufi.h libraries/usb/ch376.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h libraries/msxdos/msxdos.h
./bin/usbctrl/ufi_reporters.c.asm: usbctrl/ufi_reporters.c libraries/usb/class_ufi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h
./bin/usbctrl/rotator.c.asm: usbctrl/rotator.c usbctrl/rotator.h
./bin/usbctrl/command_printer_check.c.asm: usbctrl/command_printer_check.c \
 usbctrl/command_printer_check.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h usbctrl/arguments.h usbctrl/device_search.h \
 libraries/usb/class_ufi.h libraries/usb/ch376.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/fusion/msx_fusion.h \
 libraries/usb/protocol.h libraries/usb/class_printer.h \
 libraries/usb/find_device.h
./bin/usbctrl/command_ftdi_check.c.asm: usbctrl/command_ftdi_check.c \
 usbctrl/../sys/ftdi/vendor_ftdi.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/ch376.h libraries/usb/transfers.h usbctrl/arguments.h \
 usbctrl/command_printer_check.h libraries/usb/ch376.h \
 usbctrl/device_search.h libraries/usb/class_ufi.h \
 libraries/usb/protocol.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/dev_transfers.h \
 libraries/usb/usb_state.h libraries/usb/class_printer.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/fusion/msx_fusion.h libraries/msxdos/msxdos.h \
 libraries/usb/protocol.h libraries/usb/find_device.h
./bin/usbctrl/read_and_write_sectors.c.asm: usbctrl/read_and_write_sectors.c \
 usbctrl/read_and_write_sectors.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/ch376.h libraries/usb/transfers.h \
 libraries/usb/class_ufi.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_cbi.h \
 libraries/usb/usb_state.h
./bin/helloworld.c.asm: helloworld.c
./bin/usbtst/usb-dev-info-ufi.c.asm: usbtst/usb-dev-info-ufi.c usbtst/usb-dev-info-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/class_ufi.h \
 libraries/usb/protocol.h libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h \
 usbtst/usb-dev-info.h libraries/usb/usb_state.h
./bin/usbtst/usb-dev-write-ufi.c.asm: usbtst/usb-dev-write-ufi.c \
 usbtst/usb-dev-write-ufi.h usbtst/nextor.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/class_ufi.h libraries/usb/protocol.h \
 libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
./bin/usbtst/work-area.c.asm: usbtst/work-area.c libraries/usb/usb_state.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h
./bin/usbtst/usb-lun-info-ufi.c.asm: usbtst/usb-lun-info-ufi.c usbtst/usb-lun-info-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/class_ufi.h \
 libraries/usb/protocol.h libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h
./bin/usbtst/usb-dev-read-ufi.c.asm: usbtst/usb-dev-read-ufi.c usbtst/usb-dev-read-ufi.h \
 usbtst/nextor.h libraries/usb/protocol.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/class_ufi.h \
 libraries/usb/protocol.h libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h
./bin/usbtst/main.c.asm: usbtst/main.c usbtst/main.h usbtst/print.h \
 usbtst/usb-dev-info-ufi.h usbtst/nextor.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 usbtst/usb-dev-read-ufi.h usbtst/usb-dev-write-ufi.h \
 usbtst/usb-lun-info-ufi.h libraries/usb/usb_state.h \
 libraries/usb/protocol.h libraries/usb/ch376.h libraries/usb/class_hid.h \
 libraries/usb/class_hid_keyboard.h libraries/usb/class_scsi.h \
 libraries/usb/class_ufi.h libraries/usb/usb_cbi.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h \
 libraries/usb/enumerate.h libraries/msxdos/msxdos.h \
 libraries/usb/usb_trace.h libraries/usb/class_ufi.h
./bin/service/service.c.asm: service/service.c libraries/msxdos/msxdos.h
./bin/fdisk/screen_control.c.asm: fdisk/screen_control.c fdisk/screen_control.h \
 libraries/msxdos/msxdos.h libraries/msxbios/msxbios.h \
 libraries/msxbios/system_vars.h
./bin/fdisk/driver_selection.c.asm: fdisk/driver_selection.c fdisk/driver_selection.h \
 libraries/msxdos/msxdos.h fdisk/device_selection.h fdisk/key_inputs.h \
 fdisk/screen_control.h fdisk/string_helper.h fdisk/temp.h \
 libraries/msxdos/partition.h datatypes.h
./bin/fdisk/string_helper.c.asm: fdisk/string_helper.c fdisk/string_helper.h
./bin/fdisk/lun_selection.c.asm: fdisk/lun_selection.c fdisk/lun_selection.h \
 libraries/msxdos/msxdos.h fdisk/device_selection.h \
 fdisk/driver_selection.h fdisk/key_inputs.h fdisk/partition_selection.h \
 libraries/msxdos/partition.h datatypes.h fdisk/screen_control.h \
 fdisk/temp.h
./bin/fdisk/fdisk.c.asm: fdisk/fdisk.c fdisk/fdisk.h libraries/msxdos/msxdos.h \
 fdisk/device_selection.h fdisk/driver_selection.h fdisk/fdisk2.h \
 datatypes.h libraries/msxdos/partition.h fdisk/key_inputs.h \
 fdisk/lun_selection.h fdisk/partition_selection.h fdisk/screen_control.h \
 fdisk/string_helper.h fdisk/temp.h libraries/msxbios/system_vars.h
./bin/fdisk/fdisk2.c.asm: fdisk/fdisk2.c fdisk/fdisk2.h datatypes.h \
 libraries/msxdos/partition.h fdisk/fdisk.h libraries/msxdos/msxdos.h \
 fdisk/lun_selection.h libraries/msxdos/fat.h
./bin/fdisk/key_inputs.c.asm: fdisk/key_inputs.c fdisk/key_inputs.h \
 fdisk/screen_control.h libraries/msxdos/msxdos.h
./bin/fdisk/device_selection.c.asm: fdisk/device_selection.c fdisk/device_selection.h \
 libraries/msxdos/msxdos.h fdisk/driver_selection.h fdisk/key_inputs.h \
 fdisk/lun_selection.h fdisk/screen_control.h fdisk/string_helper.h \
 fdisk/temp.h libraries/msxdos/partition.h datatypes.h
./bin/fdisk/partition_selection.c.asm: fdisk/partition_selection.c \
 fdisk/partition_selection.h libraries/msxdos/partition.h datatypes.h \
 fdisk/device_selection.h libraries/msxdos/msxdos.h \
 fdisk/driver_selection.h fdisk/key_inputs.h fdisk/lun_selection.h \
 fdisk/screen_control.h fdisk/temp.h
./bin/v9958.c.asm: v9958.c v9958.h
./bin/term/xymodem.c.asm: term/xymodem.c term/xymodem.h term/aofossilhelper.h \
 libraries/fusion/msx_fusion.h term/print.h libraries/fusion/io.h
./bin/term/term.c.asm: term/term.c term/term.h libraries/fusion/msx_fusion.h \
 term/system-state.h term/ansi_codes.h term/ansiprint.h term/print.h \
 term/aofossilhelper.h term/arguments.h libraries/msxdos/msxdos.h \
 term/xymodem.h libraries/msxbios/extbio.h libraries/msxbios/msxbios.h \
 libraries/msxbios/system_vars.h
./bin/term/arguments.c.asm: term/arguments.c term/arguments.h term/system-state.h
./bin/term/aofossilhelper.c.asm: term/aofossilhelper.c term/aofossilhelper.h \
 term/arguments.h term/msx2ansi.h libraries/fusion/msx_fusion.h \
 term/print.h term/system-state.h libraries/extbio/serial.h
./bin/term/fossil_interface.c.asm: term/fossil_interface.c libraries/fusion/msx_fusion.h
./bin/term/ansiprint.c.asm: term/ansiprint.c term/ansiprint.h term/print.h \
 term/msx2ansi.h
./bin/term/system-state.c.asm: term/system-state.c term/system-state.h
./bin/print.c.asm: print.c print.h
./bin/rs232tst.c.asm: rs232tst.c libraries/msxbios/extbio.h \
 libraries/msxdos/msxdos.h libraries/msxbios/system_vars.h
./bin/muflash/arguments.c.asm: muflash/arguments.c muflash/arguments.h print.h
./bin/muflash/main.c.asm: muflash/main.c muflash/arguments.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
./bin/libraries/extbio/serial-helper-write-bytes.c.asm: libraries/extbio/serial-helper-write-bytes.c \
 libraries/msxdos/msxdos.h libraries/extbio/serial-helpers.h \
 libraries/extbio/serial.h libraries/msxbios/system_vars.h \
 libraries/z80.h
./bin/libraries/extbio/serial-helper-read-char.c.asm: libraries/extbio/serial-helper-read-char.c \
 libraries/msxdos/msxdos.h libraries/extbio/serial-helpers.h \
 libraries/extbio/serial.h libraries/msxbios/system_vars.h \
 libraries/z80.h
./bin/libraries/extbio/serial-helper-write-string.c.asm: \
 libraries/extbio/serial-helper-write-string.c libraries/msxdos/msxdos.h \
 libraries/extbio/serial-helpers.h libraries/extbio/serial.h \
 libraries/msxbios/system_vars.h libraries/z80.h
./bin/libraries/extbio/serial-helper-write-char.c.asm: libraries/extbio/serial-helper-write-char.c \
 libraries/msxdos/msxdos.h libraries/extbio/serial-helpers.h \
 libraries/extbio/serial.h libraries/msxbios/system_vars.h \
 libraries/z80.h
./bin/libraries/extbio/serial-helper-find-driver.c.asm: libraries/extbio/serial-helper-find-driver.c \
 libraries/extbio/serial-helpers.h libraries/extbio/serial.h
./bin/libraries/fusion/width.c.asm: libraries/fusion/width.c libraries/fusion/msx_fusion.h
./bin/libraries/fusion/fprintf.c.asm: libraries/fusion/fprintf.c libraries/fusion/fusion_ext.h \
 libraries/fusion/io.h
./bin/libraries/msxdos/rename.c.asm: libraries/msxdos/rename.c
./bin/libraries/xmodem/crc16.c.asm: libraries/xmodem/crc16.c libraries/xmodem/crc16.h
./bin/libraries/xmodem/xmodem.c.asm: libraries/xmodem/xmodem.c libraries/xmodem/xmodem.h \
 libraries/xmodem/crc16.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/extbio/serial-helpers.h \
 libraries/extbio/serial.h libraries/z80.h
./bin/libraries/xmodem/utils.c.asm: libraries/xmodem/utils.c libraries/xmodem/utils.h
./bin/libraries/delay/delay.c.asm: libraries/delay/delay.c libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
./bin/libraries/msx/libgen/basename.c.asm: libraries/msx/libgen/basename.c \
 libraries/msx/libgen/../libgen.h
./bin/libraries/msx/libgen/dirname.c.asm: libraries/msx/libgen/dirname.c \
 libraries/msx/libgen/../libgen.h
./bin/libraries/usb/dev_transfers.c.asm: libraries/usb/dev_transfers.c \
 libraries/usb/dev_transfers.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/transfers.h \
 libraries/usb/protocol.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h
./bin/libraries/usb/class_printer.c.asm: libraries/usb/class_printer.c \
 libraries/usb/class_printer.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/protocol.h
./bin/libraries/usb/usb_cbi.c.asm: libraries/usb/usb_cbi.c libraries/usb/usb_cbi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/ch376.h \
 libraries/usb/transfers.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h
./bin/libraries/usb/enumerate.c.asm: libraries/usb/enumerate.c libraries/usb/enumerate.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h libraries/usb/enumerate_hub.h \
 libraries/usb/enumerate_storage.h
./bin/libraries/usb/usb_state.c.asm: libraries/usb/usb_state.c libraries/usb/usb_state.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h
./bin/libraries/usb/class_hub.c.asm: libraries/usb/class_hub.c libraries/usb/class_hub.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h
./bin/libraries/usb/trace_ufi_format_capacities_response.c.asm: \
 libraries/usb/trace_ufi_format_capacities_response.c \
 libraries/usb/usb_trace.h libraries/usb/class_ufi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h
./bin/libraries/usb/enumerate_storage.c.asm: libraries/usb/enumerate_storage.c \
 libraries/usb/enumerate_storage.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h
./bin/libraries/usb/enumerate_hub.c.asm: libraries/usb/enumerate_hub.c \
 libraries/usb/enumerate_hub.h libraries/usb/enumerate.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h libraries/usb/class_hub.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
./bin/libraries/usb/transfers.c.asm: libraries/usb/transfers.c libraries/usb/transfers.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/z80.h
./bin/libraries/usb/class_ufi.c.asm: libraries/usb/class_ufi.c libraries/usb/class_ufi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_cbi.h libraries/usb/ch376.h \
 libraries/usb/dev_transfers.h libraries/usb/usb_state.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h libraries/z80.h
./bin/libraries/usb/class_hid.c.asm: libraries/usb/class_hid.c libraries/usb/class_hid.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h
./bin/libraries/usb/find_device.c.asm: libraries/usb/find_device.c libraries/usb/find_device.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/msxbios/extbio_rc2014.h libraries/usb/protocol.h
./bin/libraries/usb/ch376.c.asm: libraries/usb/ch376.c libraries/usb/ch376.h \
 libraries/usb/ch376inc.h
./bin/libraries/usb/protocol.c.asm: libraries/usb/protocol.c libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
./bin/libraries/usb/class_hid_keyboard.c.asm: libraries/usb/class_hid_keyboard.c \
 libraries/usb/class_hid_keyboard.h
./bin/libraries/usb/class_scsi.c.asm: libraries/usb/class_scsi.c libraries/usb/class_scsi.h \
 libraries/usb/protocol.h libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/usb/usb_state.h libraries/msxbios/system_vars.h \
 libraries/z80.h
./bin/libraries/usb/format_mapping.c.asm: libraries/usb/format_mapping.c \
 libraries/usb/format_mapping.h
./bin/libraries/sysload/sysload.c.asm: libraries/sysload/sysload.c libraries/sysload/sysload.h \
 libraries/sysload/sys.h libraries/msxbios/extbio.h libraries/fusion/io.h \
 libraries/msxdos/msxdos.h
./bin/libraries/msxbios/system_vars.c.asm: libraries/msxbios/system_vars.c \
 libraries/msxbios/system_vars.h
./bin/libraries/fossil/fossil.c.asm: libraries/fossil/fossil.c libraries/fossil/fossil.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h
./bin/fossilon.c.asm: fossilon.c libraries/msxdos/msxdos.h xstdio.h \
 libraries/msxbios/extbio.h
./bin/sys/hello/init/hello.c.asm: sys/hello/init/hello.c libraries/sysload/sysload.h \
 libraries/sysload/sys.h
./bin/sys/sio2/body/sio2.c.asm: sys/sio2/body/sio2.c sys/sio2/body/../sio2.h \
 libraries/delay/delay.h libraries/msxbios/system_vars.h libraries/z80.h
./bin/sys/sio2/init/sio2.c.asm: sys/sio2/init/sio2.c sys/sio2/init/../sio2.h \
 libraries/extbio/serial.h libraries/sysload/sysload.h \
 libraries/sysload/sys.h
./bin/sys/ftdi/body/ftdi.c.asm: sys/ftdi/body/ftdi.c sys/ftdi/body/../ftdi.h \
 sys/ftdi/body/../vendor_ftdi.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/ch376.h libraries/usb/transfers.h \
 libraries/extbio/serial.h
./bin/sys/ftdi/vendor_ftdi.c.asm: sys/ftdi/vendor_ftdi.c sys/ftdi/vendor_ftdi.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/ch376.h \
 libraries/usb/transfers.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/z80.h
./bin/sys/ftdi/init/ftdi.c.asm: sys/ftdi/init/ftdi.c sys/ftdi/init/../ftdi.h \
 sys/ftdi/init/../vendor_ftdi.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/ch376.h libraries/usb/transfers.h \
 libraries/sysload/sysload.h libraries/sysload/sys.h \
 libraries/usb/find_device.h libraries/usb/protocol.h \
 libraries/usb/dev_transfers.h libraries/usb/protocol.h
./bin/sys/fossil/body/fossil-drv.c.asm: sys/fossil/body/fossil-drv.c libraries/extbio/serial.h \
 libraries/z80.h
./bin/sys/fossil/init/fossil.c.asm: sys/fossil/init/fossil.c sys/fossil/init/fossil.h \
 libraries/msxbios/system_vars.h libraries/msxdos/msxdos.h \
 libraries/z80.h
./bin/sys/prnt/body/prnt.c.asm: sys/prnt/body/prnt.c libraries/msxbios/system_vars.h \
 libraries/usb/class_printer.h libraries/usb/ch376.h \
 libraries/usb/ch376inc.h libraries/usb/dev_transfers.h \
 libraries/usb/transfers.h libraries/usb/protocol.h
./bin/sys/prnt/init/prnt.c.asm: sys/prnt/init/prnt.c sys/prnt/init/../prnt.h \
 libraries/msxbios/extbio_rc2014.h libraries/usb/protocol.h \
 libraries/usb/ch376.h libraries/usb/ch376inc.h \
 libraries/usb/dev_transfers.h libraries/usb/transfers.h \
 libraries/fusion/msx_fusion.h libraries/sysload/sys.h \
 libraries/sysload/sysload.h libraries/sysload/sys.h \
 libraries/msxbios/system_vars.h libraries/usb/class_printer.h \
 libraries/usb/protocol.h libraries/usb/find_device.h
./bin/fosiltst.c.asm: fosiltst.c libraries/fossil/fossil.h \
 libraries/msxbios/extbio.h libraries/msxdos/msxdos.h
./bin/siocfg/arguments.c.asm: siocfg/arguments.c siocfg/arguments.h
./bin/siocfg/siocfg.c.asm: siocfg/siocfg.c siocfg/arguments.h libraries/msxbios/extbio.h
./bin/mbrot.c.asm: mbrot.c config_request.h libraries/msxbios/msxbios.h \
 libraries/msxdos/msxdos.h v9958.h
./bin/rtccalb/rtccalb.c.asm: rtccalb/rtccalb.c print.h libraries/msxdos/msxdos.h
./bin/config_request.c.asm: config_request.c v9958.h
./bin/sraltest/baud_rate_clock_calculator.c.asm: sraltest/baud_rate_clock_calculator.c \
 sraltest/baud_rate_clock_calculator.h
./bin/sraltest/sraltest.c.asm: sraltest/sraltest.c sraltest/../sys/sio2/sio2.h \
 sraltest/baud_rate_clock_calculator.h libraries/delay/delay.h \
 libraries/msxbios/system_vars.h libraries/extbio/serial-helpers.h \
 libraries/extbio/serial.h libraries/msxdos/msxdos.h
./bin/mode/mode.c.asm: mode/mode.c libraries/extbio/serial.h
./bin/romflash/romflash.c.asm: romflash/romflash.c romflash/flashloader.h \
 libraries/msxbios/extbio.h libraries/msxdos/msxdos.h \
 libraries/msxbios/system_vars.h
./bin/cpusptst/cpusptst.o: ./cpusptst/cpusptst.asm
./bin/xstdio.o: ./xstdio.asm
./bin/clrdir/fdisk.o: ./clrdir/fdisk.asm
./bin/usbctrl/floopy_boot_code.o: ./usbctrl/floopy_boot_code.asm
./bin/usbtst/print.o: ./usbtst/print.asm
./bin/romflashwriter/romflashwriter.o: ./romflashwriter/romflashwriter.asm
./bin/service/service.o: ./service/service.asm
./bin/fdisk/fdisk.o: ./fdisk/fdisk.asm
./bin/term/print.o: ./term/print.asm
./bin/term/msx2ansi.o: ./term/msx2ansi.asm
./bin/term/screen.o: ./term/screen.asm
./bin/memmap-override.o: ./memmap-override.asm
./bin/muflash/slot_helpers.o: ./muflash/slot_helpers.asm
./bin/getslt.o: ./getslt.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_get_driver_name.o: ./libraries/extbio/extbio_serial_get_driver_name.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_read.o: ./libraries/extbio/extbio_serial_read.asm ./msx.inc
./bin/libraries/extbio/extbio_sio_get_present.o: ./libraries/extbio/extbio_sio_get_present.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_set_protocol.o: ./libraries/extbio/extbio_serial_set_protocol.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_get_present.o: ./libraries/extbio/extbio_usb_get_present.asm ./msx.inc
./bin/libraries/extbio/unapi_get_ram_helper.o: ./libraries/extbio/unapi_get_ram_helper.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_set_baudrate.o: ./libraries/extbio/extbio_serial_set_baudrate.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_ftdi_poll_modem_status.o: ./libraries/extbio/extbio_usb_ftdi_poll_modem_status.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_write.o: ./libraries/extbio/extbio_serial_write.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_set_dtr_rts.o: ./libraries/extbio/extbio_serial_set_dtr_rts.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_data_out_transfer.o: ./libraries/extbio/extbio_usb_data_out_transfer.asm ./msx.inc
./bin/libraries/extbio/enable_vdp_interrupts.o: ./libraries/extbio/enable_vdp_interrupts.asm ./msx.inc
./bin/libraries/extbio/extbio_rc2014_hello.o: ./libraries/extbio/extbio_rc2014_hello.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_ftdi_purge_rx_buffer.o: ./libraries/extbio/extbio_usb_ftdi_purge_rx_buffer.asm ./msx.inc
./bin/libraries/extbio/extbio_sio_set_clock.o: ./libraries/extbio/extbio_sio_set_clock.asm ./msx.inc
./bin/libraries/extbio/disable_vdp_interrupts.o: ./libraries/extbio/disable_vdp_interrupts.asm ./msx.inc
./bin/libraries/extbio/extbio_fossil_install.o: ./libraries/extbio/extbio_fossil_install.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_data_in_transfer.o: ./libraries/extbio/extbio_usb_data_in_transfer.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_ftdi_set_line_property2.o: ./libraries/extbio/extbio_usb_ftdi_set_line_property2.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_set_flowctrl.o: ./libraries/extbio/extbio_serial_set_flowctrl.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_demand_read.o: ./libraries/extbio/extbio_serial_demand_read.asm ./msx.inc
./bin/libraries/extbio/memmapper.o: ./libraries/extbio/memmapper.asm
./bin/libraries/extbio/extbio_usb_ftdi_purge_tx_buffer.o: ./libraries/extbio/extbio_usb_ftdi_purge_tx_buffer.asm ./msx.inc
./bin/libraries/extbio/extbio_get_dev_info_table.o: ./libraries/extbio/extbio_get_dev_info_table.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_control_transfer.o: ./libraries/extbio/extbio_usb_control_transfer.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_data_in_transfer_n.o: ./libraries/extbio/extbio_usb_data_in_transfer_n.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_get_available_ports.o: ./libraries/extbio/extbio_serial_get_available_ports.asm ./msx.inc
./bin/libraries/extbio/extbio_rc2014_get_version.o: ./libraries/extbio/extbio_rc2014_get_version.asm ./msx.inc
./bin/libraries/extbio/debug_break.o: ./libraries/extbio/debug_break.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_get_diagnostics.o: ./libraries/extbio/extbio_serial_get_diagnostics.asm ./msx.inc
./bin/libraries/extbio/extbio_sio_get_clock.o: ./libraries/extbio/extbio_sio_get_clock.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_ftdi_read_data.o: ./libraries/extbio/extbio_usb_ftdi_read_data.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_get_rx_buffer_size.o: ./libraries/extbio/extbio_serial_get_rx_buffer_size.asm ./msx.inc
./bin/libraries/extbio/extbio_serial_purge_buffers.o: ./libraries/extbio/extbio_serial_purge_buffers.asm ./msx.inc
./bin/libraries/extbio/extbio_extbio_get_dev.o: ./libraries/extbio/extbio_extbio_get_dev.asm ./msx.inc
./bin/libraries/extbio/extbio_usb_ftdi_set_baudrate.o: ./libraries/extbio/extbio_usb_ftdi_set_baudrate.asm ./msx.inc
./bin/libraries/fusion/inputchar.o: ./libraries/fusion/inputchar.asm
./bin/libraries/fusion/io.o: ./libraries/fusion/io.asm
./bin/libraries/fusion/interrupts.o: ./libraries/fusion/interrupts.asm
./bin/libraries/fusion/inputstring.o: ./libraries/fusion/inputstring.asm
./bin/libraries/fusion/inkey.o: ./libraries/fusion/inkey.asm
./bin/libraries/msxdos/fputc_cons_native.o: ./libraries/msxdos/fputc_cons_native.asm
./bin/libraries/msxdos/msxdos_set_environment.o: ./libraries/msxdos/msxdos_set_environment.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_get_environment.o: ./libraries/msxdos/msxdos_get_environment.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_drv_direct_1.o: ./libraries/msxdos/msxdos_drv_direct_1.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_rename_file.o: ./libraries/msxdos/msxdos_rename_file.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_get_drive_letter_info.o: ./libraries/msxdos/msxdos_get_drive_letter_info.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_explain_error_code.o: ./libraries/msxdos/msxdos_explain_error_code.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_read_file.o: ./libraries/msxdos/msxdos_read_file.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_rejoin_parent_process.o: ./libraries/msxdos/msxdos_rejoin_parent_process.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_drv_dev_get_name.o: ./libraries/msxdos/msxdos_drv_dev_get_name.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxbios_break_x.o: ./libraries/msxdos/msxbios_break_x.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_gdrvr.o: ./libraries/msxdos/msxdos_gdrvr.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_dev_read.o: ./libraries/msxdos/msxdos_dev_read.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_get_time.o: ./libraries/msxdos/msxdos_get_time.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_gpart.o: ./libraries/msxdos/msxdos_gpart.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_dirio.o: ./libraries/msxdos/msxdos_dirio.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_find_environment.o: ./libraries/msxdos/msxdos_find_environment.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxbios_posit.o: ./libraries/msxdos/msxbios_posit.asm ./msx.inc
./bin/libraries/msxdos/msxdos_callsub.o: ./libraries/msxdos/msxdos_callsub.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_terminate_with_error.o: ./libraries/msxdos/msxdos_terminate_with_error.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_write_file.o: ./libraries/msxdos/msxdos_write_file.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_dev_write.o: ./libraries/msxdos/msxdos_dev_write.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_drv_dev_logical_unit_count.o: ./libraries/msxdos/msxdos_drv_dev_logical_unit_count.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxbios_init_palette.o: ./libraries/msxdos/msxbios_init_palette.asm ./msx.inc
./bin/libraries/msxdos/msxdos_open_file.o: ./libraries/msxdos/msxdos_open_file.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_drv_lun_info.o: ./libraries/msxdos/msxdos_drv_lun_info.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_delete_file.o: ./libraries/msxdos/msxdos_delete_file.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_set_time.o: ./libraries/msxdos/msxdos_set_time.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_create_file.o: ./libraries/msxdos/msxdos_create_file.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_close_file.o: ./libraries/msxdos/msxdos_close_file.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_set_date.o: ./libraries/msxdos/msxdos_set_date.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/msxdos/msxdos_work_area.o: ./libraries/msxdos/msxdos_work_area.asm ./libraries/msxdos/msxdos.inc
./bin/libraries/usb/ch376.o: ./libraries/usb/ch376.asm
./bin/libraries/sysload/sysload.o: ./libraries/sysload/sysload.asm ./msx.inc
./bin/libraries/msxbios/msxbios_initxt.o: ./libraries/msxbios/msxbios_initxt.asm ./msx.inc
./bin/libraries/msxbios/msxbios_init32.o: ./libraries/msxbios/msxbios_init32.asm ./msx.inc
./bin/libraries/msxbios/msxbios_soft_reset.o: ./libraries/msxbios/msxbios_soft_reset.asm ./msx.inc
./bin/libraries/fossil/fossil.o: ./libraries/fossil/fossil.asm ./msx.inc
./bin/sys/hello/body/crt.o: ./sys/hello/body/crt.asm
./bin/sys/hello/body/hello.o: ./sys/hello/body/hello.asm ./msx.inc
./bin/sys/sio2/body/sio_out.o: ./sys/sio2/body/sio_out.asm ./msx.inc
./bin/sys/sio2/body/crt.o: ./sys/sio2/body/crt.asm
./bin/sys/sio2/body/sio_in.o: ./sys/sio2/body/sio_in.asm ./msx.inc
./bin/sys/sio2/body/data.o: ./sys/sio2/body/data.asm ./msx.inc 
./bin/sys/sio2/body/sio2.o: ./sys/sio2/body/sio2.asm ./msx.inc
./bin/sys/sio2/body/sio_configure.o: ./sys/sio2/body/sio_configure.asm ./msx.inc 
./bin/sys/sio2/body/interrupt_handler.o: ./sys/sio2/body/interrupt_handler.asm ./msx.inc 
./bin/sys/sio2/init/init.o: ./sys/sio2/init/init.asm ./msx.inc 
./bin/sys/sio2/init/probe.o: ./sys/sio2/init/probe.asm
./bin/sys/ftdi/body/crt.o: ./sys/ftdi/body/crt.asm
./bin/sys/ftdi/body/ftdi.o: ./sys/ftdi/body/ftdi.asm ./msx.inc
./bin/sys/ftdi/init/init.o: ./sys/ftdi/init/init.asm
./bin/sys/fossil/body/crt.o: ./sys/fossil/body/crt.asm
./bin/sys/fossil/body/fossil-drv.o: ./sys/fossil/body/fossil-drv.asm ./msx.inc
./bin/sys/fossil/init/transition.o: ./sys/fossil/init/transition.asm
./bin/sys/prnt/body/crt.o: ./sys/prnt/body/crt.asm
./bin/sys/prnt/body/prnt.o: ./sys/prnt/body/prnt.asm
./bin/rtccalb/mesaure.o: ./rtccalb/mesaure.asm
./bin/v9958.o: ./v9958.asm ./msx.inc
./bin/romflash/flashloader.o: ./romflash/flashloader.asm 
./bin/utils.o: ./utils.asm 
./bin/rs232.o: ./rs232.asm ./msx.inc
