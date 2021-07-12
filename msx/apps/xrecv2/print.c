#include "print.h"
#include "stdio.h"

void print(char *s) __z88dk_fastcall { printf(s); }

void printChar(char c) __z88dk_fastcall { printf("%c", c); }
