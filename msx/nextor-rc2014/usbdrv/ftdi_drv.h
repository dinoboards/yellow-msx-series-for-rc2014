#ifndef _FTDI_DRV_H
#define _FTDI_DRV_H

#include <ch376.h>
#include <stdint.h>

#define FTDI_FN_SET_BAUDRATE 0x01

extern usb_error extbio_rc2014_ftdi(const uint8_t ftdi_fn, uint8_t *params) __sdcccall(1);
extern usb_error extbio_ftdi_set_baudrate(int32_t *const baudrate) __sdcccall(1);

#endif
