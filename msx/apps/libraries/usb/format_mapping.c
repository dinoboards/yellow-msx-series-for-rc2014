
#include "format_mapping.h"

#define NUMBER_OF_DISK_GEOMETRIES 3

disk_geometry maps[NUMBER_OF_DISK_GEOMETRIES] = {
    {2880, 80, 2, 18, "1.44MB"}, {1440, 80, 2, 9, "720KB"}, {2400, 80, 2, 15, "1.2MB"}};

const disk_geometry *size_to_geometry(const uint16_t total_number_of_blocks) __sdcccall(1) {
  for (uint8_t i = 0; i < NUMBER_OF_DISK_GEOMETRIES; i++)
    if (maps[i].number_of_blocks == total_number_of_blocks)
      return &maps[i];

  return NULL;
}
