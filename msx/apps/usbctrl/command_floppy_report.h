#ifndef __COMMAND_FLOPPY_REPORT_H
#define __COMMAND_FLOPPY_REPORT_H

#include <ch376.h>
#include <stdbool.h>
#include <stdint.h>

usb_error report_floppies(const uint8_t last_device_address) __sdcccall(1);

#endif
