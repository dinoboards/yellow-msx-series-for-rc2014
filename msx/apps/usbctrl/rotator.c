#include "rotator.h"
#include <stdint.h>
#include <stdio.h>

static unsigned char advance[4] = {'-', '\\', '|', '/'};
static uint8_t       rotate_number;

void rotate_init(void) {
  rotate_number = 1;
  printf("%c", advance[0]);
}

void rotate(void) { printf("\b%c", advance[rotate_number++ % 4]); }

void rotate_stop(void) { printf("\b"); }

void erase_line(void) {
  printf("\r");
  for (int i = 0; i < 39; i++)
    printf(" ");
  printf("\r");
}
