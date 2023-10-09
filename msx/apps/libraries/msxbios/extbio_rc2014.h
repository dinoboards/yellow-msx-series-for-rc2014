#ifndef __EXTBIO_RC2014
#define __EXTBIO_RC2014

#include <protocol.h>

/**
 * @brief Retrieve hardware probe status of the USB Module
 *
 * @return uint16_t low byte is the number of USB devices present, high byte is 1 if CH376 is present
 */
extern uint16_t extbio_usb_get_present(void);

#endif
