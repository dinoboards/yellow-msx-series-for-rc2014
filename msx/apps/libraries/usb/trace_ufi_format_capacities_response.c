#include "usb_trace.h"

void log_ufi_format_capacities_response(const ufi_format_capacities_response const *response) {
  printf("Descriptor Code: %02X, Length: %d\r\n", response->capacity_list_length);

  const uint8_t count = response->capacity_list_length / 8;

  for (uint8_t i = 0; i < count; i++) {
    const uint32_t sector_size = response->descriptors[i].block_size[1] << 8 + response->descriptors[i].block_size[0];

    printf("  SEC_SIZ: %ld, NUM_OF_SEC: %ld\r\n", sector_size, convert_from_msb_first(response->descriptors[i].number_of_blocks));
  }
}
