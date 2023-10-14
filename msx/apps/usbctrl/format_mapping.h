#ifndef __FORMAT_MAPPING
#define __FORMAT_MAPPING

#include <stdint.h>

typedef struct {
  uint16_t    number_of_blocks;
  uint8_t     tracks;
  uint8_t     sides;
  uint8_t     sectors_per_track;
  char *const description;
} disk_geometry;

const disk_geometry *size_to_geometry(uint16_t total_number_of_blocks) __sdcccall(1);

#endif
