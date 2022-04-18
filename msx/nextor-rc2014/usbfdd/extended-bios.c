#include <stdbool.h>
#include <stdlib.h>

#include "print.h"

typedef struct {
  uint8_t a;
  uint8_t f;
  uint16_t bc;
  uint16_t de;
  uint16_t hl;
} registers;

// __z88dk_fastcall
uint8_t fn_enum() {
  printf("fn_enum\r\n");
  // yprintf(3, "a = %02x, bc=%04x, de=%04x, hl=%04x", r->a, r->bc, r->de, r->hl);
  return 1;
}
