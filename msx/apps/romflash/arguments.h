#ifndef __ARGUMENTS
#define __ARGUMENTS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { ROM_TYPE_UNDEFINED, ROM_TYPE_EXTENDED, ROM_TYPE_DRIVER } RomType;

extern RomType     flash_rom_type;
extern const char *flash_file_name;

void process_cli_arguments(const int argc, const char **argv);

#endif
