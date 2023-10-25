#ifndef PRINTER_DRV_H
#define PRINTER_DRV_H

#include "work-area.h"

extern uint8_t USBPRT(const uint8_t ch) __sdcccall(1);
extern void    install_printer(void);
extern void    drv_timi_printer(void);

#endif
