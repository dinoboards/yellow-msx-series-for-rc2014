// CPU SPEED TEST

#define __Z88DK_R2L_CALLING_CONVENTION
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void             intOn();
extern void             intOff();
extern uint16_t         counter;
extern uint16_t         period;
extern volatile uint8_t intTriggered;

void main() {

  intOn();

  printf("spiking cpu speed measure\r\n");

  intTriggered = 0;
  while (intTriggered == 0)
    ;
  intTriggered = 0;
  while (intTriggered == 0) {
    counter++;
  }

  printf("Triggered count %d", counter);

  intOff();
}
