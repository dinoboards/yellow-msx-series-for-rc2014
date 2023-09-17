
ch376
  * should only know about concepts at the ch376 - not usb specific device types
  * endpoints, address, toggles, etc ok

hw
  * should have common usb operation to transfer setups, bulk in/out, get/set descriptors etc.
  * rename to usb?

efi
  * should consume hw and expose EFI operations

scsi:
  * should consume hw and expose SCSI operations

printer
  * should consume hw and expose printer operations (single send text operation)

usb:
  * merge with hw?

usb-enumerate:
  * main enumeration function to discover usb devices
  * needs work-area and working scratch pad
  * consume hw operations



