#ifndef _TTY_H
#define _TTY_H

#include <stdint.h>

typedef struct {
  uint8_t console : 1; /* keyboard and crt*/
  uint8_t com : 2;     /* zero not maped, postive number use that port*/
  uint8_t lpt : 1;     /* can not map both com and lpt outout at same time */
} tty_connections_out_t;

typedef struct {
  uint8_t console : 1; /* keyboard and crt*/
  uint8_t com : 2;     /* zero not maped, postive number use that port*/
} tty_connections_in_t;

typedef struct {
  tty_connections_in_t  inputs;
  tty_connections_out_t outputs;
} tty_settings_t;

extern uint8_t tty_update(const tty_settings_t *const settings) __sdcccall(1);

#endif
