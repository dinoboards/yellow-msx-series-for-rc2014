#ifndef _COMMAND_REPORT_ALL_DEVICES_H
#define _COMMAND_REPORT_ALL_DEVICES_H

#include <ch376.h>
#include <stdbool.h>
#include <stdint.h>

usb_error command_report_all_devices(const uint8_t last_device_address) __sdcccall(1);

#endif
