#include "ftdi_drv.h"
#include "usb_state.h"
#include <usb/dev_transfers.h>
#include <vendor_ftdi.h>

usb_error extbio_rc2014_ftdi(const uint8_t ftdi_fn, uint8_t *params) __sdcccall(1) {
  (void)ftdi_fn;
  (void)params;
  return 0;
  // switch(ftdi_fn) {
  //   case FTDI_FN_SET_BAUDRATE:
  //     return extbio_ftdi_set_baudrate((int32_t*)params);
  // }

  // return USB_ERR_FAIL;
}

usb_error extbio_ftdi_set_baudrate(int32_t *const baudrate) __sdcccall(1) {
  (void)baudrate;
  return 0;
  // device_config_ftdi *const ftdi_config = (device_config_ftdi *)find_device_config(USB_IS_FTDI);

  // if (ftdi_config == NULL)
  //   return USB_ERROR_DEVICE_NOT_FOUND;

  // return ftdi_set_baudrate(ftdi_config, baudrate);
}
