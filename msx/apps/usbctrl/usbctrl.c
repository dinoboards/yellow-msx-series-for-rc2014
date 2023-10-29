#include "arguments.h"
#include "command_floppy_check.h"
#include "command_floppy_dump.h"
#include "command_floppy_format.h"
#include "command_floppy_report.h"
#include "command_floppy_report_drive.h"
#include "command_ftdi_check.h"
#include "command_printer_check.h"
#include "command_report_all_devices.h"
#include <extbio_rc2014.h>

uint8_t main(const int argc, const char *const argv[]) {
  process_cli_arguments(argc, argv);

  uint16_t      hl = extbio_usb_get_present();
  const uint8_t h  = hl >> 8;
  const uint8_t l  = hl & 0xFF;

  if (!h) {
    printf("CH376 not detected\r\n");
    return 255;
  }

  switch (subcommand) {
  case cmd_none:
    break;

  case cmd_floppy_format:
    return command_floppy_format();

  case cmd_floppy_check:
    return command_floppy_check();

  case cmd_report_all_devices:
    command_report_all_devices(l);
    printf("usb device details saved to file %s\r\n", report_file_name);
    break;

  case cmd_floppy_report:
    return command_floppy_report(l);

  case cmd_floppy_report_drive:
    return command_floppy_report_drive();

  case cmd_floppy_dump:
    return command_floppy_dump();

  case cmd_printer_check:
    return command_printer_check();

  case cmd_ftdi_check:
    return command_ftdi_check();
  }

  return 0;
}
