#ifndef __FORMAT_FATS_H
#define __FORMAT_FATS_H

#include <ch376.h>
#include <dev_transfers.h>
#include <format_mapping.h>

usb_error format_fat(device_config *const storage_device, const disk_geometry *const geometry) __sdcccall(1);

#endif
