#include "arguments.h"
#include "command_floppy_check.h"
#include "command_floppy_find.h"
#include "command_floppy_format.h"
#include "command_floppy_report.h"
#include "command_report_all_devices.h"
#include <extbio_rc2014.h>

uint8_t main(const int argc, const char *argv[]) {
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

  case cmd_format_floppy:
    return format_floppy();

  case cmd_check_floppy_sectors:
    return check_floppy_sectors();

  case cmd_report_to_file:
    report_all_devices(l);
    printf("usb device details saved to file %s\r\n", report_file_name);
    break;

  case cmd_report_floppies:
    return report_floppies(l);

  case cmd_floppy_find:
    return floppy_find();
  }

  return 0;
}
