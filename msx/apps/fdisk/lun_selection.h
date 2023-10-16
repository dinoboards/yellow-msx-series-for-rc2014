#ifndef LUN_SELECTION_H
#define LUN_SELECTION_H

#include <msxdos.h>
#include <stdint.h>

extern void           goLunSelectionScreen(uint8_t deviceIndex);
extern uint8_t        selectedLunIndex;
extern msxdosLunInfo *selectedLun;
extern msxdosLunInfo  luns[MAX_LUNS_PER_DEVICE];
extern uint8_t        availableLunsCount;

#endif
