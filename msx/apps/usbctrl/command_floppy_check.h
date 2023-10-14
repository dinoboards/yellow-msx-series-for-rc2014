#ifndef COMMAND_FLOPPY_CHECK_H
#define COMMAND_FLOPPY_CHECK_H

#include "ch376.h"

usb_error check_floppy_sectors(void) __sdcccall(1);

#endif
