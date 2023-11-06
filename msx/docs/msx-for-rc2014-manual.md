
# RC2014 MSX Manual

Details of applications, services and configuration that is specific to the Yellow MSX for RC2014 system.

# Boot Environment

There are two basic ROM versions available for the platform (CBIOS or MSX-BASIC)

## CBIOS/NEXTOR:

* Available in a 50Hz or 60Hz build
* Does not include MSX-BASIC
* Should be able to run all non-basic software
* includes RC2014 specific bios Extensions

## MSX-BASIC/NEXTOR:
* available in various country configurations (date, time, number and keyboard configs)
* Does include a working copy of MSX-BASIC
* Includes RC2014 specific bios Extensions

# RC2014 Extensions

The rom images mentioned above include bios extensions (using the MSX EXTBIO interface), to
provide application services specific to the platform.

These allow control of items such as the RC2014 SIO/2 module
and interacting/enumerating USB devices attached to the Cassette+USB module.

See [docs/extended-bios.md](docs/extended-bios.md) for details of the specific enhancements

## SIO/2

The embedded ROM includes a EXTBIO extension that installs a fossil compatible serial interface to drive the RC2014 SIO/2 module.

See the applications, `xrecv`, `term`, `exp8266`

## Compact Flash

The embedded ROM NEXTOR driver will detect and allow the use of the RC2014 Compact flash modules.

The use of `FDISK` is probably required to initialise the disk with FAT12/FAT16 partitions.

## Embedded Boot Disk

On boot, if no disks devices are found containing a bootable MSX-DOS/NEXTOR image, then the system
will boot from a read-only disk image included in the ROM.

If you do have a compatible disk images (USB or CF), you can use `FDISK.COM` on the embedded boot image to prepare your desired boot device, and then copy the `NEXTOR.SYS` and `COMMAND2.COM` (and optionally the `AUTOEXEC.BAT`) files
from this embedded image to the device.  When you reboot, the system should boot from attached device in preference to the embedded ROM disk image.

## TSR/SERVICE framework

To enable support for additional hardware not supported by the embedded ROM, a TSR (Terminate Stay Resident) service
framework has been developed.

This system allow the loading within MSX-DOS's `AUTOEXEC.BAT`, to install additional drivers or other
supporting software.

For example, to support USB printers, the `PRNT.SYS` service, can be installed to enable MSX-DOS to print via
an attached USB to Centronics adapter.

To install,  add the following to `AUTOEXEC.BAT`:

```
SERVICE=PRNT.SYS
```

