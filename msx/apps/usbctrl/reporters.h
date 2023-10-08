#ifndef REPORTERS_H
#define REPORTERS_H

#include "protocol.h"
#include <stdio.h>

void report_device_descriptor(const device_descriptor *const p, FILE *file_handle) __sdcccall(1);
void report_device_configuration(const config_descriptor *const config, FILE *file_handle) __sdcccall(1);
void report_device_interface(const interface_descriptor *const interface, FILE *file_handle) __sdcccall(1);

#endif
