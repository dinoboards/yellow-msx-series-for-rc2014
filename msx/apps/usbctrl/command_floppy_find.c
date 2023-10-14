#include "command_floppy_find.h"
#include "arguments.h"
#include "device_search.h"
#include "ufi_reporters.h"
#include <msxdos.h>
#include <msxdos_rc2014_ex.h>
#include <stdio.h>

usb_error floppy_find(void) {
  driveLetterInfo info;
  memset(&info, 0, sizeof(driveLetterInfo));

  uint8_t result = msxdosGetDriveLetterInfo(floppy_drive_letter - 'A', &info);

  printf("Floppy drive letter %c\r\n", floppy_drive_letter);
  printf("slot %02X, ", info.phyical.slot);
  printf("segment %d, ", info.phyical.segment);
  printf("number %d, ", info.phyical.number);
  printf("index %d, ", info.phyical.index); //<<-- this is the index within the driver
  printf("lun %d\r\n", info.phyical.lun);

  msxdosLunInfo lunInfo;
  memset(&lunInfo, 0, sizeof(msxdosLunInfo));

  result = msxdosDrvLunInfo(info.phyical.slot, info.phyical.index, info.phyical.lun, &lunInfo);

  printf("lunInfo.mediumType %d\r\n", lunInfo.mediumType);
  printf("lunInfo.sectorSize %d\r\n", lunInfo.sectorSize);
  printf("lunInfo.sectorCount %d\r\n", lunInfo.sectorCount);
  printf("lunInfo.flags %d\r\n", lunInfo.flags);
  printf("lunInfo.cylinders %d\r\n", lunInfo.cylinders);
  printf("lunInfo.heads %d\r\n", lunInfo.heads);
  printf("lunInfo.sectorsPerTrack %d\r\n", lunInfo.sectorsPerTrack);
  printf("lunInfo.suitableForPartitioning %d\r\n", lunInfo.suitableForPartitioning);
  printf("lunInfo.number %d\r\n", lunInfo.number);

  rc2014LunInfoExtended lunInfoEx;
  memset(&lunInfoEx, 0, sizeof(rc2014LunInfoExtended));

  uint16_t x = rc2014GetLunInfoEx(info.phyical.slot, info.phyical.index, info.phyical.lun, &lunInfoEx);
  if (!(lunInfoEx.extendedFlags & 0x01)) {
    printf("Not USB floppy\r\n");
    return 255;
  }

  printf("rc2014GetLunInfoEx %04X\r\n", x);
  printf("lunInfoEx.mediumType %d\r\n", lunInfoEx.extendedMediumType);
  printf("lunInfoEx.extendedFlags %02X\r\n", lunInfoEx.extendedFlags);
  printf(" address %02X\r\n", lunInfoEx.extendedInfo.usbInfo.address);
  printf(" configIndex %02X\r\n", lunInfoEx.extendedInfo.usbInfo.configIndex);
  printf(" interfaceIndex %02X\r\n", lunInfoEx.extendedInfo.usbInfo.interfaceIndex);

  device_interface.address         = lunInfoEx.extendedInfo.usbInfo.address;
  device_interface.config_index    = lunInfoEx.extendedInfo.usbInfo.configIndex;
  device_interface.interface_index = lunInfoEx.extendedInfo.usbInfo.interfaceIndex;

  ufi_format_capacities_response r;
  memset(&r, 0, sizeof(r));

  device_config storage_device;
  memset(&storage_device, 0, sizeof(storage_device));

  result = construct_device_config(&storage_device);
  if (result)
    return result;

  printf("storage_device.address %02X\r\n", storage_device.address);
  printf("storage_device.interface_number %02X\r\n", storage_device.interface_number);
  printf("storage_device.type %02X\r\n", storage_device.type);
  printf("storage_device.max_packet_size %02X\r\n", storage_device.max_packet_size);
  for (uint8_t i = 0; i < 3; i++) {
    printf("storage_device.endpoints[%d].number %02X\r\n", i, storage_device.endpoints[i].number);
    printf("storage_device.endpoints[%d].toggle %02X\r\n", i, storage_device.endpoints[i].toggle);
    printf("storage_device.endpoints[%d].max_packet_sizex %02X\r\n", i, storage_device.endpoints[i].max_packet_sizex);
  }

  result = retrieve_floppy_formats(&storage_device, &r);
  if (result)
    return result;

  log_ufi_format_capacities_response(&r);

  return result;
}
