#ifndef __EXTBIO_RC2014
#define __EXTBIO_RC2014

#include <protocol.h>

/**
 * @brief Retrieve hardware probe status of the USB Module
 *
 * @return uint16_t low byte is the number of USB devices present, high byte is 1 if CH376 is present
 */
extern uint16_t extbio_usb_get_present();


/**
 * @brief Retrieve the usb device descritor for the usb device at the specified address
 *
 * @param device_address the address the device has been configured to respond to
 * @param p_device_descriptor a buffer to store the device descriptor in - must be in the top page
 * @return uint16_t low byte is 0 if no error, otherwise the error code
 */
extern uint16_t extbio_usb_get_device_descriptor(const uint8_t device_index, device_descriptor *const p_device_descriptor);

#endif
