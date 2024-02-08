---
title: Yellow MSX Manual
date: $version$
toc: false
toc-own-page: false
titlepage-logo: "docs/dino.png"
logo-width: 70mm
...

\backgroundsetup{
  position=current page.east,
  angle=-90,
  nodeanchor=east,
  vshift=-3mm,
  opacity=1,
  scale=3,
  contents=Draft
}

$include{"book-common.md"}$

# Preface {.unlisted .unnumbered}

\begin{wrapfigure}{r}{0.40\textwidth}
  \centering
  \includegraphics[width=0.40\textwidth]{docs/full-kit-profile-1.jpg}
\end{wrapfigure}


So you have finished building and assembling your MSX kits and you've put the soldering iron way.  You have a working MSX compatible computer built with your very own hands! Congratulations and well done - that's an achievement you certainly can be proud of.

But now what, you ask?  Perhaps you have typed a few commands at the DOS prompt.  Perhaps you figured out how to load a game or write a little BASIC program and run.  But you want to explore and learn more.  What can you make this little machine do?

Well this manual is here to help.  Whether you have lots of previous experience with 8 bit computers from the 80s or not, this guide will help you in your continuing exploration of all things 8 bit retro.

\small
\begin{noteblock}
\begin{raggedright}
The Yellow MSX is a retro self assembled computer kit.  Its made up of a number of modules (CPU, Memory, Storage, Sound, Keyboard and Video), that once assembled together into a working computer - give you a fully operational MSX compatible computer.

Due to its modular nature, you can customise and extend the system in many ways, including installation of other RC2014 compatible modules.

More details can be found on the official GitHub page:

https://github.com/vipoo/yellow-msx-series-for-rc2014/tree/main?tab=readme-ov-file\#yellow-msx-series-for-rc2014
\end{raggedright}
\end{noteblock}
\normalsize

\pagebreak

# Glossary {.unlisted .unnumbered}

|Term|Meaning|
|-|------|
|8-bit|Refers to the computer's central processing unit (CPU) architecture, where each data unit is 8 bits long.|
|Assembler|A program that converts assembly language code into machine code for the computer's CPU.|
|BIOS|Basic Input/Output System, a set of software instructions stored in ROM that initializes hardware during the boot process.|
|Cartridge|External plug-in modules containing additional software or hardware enhancements for the computer.|
|CLI|Command Line Interface - A text-based interface where users interact with the computer by entering commands.|
|CPU|Central Processing Unit, the primary component responsible for executing instructions in the computer.|
|I/O|Input/Output - Refers to the communication between the computer and external devices.|
|MSX-BASIC|The built-in programming language for MSX computers, providing a simple way to write and run programs.|
|MSX™|A standardized home computer architecture developed by Microsoft and ASCII Corporation in the early 1980s.|
|Palette|A set of colors available for use in graphics and display.|
|Pixel|Short for picture element, the smallest unit of display on a computer screen.|
|RAM|Random Access Memory, the volatile memory used by the computer for temporary storage of data and program code.|
|ROM|Read-Only Memory, a type of non-volatile memory that stores the computer's firmware or permanent software.|
|Sprite|A graphical object that can be moved independently on the screen, commonly used in 2D gaming graphics.|
|VDP|Video Display Processor, a dedicated chip responsible for handling graphics and video output.|
|RC2014™|A simple 8 bit Z80 based modular computer kit system (see https://rc2014.co.uk/)|

\small
\begin{cautionblock}
\begin{raggedright}
The term 'RC2014' is a registered trademark of RFC2795 Ltd. The Yellow MSX kits are not affiliated, authorized or endorsed by RFC2795 Ltd.

All other trademarks mentioned in this document are the property of their respective owners and any use of these trademarks also does not imply affiliation, authorisation or endorsement.
\end{raggedright}
\end{cautionblock}
\normalsize

\newpage
\hypersetup{linkcolor=}
\tableofcontents

\newpage

# System Overview

## Official Yellow MSX Modules

Below is a list of all the MSX Kit Modules available.

|Module|Description|MSX|RomWBW|
|--|------|-|-|
|12+1 Backplane|Support up to 12 Modules & Cartridge Slot Extension|Required|Yes|
|Cartridge Slot|Add support for 2 MSX Cartridges|Optional|No|
|Cassette+USB|Compatible cassette interface and a USB Port|Optional|No|
|Game|Add basic sound and game controller inputs|Required|Yes|
|Keyboard|73 Key MSX keyboard|Required|Yes|
|Memory|1024K RAM/512K ROM Module|Required|No|
|Music|Extended Music Support (YM2413)|Optional|No|
|PPI|Keyboard interface & Memory Slot signals|Required|Yes|
|RTC|Real time clock|Optional|Yes|
|Turbo CPU|Z80 CPU operating from 3.5Mhz to 20Mhz|Required|Yes|
|V99x8 MSX RGB|MSX RGB video output|Required|Yes|
\small
\begin{tipblock}
It is possible to use the standard RC2014 Backplane Pro for some modules, but as not all lanes are supplied, some additional wiring will be required. The Cartridge Slot Extension, though is only compatible with the 12+1 Backplane.
\end{tipblock}
\begin{tipblock}
The RC2014 Z80 CPU and Dual Clock modules could be used as an alternative to the Turbo CPU.
\end{tipblock}
\normalsize

## Compatible RC2014 Modules

Some of the official and 3rd-party RC2014 Modules will work in a system configured for MSX operation.  These includes:

|Module|Notes|
|-|--|
|Z80 CPU v2.1 & Dual Clock|Alternative for the Turbo CPU Module.  Clock must be configured at 3.5Mhz|
|Compact Flash|Must be formatted for MSX (see FDISK utility)|
|Dual Serial SIO/2|Only the B Port supported|
|Bubble LED Display|Controlled through MSX Basic|
|Others|So long as there are no port conflicts there is a good chance it will work in a MSX configured kit|

\pagebreak

## System ROM

The Memory Module, includes a 512K boot ROM.  On powerup and/or reset, the system will execute the code flashed within. The ROM image contains numerous sub-systems for the operations of the computer - and there are numerous pre-built images available for you to download to suit your requirements.

The ROM images available for download are for the most part, identical.  The key differences are the choice of system BIOS/MSX-BASIC and country/date formatting rules.

The following table describes the key subsystem that the various system ROMs contain.

|Unit|Description|
|-|--|
|SYSTEM BIOS|Manages the bootup sequence and provides basic I/O access for common hardware devices (keyboard, memory paging, video).|
|MSX-DOS/NEXTOR KERNEL|The core kernel of the disk operating system built for the MSX platform|
|DISK DRIVERS|Disk drivers written for this platform (RC2014 Compact Flash, USB storages and others)|
|EXTENDED BIOS|RC2014 Platform specific services|
|EMBEDDED BOOT DISK|A small read only disk image containing MSX-DOS boot files and other utilities, to enable the platform to boot into MSX-DOS, when no external storage devices are detected|
|MSX-BASIC|A specific country configured build of MSX BASIC|

There are 2 core image types that you can choose to install on your system:

1. CBIOS system, configured for 50Hz or 60Hz video display rate
2. MSX-BASIC system, configured for specific country and format defaults

### CBIOS images

The CBIOS image includes all units, except for MSX-BASIC.  It supports loading and running the vast majority of MSX-DOS programs and games.

As it does not include MSX-BASIC, some programs and all BASIC programs will not work.

You can find more information of CBIOS at their website: https://cbios.sourceforge.net/

There are 2 variations of the CBIOS image. One for 60Hz video displays and the other for 50Hz. Both of these images will work if your output is RGB.  If you are using composite or S-Video output, you will need to ensure the correct frequency for your display type (eg: 50Hz for PAL, 60Hz for NTSC).

### MSX-BASIC images

These are the recommended images, as they enable most compatibility. With one of these images, you should be able to run all MSX-DOS, GAMES and BASIC programs.

At time of writing images with the following configurations are available:

|Country|Frequency|Char Set|Date Format|
|-|-|-|-|
|Australia|50Hz|International|D-M-Y|
|Japan|60Hz|Japanese|Y-M-D|
|USA|60Hz|International|M-D-Y|
|International|50Hz|International|M-D-Y|
|UK|50Hz|International|D-M-Y|
|France|50Hz|International|D-M-Y|
|Germany|50Hz|International|D-M-Y|
|Italy|50Hz|International|D-M-Y|
|Spain|50Hz|International|M-D-Y|
|Arabic|60Hz|Japanese|Y-M-D|
|Korean|60Hz|Japanese|Y-M-D|

### MSX-DOS/NEXTOR Operating System

Both the CBIOS and MSX-BASIC ROM versions will boot into the NEXTOR version of MSX-DOS operating system. After the boot report is presented on the console, you will be presented with the standard MSX-DOS prompt (same as MS-DOS).  Eg: `A>`.

For more information on using NEXTOR/MSX-DOS have a look at the official *Nextor User Manual* and the *Nextor Getting Started Guide* documents.

If you have one of the MSX-BASIC images loaded in your ROM.  You can start basic, by typing BASIC at the DOS prompt:

```
A>BASIC
```

\newpage

# RC2014 MSX Extensions

## BIOS BOOT

During boot/powerup the hardware components are scanned, tests conducted, and then reported to the console.

The systems scanned and reported are:

|Item|Reports|
|-|---|
|CPU|Detects the CPU speed (3.5Mhz or Turbo 20Mhz)|
|COMPACT FLASH *|Reports the capacity if detected|
|MUSIC ROM *|Reports if a disk image detected in the MSX MUSIC module|
|MSX RTC/F4|Reports if the module is operational|
|MSX MUSIC|Reports if the module is operational|
|MSX GAME|Reports if the module is operational|
|V99x8|Reports if the V9938 or V9958 VDP is installed & its refresh rate (50Hz or 60Hz)|
|CH376 *|Reports if Cassette+USB module is present|
|USB *|Reports any detected USB devices|

\* These devices are installed within the Nextor RC2014 Kernel Driver.

## RC2014 Nextor Drivers

### CH376 USB Drivers

The CH376 driver will report to the console any detected USB devices.

The supported USB devices are:

|Item|Report|
|-|---|
|Keyboard|Standard USB HID Keyboard|
|Printer \*|USB to Centronics Parallel Adapter|
|Mass Storage|Flash, HDD, and other support storage devices|
|Floppy Disk|USB to 3.5Mb Floppy disk drives|
|UNKNOWN|An unknown and unsupported USB device|
|CDC \*|Ethernet Network Adapter*|

\* Runtime drivers are required to be installed for operation. See the System Services and Drivers Section.

\small
\begin{importantblock}
\begin{raggedright}

The USB drivers do not support hot swapping of devices.  Drives will not be re-mounted if removed and re-inserted.  Always reset the computer after inserting or removing any USB devices.

\end{raggedright}
\end{importantblock}
\normalsize

### Compact Flash

The Nextor RC2014 Kernel Driver will detect and allow the use of the RC2014 Compact Flash module.

The use of `FDISK` is required to initialise the disk with FAT12/FAT16 partitions.

### Music Module's Embedded Disk

The MSX MUSIC module's ROM spare capacity can support an optional disk image.  If detected the Nextor RC2014 Kernel Driver will mount it as a read only disk.

### USB Mass Storage

The Nextor RC2014 Kernel Driver will detect and mount Mass Storage devices such as Flash drives and USB Hard Disks.

### USB 3.5MB Floppy Drives

The Nextor RC2014 Kernel Driver will detect and mount any floppy drives/disks detected.

### Embedded Boot Disk

On boot, if no disks devices are found containing a bootable MSX-DOS/NEXTOR image, then the system
will boot from the read-only disk image included in the system ROM.

\small
\begin{noteblock}
\begin{raggedright}
Only drives that have the NEXTOR.SYS and COMMAND2.COM files on a standard FAT12/FAT16 image are bootable.

You can prepare any external device for booting, by using the included `FDISK.COM` application to prepare a FAT12/FAT16 image.  Once the FAT format has been applied, you can then copy the operating system files (`NEXTOR.SYS` and `COMMAND2.COM`) to the new disk image.  On the next boot, the system will boot from that disk.
\end{raggedright}
\end{noteblock}
\normalsize

### Disk Boot Order

On powerup, the Nextor RC2014 Kernel Driver will scan and mount any disk drives detected.  See section *3.2. Booting Nextor* in the `Nextor User Manual`.

The physical device mount order is as per below.

1. USB Storage drives (upto 4 supported)
2. Compact Flash Storage
3. Music Module's ROM (optional read only disk image)
4. Memory Module's ROM (read only embedded disk image)

Devices are assigned by the kernel starting from `A:`; the consequence of this is, if the attached devices are changed (ie: a USB Flash Drive is inserted or removed), the other devices may be assigned a different drive letter.

The mount order of USB drives is to always mount Floppy Drives before the Mass Storage drives.

### Example of boot disk configuration

#### A single USB Flash Drive

If you have a system with a single USB flash disk only, the mount order would be:

    A: USB flash Drive
    B: Embedded ROM disk image.

> The system will boot from the first disk that has the NEXTOR.SYS and COMMAND2.COM files.

#### Two USB Flash Drives

In this configuration, the USB disks are mounted first, then the embedded ROM disk.

    A: First detected USB disk
    B: Second detected USB disk
    C: Embedded ROM disk image

#### A USB Flash Drive, a USB Floppy Drive and the Compact Flash Module

In this configuration, the Floppy Drives are always mounted first.  The rest are mounted as per below:

    A: USB Floppy Drive
    B: USB Flash Drive
    C: Compact Flash Module
    D: Embedded ROM disk image

\newpage

# Applications & Services

## System Services and Drivers

A custom built *Services* framework can be used to load various drivers and other support services at boot into RAM.  This allows the Yellow MSX system to support a large range of additional hardware extensions without the need for all driver software to fit within the 512K system ROM.

This framework is similar to the MSX [MemMan](https://www.msx.org/wiki/MemMan) system.

\small
> MemMan has an unknown compatibility issue with the Yellow MSX platform, and as at writing, has not been resolved.
\normalsize


This service framework, allows the installation of TSR (Terminate Stay Resident) software.  The services are copied into a free segment of 16K page, thus do not consume any of the TPA that is available for standard MSX-DOS and Basic programs.

All service TSR programs have the extension `.SYS`, eg: `PRNT.SYS`

To installed a service, you can use the `SERVICE.COM` application to load them. For example to install the USB printer driver, you can run the following command.

```
SERVICE=PRNT.SYS
```

This command can be placed into your AUTOEXEC.BAT script, so that the printer driver is always loaded at system start.

\newpage

## Services

$include{"../bin/sys-manuals.md"}$

\newpage

## Yellow MSX specific Applications

The Yellow MSX for RC2014 in addition to all the standard MSX-DOS applications, includes some programs specifically written and designed for the system and its hardware modules.

$include{"../bin/app-manuals.md"}$

\newpage

## Other 3rd Party Applications

The following MSX-DOS applications have been tested and are available within the releases archives published on the projects GitHub releases.

$include{"../bin/3rd-app-manuals.md"}$

\newpage

\

\newpage


