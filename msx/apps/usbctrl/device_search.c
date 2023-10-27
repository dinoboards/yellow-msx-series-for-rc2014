#include "device_search.h"
#include <msxdos.h>
#include <msxdos_rc2014_ex.h>
#include <protocol.h>
#include <usb/find_device.h>

usb_error find_next_floppy_interface(const uint8_t address, device_config_interface *const floppy_interface) __sdcccall(1) {
  uint8_t buffer[151];

  device_descriptor my_device_descriptor;
  memset(&my_device_descriptor, 0, sizeof(device_descriptor));

  usb_error result = usbtrn_get_descriptor2(&my_device_descriptor, address);
  if (result) {
    printf("usbtrn_get_descriptor2 failed %d\r\n", result);
    return result;
  }

  for (uint8_t config_index = 0; config_index < my_device_descriptor.bNumConfigurations; config_index++) {
    memset(buffer, 0, sizeof(buffer));
    result = usbtrn_get_full_config_descriptor(config_index, address, my_device_descriptor.bMaxPacketSize0, 150, buffer);
    if (result) {
      printf("usbtrn_get_full_config_descriptor failed %d\r\n", result);
      return result;
    }

    const config_descriptor *const config = (config_descriptor *)buffer;

    for (uint8_t interface_index = 0; interface_index < config->bNumInterfaces; interface_index++) {
      const interface_descriptor *const interface =
          (interface_descriptor *)(buffer + sizeof(config_descriptor) + interface_index * sizeof(interface_descriptor));

      if (interface->bInterfaceClass == 8 && interface->bInterfaceSubClass == 4 && interface->bInterfaceProtocol == 0) {
        floppy_interface->address         = address;
        floppy_interface->config_index    = config_index;
        floppy_interface->interface_index = interface_index;
        return USB_ERR_OK;
      }
    }
  }

  return 255;
}

usb_error get_device_config_from_drive_letter(device_config *storage_device) {
  driveLetterInfo info;
  memset(&info, 0, sizeof(driveLetterInfo));

  uint8_t result = msxdosGetDriveLetterInfo(floppy_drive_letter - 'A', &info);

  // printf("Floppy drive letter %c\r\n", floppy_drive_letter);
  // printf("slot %02X, ", info.phyical.slot);
  // printf("segment %d, ", info.phyical.segment);
  // printf("number %d, ", info.phyical.number);
  // printf("index %d, ", info.phyical.index); //<<-- this is the index within the driver
  // printf("lun %d\r\n", info.phyical.lun);

  msxdosLunInfo lunInfo;
  memset(&lunInfo, 0, sizeof(msxdosLunInfo));

  result = msxdosDrvLunInfo(info.phyical.slot, info.phyical.index, info.phyical.lun, &lunInfo);

  // printf("lunInfo.mediumType %d\r\n", lunInfo.mediumType);
  // printf("lunInfo.sectorSize %d\r\n", lunInfo.sectorSize);
  // printf("lunInfo.sectorCount %d\r\n", lunInfo.sectorCount);
  // printf("lunInfo.flags %d\r\n", lunInfo.flags);
  // printf("lunInfo.cylinders %d\r\n", lunInfo.cylinders);
  // printf("lunInfo.heads %d\r\n", lunInfo.heads);
  // printf("lunInfo.sectorsPerTrack %d\r\n", lunInfo.sectorsPerTrack);
  // printf("lunInfo.suitableForPartitioning %d\r\n", lunInfo.suitableForPartitioning);
  // printf("lunInfo.number %d\r\n", lunInfo.number);

  rc2014LunInfoExtended lunInfoEx;
  memset(&lunInfoEx, 0, sizeof(rc2014LunInfoExtended));

  uint16_t x = rc2014GetLunInfoEx(info.phyical.slot, info.phyical.index, info.phyical.lun, &lunInfoEx);
  if (!(lunInfoEx.extendedFlags & 0x01)) {
    // printf("Not USB floppy\r\n");
    return 255;
  }

  // printf("rc2014GetLunInfoEx %04X\r\n", x);
  // printf("lunInfoEx.mediumType %d\r\n", lunInfoEx.extendedMediumType);
  // printf("lunInfoEx.extendedFlags %02X\r\n", lunInfoEx.extendedFlags);
  // printf(" address %02X\r\n", lunInfoEx.extendedInfo.usbInfo.address);
  // printf(" configIndex %02X\r\n", lunInfoEx.extendedInfo.usbInfo.configIndex);
  // printf(" interfaceIndex %02X\r\n", lunInfoEx.extendedInfo.usbInfo.interfaceIndex);

  return construct_device_config(lunInfoEx.extendedInfo.usbInfo.address, lunInfoEx.extendedInfo.usbInfo.configIndex,
                                 lunInfoEx.extendedInfo.usbInfo.interfaceIndex, storage_device);
}

usb_error retrieve_floppy_formats(device_config *const storage_device, ufi_format_capacities_response *const r) __sdcccall(1) {
  usb_error result;

  wait_for_device_ready(storage_device, 2500);

  ufi_inquiry_response response;
  memset(&response, 0, sizeof(ufi_inquiry_response));
  result = ufi_inquiry(storage_device, &response);
  if (result) {
    printf("USB Error reading floppy's status: %d\r\n", result);
    return result;
  }

  wait_for_device_ready(storage_device, 1000);

  result = ufi_read_format_capacities(storage_device, r);
  if (result) {
    printf("USB Error reading floppy's format capacities: %d\r\n", result);
    return result;
  }

  return 0;
}

