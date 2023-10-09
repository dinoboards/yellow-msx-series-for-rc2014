#ifndef REPORTERS_H
#define REPORTERS_H

#include "protocol.h"
#include <stdio.h>

void msx_fprintf(int file_handle, const char *const fmt, ...);

void report_device_descriptor(const device_descriptor *const p, const int file_handle) __sdcccall(1);
void report_device_configuration(const config_descriptor *const config, const int file_handle) __sdcccall(1);
void report_device_interface(const interface_descriptor *const interface, const int file_handle) __sdcccall(1);
void report_device_endpoint(const endpoint_descriptor *const endpoint, const int file_handle) __sdcccall(1);

#endif
