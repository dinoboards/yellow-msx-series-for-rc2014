#include "../ftdi.h"
#include <stdio.h>
#include <sysload/sysload.h>

void serial_get_available_ports(uint8_t *const available_ports) __z88dk_fastcall;

#define relocated(x) ((uint8_t *)((uint16_t)(x) + (uint16_t)(my_header)))

uint16_t init(installed_sys_t *my_header) __z88dk_fastcall {
  printf("intalling ftdi @ %p\r\n", my_header);

  uint8_t *const relocated_assigned_port_number = &relocated(&assigned_port_number);

  printf("assigned_port_number: %p, %d\r\n", relocated_assigned_port_number, *relocated_assigned_port_number);

  serial_get_available_ports(relocated_assigned_port_number);
  printf("serial_get_available_ports: %d\r\n", *relocated_assigned_port_number);
  *relocated_assigned_port_number += 1;
  printf("2.serial_get_available_ports: %d\r\n", *relocated_assigned_port_number);

  return 0;
}
