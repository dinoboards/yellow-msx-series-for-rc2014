$define{doc_title}{Yellow MSX For RC2014 Manual}$
$include{"book.latex"}$

# System Overview

The Yellow MSX FOR RC2014 is a kit computer compatible with the MSX computer system from the 1980s, based on the physical architecture and system BUS of the RC2014 kit computer.

\begin{wrapfigure}{r}{0.40\textwidth}
  \centering
  \includegraphics[width=0.40\textwidth]{full-kit-profile-1.jpg}
\end{wrapfigure}


The system is designed around a number of modules for various sub-systems, such as CPU, Memory, Storage, Sound, Keyboard and Video.  Each of these modules are connected into a  backplane.

Many of the official RC2014 and compatible RC2014 modules can also be installed into the system.

More specific details for each of the Yellow MSX kits can be found on the project's official GitHub page https://github.com/vipoo/yellow-msx-series-for-rc2014/tree/main?tab=readme-ov-file#summary-of-all-kits

\pagebreak

## Official Yellow MSX Modules

Below is a list of all the MSX Kit Modules available.

|Module|Description|MSX|RomWBW|
|--|------|-|-|
|12+1 Backplane|Support up to 12 Modules, and Cartridge Slot Extension|Yes|Yes|
|Cartridge Slot|Add support for 2 MSX Cartridges|Optional|No|
|Cassette+USB|Compatible cassette interface and a USB Port|Optional|No|
|Game|Add basic sound and game controller inputs|Yes|Yes|
|Keyboard|73 Key MSX keyboard|Yes|Yes|
|Memory|1024K RAM/512K ROM Module|Yes|No|
|Music|Extended Music Support (YM2413)|Optional|Yes|
|PPI|Keyboard interface &  Memory Slot signals|Yes|No|
|RTC|Real time clock|Optional|Yes|
|Turbo CPU|Z80 CPU operating from 3.5Mhz to 20Mhz|Yes|Yes|
|V99x8 MSX RGB|MSX RGB video output|Yes|Yes|

\* It is possible to use the standard RC2014 Backplane Pro. Some additional wiring required for specific modules. Cartridge Slot Extension requires the 12+1 Backplane.|

## Compatible RC2014 Modules

Some of the official and 3rd-party RC2014 Modules will work in a system configured for MSX operation.  These includes:

|Module|Notes|
|-|--|
|Z80 CPU v2.1 & Dual Clock|Clock must be configured at 3.5Mhz|
|Compact Flash|Must be formatted for MSX (see FDISK utility)|
|Dual Serial SIO/2|Only the B Port supported|
|Bubble LED Display|Controlled through MSX Basic|
|Others|So long as there are no port conflicts there is a good chance it will work in MSX configured kit|


## System ROM

On powerup and/or reset, the system will run the code within the ROM of the Memory Module. This single ROM image contains various sub-systems.  The configuration and choice of sub-system is flexible and can be customised as needed.

|Unit|Description|
|-|--|
|SYSTEM BIOS|Manages the bootup sequence and supporting services for common hardware devices (keyboard, memory paging, video)|
|MSX-DOS/NEXTOR KERNEL|The core kernel of disk operating system built for the MSX platform|
|DISK DRIVERS|Disk drivers written for this platform (RC2014 Compact Flash, USB storages and others)|
|EXTENDED BIOS|Additional hardware support code for the platform (SIO/2, general USB)|
|EMBEDDED BOOT DISK|A small read only disk image containing MSX-DOS boot files and other utilities, to enable the platform to boot into MSX-DOS, when no external storage devices are detected|
|MSX-BASIC|A specific country configured build of MSX BASIC|

There are 2 core image types that can be installed on your system:

1. CBIOS Basic system, configured for 50Hz or 60Hz video display rate
2. MSX-BASIC system, configured for specific country and format defaults

### CBIOS images

The CBIOS image includes all units, except for MSX-BASIC.  It supports loading and running the vast majority of MSX-DOS programs and games.

As it does not include MSX-BASIC, some programs and all BASIC programs will not be work.

You can find more information of CBIOS at their website: https://cbios.sourceforge.net/

There are 2 variations of the CBIOS image. One for 60Hz video displays and the other for 50Hz. Both of these images will work if your output is RGB.  If you are using composite or S-Video output, you will need to ensure the correct frequency for your display type (eg: 50Hz for PAL, 60Hz for NTSC).

### MSX-BASIC images

These are the recommended images, as they enable most compatibility. With one of these images, you should be able to run all MSX-DOS, GAMES and BASIC programs.

At time of writing an image with the following configurations are available:

|Country|Frequency|Char Set|Date Format|
|-|-|-|-|
|Australia|50Hz|International|D-M-Y|
|Japan|60Hz|Japanese|Y-M-D|
|USA|60Hz|International|M-D-Y|
|international|50Hz|International|M-D-Y|
|UK|50Hz|International|D-M-Y|
|France|50Hz|International|D-M-Y|
|Germany|50Hz|International|D-M-Y|
|Italy|50Hz|International|D-M-Y|
|Spain|50Hz|International|M-D-Y|
|Arabic|60Hz|Japanese|Y-M-D|
|Korean|60Hz|Japanese|Y-M-D|

# RC2014 Extensions

The rom images mentioned above include bios extensions (using the MSX EXTBIO interface), to
provide application services specific to the platform.

These allow control of items such as the RC2014 SIO/2 module
and interacting/enumerating USB devices attached to the Cassette+USB module.

See [docs/extended-bios.md](docs/extended-bios.md) for details of the specific enhancements

## Compact Flash

The embedded ROM NEXTOR driver will detect and allow the use of the RC2014 Compact flash modules.

The use of `FDISK` is probably required to initialise the disk with FAT12/FAT16 partitions.

## Embedded Boot Disk

On boot, if no disks devices are found containing a bootable MSX-DOS/NEXTOR image, then the system
will boot from a read-only disk image included in the ROM.

If you do have a compatible disk images (USB or CF), you can use `FDISK.COM` on the embedded boot image to prepare your desired boot device, and then copy the `NEXTOR.SYS` and `COMMAND2.COM` (and optionally the `AUTOEXEC.BAT`) files
from this embedded image to the device.  When you reboot, the system should boot from attached device in preference to the embedded ROM disk image.

# Applications & Services

## System Services and Drivers

As the Yellow MSX system can support many different hardware extensions, appropriate driver software is required.  As there is limited space within the 512K ROM to store all the drivers required for all the potential hardware installed on your system, a dynamic service framework was developed.

This framework is similar to the MSX [MemMan](https://www.msx.org/wiki/MemMan) system.

> (MemMan has an unknown compatibility issue, and as at writing has not been resolved.)

This service framework, allows the installation of TSR (Terminate Stay Resident) software.  The services are copied into a free segment of 16K page, thus do not consume any of the TPA that is available for standard MSX-DOS and Basic programs.

All service TSR programs have the extension `.SYS`, eg: `PRNT.SYS`

To installed a service, you can use the `SERVICE.COM` application to load them. For example to install the USB printer driver, you can run the following command.

```
SERVICE=PRNT.SYS
```

This command can be placed into your AUTOEXEC.BAT script, so that the printer driver is always loaded at system start.

## Services

$include{"../bin/sys-manuals.md"}$

## Yellow MSX specific apps

The Yellow MSX for RC2014 in addition to all the standard MSX-DOS applications, includes some programs specifically designed for the system and its hardware modules.

$include{"../bin/app-manuals.md"}$
