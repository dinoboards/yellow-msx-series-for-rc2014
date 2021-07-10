#include "crt_override.h"
#include "msxdos.h"
#include <stdlib.h>
#include <string.h>

// z88dk MSXDOS CRT does not seem to supply the argument correctly
// manually implemented a crude parser
int main() {
  char *        argv[20];
  uint8_t       argc = 0;
  uint8_t       i = 0;
  const uint8_t len = strlen(msxdosCommand);

  for (i = 1; i < len; i++)
    if (msxdosCommand[i] == ' ')
      argc++;

  i = 0;
  char *token = strtok(msxdosCommand + 1, " ");

  while (token != NULL) {
    argv[i++] = token;
    token = strtok(NULL, " ");
  }

  argv[i++] = NULL;

  return _main(argv, argc);
}
