#ifndef _DRIVER_SELECTION_H
#define _DRIVER_SELECTION_H

#include <msxdos.h>

extern msxdosDriverInfo  drivers[MAX_INSTALLED_DRIVERS];
extern msxdosDriverInfo *selectedDriver;
extern char              selectedDriverName[50];
extern uint8_t           installedDriversCount;

void goDriverSelectionScreen(void);

#endif
